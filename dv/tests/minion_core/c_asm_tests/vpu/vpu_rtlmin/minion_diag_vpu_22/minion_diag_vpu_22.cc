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
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00004
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0000c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00010
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00014
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00018
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0001c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00020
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00024
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00028
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00030
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00034
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00038
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0003c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00040
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00044
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00048
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0004c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00054
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00058
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0005c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00060
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00064
			 0xbf028f5c,                                                  // -0.51                                       /// 00068
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0006c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00070
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00074
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00078
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00084
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00088
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0008c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00090
			 0x0c600000,                                                  // Leading 1s:                                 /// 00094
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00098
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000a0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000a4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000a8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000ac
			 0x33333333,                                                  // 4 random values                             /// 000b0
			 0x0e000007,                                                  // Trailing 1s:                                /// 000b4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000bc
			 0x0c400000,                                                  // Leading 1s:                                 /// 000c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000c4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000c8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000cc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000d4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000d8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000dc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000ec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000f0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000f4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000f8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000fc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00104
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00108
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0010c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00110
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00114
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0011c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00128
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x55555555,                                                  // 4 random values                             /// 00130
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00134
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00138
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0013c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00140
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x0c780000,                                                  // Leading 1s:                                 /// 00148
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0014c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00154
			 0xbf028f5c,                                                  // -0.51                                       /// 00158
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00160
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00164
			 0x3f028f5c,                                                  // 0.51                                        /// 00168
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00174
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00178
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0017c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0xcb000000,                                                  // -8388608.0                                  /// 00184
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00188
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0018c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00190
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00194
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0019c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001a0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001a8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001ac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001b0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001b8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001bc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 001c0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001c4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001c8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001d4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001d8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001dc
			 0x00000000,                                                  // zero                                        /// 001e0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001e4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001ec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001f0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001f4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001f8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001fc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00200
			 0xffc00001,                                                  // -signaling NaN                              /// 00204
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0020c
			 0x4b000000,                                                  // 8388608.0                                   /// 00210
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00214
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00218
			 0x55555555,                                                  // 4 random values                             /// 0021c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00220
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00228
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0022c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00230
			 0xffc00001,                                                  // -signaling NaN                              /// 00234
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00238
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0023c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00240
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00244
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00248
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0024c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00254
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0025c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00260
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00264
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00268
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0026c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00270
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0027c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00280
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00284
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00288
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0028c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00290
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00294
			 0x0c600000,                                                  // Leading 1s:                                 /// 00298
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0029c
			 0x0c700000,                                                  // Leading 1s:                                 /// 002a0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002a8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002ac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002b0
			 0x7fc00001,                                                  // signaling NaN                               /// 002b4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002b8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002c0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002c4
			 0x55555555,                                                  // 4 random values                             /// 002c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002cc
			 0xbf028f5c,                                                  // -0.51                                       /// 002d0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002d8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002dc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002e0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002e8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002ec
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002f4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002f8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002fc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00300
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00304
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00308
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0030c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00310
			 0x7fc00001,                                                  // signaling NaN                               /// 00314
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00318
			 0x55555555,                                                  // 4 random values                             /// 0031c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0xbf028f5c,                                                  // -0.51                                       /// 00328
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0032c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00334
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00338
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0033c
			 0xff800000,                                                  // -inf                                        /// 00340
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0xffc00001,                                                  // -signaling NaN                              /// 00348
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0034c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00350
			 0x55555555,                                                  // 4 random values                             /// 00354
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00358
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x4b000000,                                                  // 8388608.0                                   /// 00360
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00364
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00368
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00374
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0037c
			 0xffc00001,                                                  // -signaling NaN                              /// 00380
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00384
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00388
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00390
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00394
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0039c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003a8
			 0x80000000,                                                  // -zero                                       /// 003ac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003b0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003b4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003b8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003bc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003c4
			 0xffc00001,                                                  // -signaling NaN                              /// 003c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003cc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003d0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003d4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003dc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003e8
			 0xbf028f5c,                                                  // -0.51                                       /// 003ec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003f0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003f4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003fc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00400
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00404
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00408
			 0xffc00001,                                                  // -signaling NaN                              /// 0040c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00410
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00414
			 0xcb000000,                                                  // -8388608.0                                  /// 00418
			 0x0e000003,                                                  // Trailing 1s:                                /// 0041c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00420
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00424
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00428
			 0x0c700000,                                                  // Leading 1s:                                 /// 0042c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00430
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00434
			 0x0c780000,                                                  // Leading 1s:                                 /// 00438
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x0e000001,                                                  // Trailing 1s:                                /// 00448
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0044c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x0c780000,                                                  // Leading 1s:                                 /// 00454
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00458
			 0x7f800000,                                                  // inf                                         /// 0045c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00460
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00468
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00470
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00474
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00478
			 0x4b000000,                                                  // 8388608.0                                   /// 0047c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00484
			 0x0c600000,                                                  // Leading 1s:                                 /// 00488
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0048c
			 0xffc00001,                                                  // -signaling NaN                              /// 00490
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00494
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0049c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004a0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004a4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004a8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004b0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004b4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004b8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004bc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004c0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004c8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004d4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004d8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004dc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004e0
			 0x0c780000,                                                  // Leading 1s:                                 /// 004e4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004e8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004ec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004f4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004fc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00500
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00508
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0050c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00510
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00518
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0051c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00520
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00524
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00528
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00530
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00534
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00538
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0053c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00548
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0054c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00550
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00554
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0055c
			 0x33333333,                                                  // 4 random values                             /// 00560
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00568
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0056c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00570
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00574
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00578
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0057c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00580
			 0x7f800000,                                                  // inf                                         /// 00584
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00588
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0058c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00590
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00594
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00598
			 0x33333333,                                                  // 4 random values                             /// 0059c
			 0x33333333,                                                  // 4 random values                             /// 005a0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005ac
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005b4
			 0x7f800000,                                                  // inf                                         /// 005b8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005bc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005c8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005cc
			 0x80000000,                                                  // -zero                                       /// 005d0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 005dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005ec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005f4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005f8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005fc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00600
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00604
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00608
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0060c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00614
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00618
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0061c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00620
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00628
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0062c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00630
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00634
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00638
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0063c
			 0xffc00001,                                                  // -signaling NaN                              /// 00640
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00644
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00648
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0064c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00654
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00658
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0065c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00660
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00664
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00668
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00670
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00674
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00678
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0067c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00680
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00684
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00688
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0068c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00694
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00698
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006a0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006ac
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006b0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006bc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006c0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006c8
			 0x3f028f5c,                                                  // 0.51                                        /// 006cc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006d0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006d8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006e0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006e4
			 0x7f800000,                                                  // inf                                         /// 006e8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x7fc00001,                                                  // signaling NaN                               /// 006fc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00700
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00704
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00710
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00714
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0071c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00720
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00724
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00728
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0072c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00730
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00738
			 0xbf028f5c,                                                  // -0.51                                       /// 0073c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00740
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00748
			 0x4b000000,                                                  // 8388608.0                                   /// 0074c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00750
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00754
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00758
			 0x55555555,                                                  // 4 random values                             /// 0075c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00760
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00764
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00768
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0076c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00770
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00774
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00778
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0077c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00780
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00784
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0078c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00790
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00794
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00798
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0079c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007a0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007a4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007b0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007b8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007bc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007c0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007c4
			 0x0e000007,                                                  // Trailing 1s:                                /// 007c8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007d4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007d8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007dc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007e4
			 0x3f028f5c,                                                  // 0.51                                        /// 007e8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007f0
			 0x0e000003,                                                  // Trailing 1s:                                /// 007f4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00800
			 0x0c700000,                                                  // Leading 1s:                                 /// 00804
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00808
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00810
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00814
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00818
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0081c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x7f800000,                                                  // inf                                         /// 00828
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00830
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00834
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0083c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00840
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00844
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00848
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00854
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00858
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0085c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00860
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x0c600000,                                                  // Leading 1s:                                 /// 00868
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0086c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00870
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00874
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00878
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0087c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00880
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00884
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0088c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00890
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00894
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0089c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008a4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008a8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008ac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008b0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008b4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008b8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008bc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008c0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008c4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008c8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008cc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008d4
			 0xcb000000,                                                  // -8388608.0                                  /// 008d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008e0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008e8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008f8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008fc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00900
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00904
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00908
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0090c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00910
			 0xffc00001,                                                  // -signaling NaN                              /// 00914
			 0x0c780000,                                                  // Leading 1s:                                 /// 00918
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0091c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00920
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00924
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00928
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0092c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00930
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00934
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0xff800000,                                                  // -inf                                        /// 0093c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00944
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0094c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00950
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00954
			 0x0c400000,                                                  // Leading 1s:                                 /// 00958
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0095c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00960
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00964
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00968
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0096c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00970
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0097c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00980
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00988
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00990
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00994
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00998
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0099c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009a0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009a8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009b0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009b4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009b8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009bc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009c0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009c4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009cc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009d4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009d8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009e0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009e4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009e8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009f0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009f4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009f8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009fc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a00
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a04
			 0x33333333,                                                  // 4 random values                             /// 00a08
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a10
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a18
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a1c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a20
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a24
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a28
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a30
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a34
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a38
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a3c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a44
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a48
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a50
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a54
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a58
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a5c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a60
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a64
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a6c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a70
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a74
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a78
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a7c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a84
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a8c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a90
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a94
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a98
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a9c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00aa4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00aac
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ab4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00abc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ac0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ac4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ac8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00acc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ad4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ad8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00adc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ae0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ae4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00aec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00af0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00af4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00af8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00afc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b00
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b04
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b08
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b0c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b10
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b14
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b18
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b20
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b24
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b28
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b2c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b30
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b34
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b40
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b44
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b4c
			 0xffc00001,                                                  // -signaling NaN                              /// 00b50
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b58
			 0x00000000,                                                  // zero                                        /// 00b5c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b64
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b68
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b6c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x00000000,                                                  // zero                                        /// 00b74
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b78
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b7c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b84
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b88
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b8c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b90
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ba0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ba4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ba8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bb0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bb4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bb8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bbc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bc0
			 0x80000000,                                                  // -zero                                       /// 00bc4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0xff800000,                                                  // -inf                                        /// 00bcc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bd0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bd4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x4b000000,                                                  // 8388608.0                                   /// 00bdc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00be0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00be4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00be8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bf0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bf4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bfc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c00
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c04
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c08
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c10
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c14
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0xbf028f5c,                                                  // -0.51                                       /// 00c1c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c20
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c24
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c28
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c2c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c34
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c38
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c40
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c48
			 0x4b000000,                                                  // 8388608.0                                   /// 00c4c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c58
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c64
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c6c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c70
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c74
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c80
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c84
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c88
			 0x33333333,                                                  // 4 random values                             /// 00c8c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c94
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c98
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c9c
			 0x55555555,                                                  // 4 random values                             /// 00ca0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cb0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cb4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cb8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00cbc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cc4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ccc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cd0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cd4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cd8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ce0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ce4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ce8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cf4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cf8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cfc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d00
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d04
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d08
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d0c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d10
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d18
			 0x4b000000,                                                  // 8388608.0                                   /// 00d1c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d20
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d24
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d2c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d30
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d34
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d38
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d3c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d40
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d44
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d48
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d4c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d50
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d54
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d58
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d5c
			 0x55555555,                                                  // 4 random values                             /// 00d60
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d64
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d68
			 0xff800000,                                                  // -inf                                        /// 00d6c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d74
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d78
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d7c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d80
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d84
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d88
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d8c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d90
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d94
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d98
			 0x7fc00001,                                                  // signaling NaN                               /// 00d9c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00da0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00da4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00da8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00db0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00db4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00db8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00dbc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00dc0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00dc8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00dd0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00dd4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00dd8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ddc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00dec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00df0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00df4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00dfc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e04
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e08
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e0c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e18
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x55555555,                                                  // 4 random values                             /// 00e20
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e24
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e2c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e30
			 0x55555555,                                                  // 4 random values                             /// 00e34
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e38
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e3c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e44
			 0x7fc00001,                                                  // signaling NaN                               /// 00e48
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e4c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e50
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e54
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e58
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e5c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e60
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e64
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e68
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e70
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e74
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x55555555,                                                  // 4 random values                             /// 00e7c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e80
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e84
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e88
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e8c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e90
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e94
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e9c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ea4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ea8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00eac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00eb0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ebc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ec4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ec8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ed4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ed8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00edc
			 0xff800000,                                                  // -inf                                        /// 00ee0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ee4
			 0x80000000,                                                  // -zero                                       /// 00ee8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00eec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ef4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00efc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f00
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f08
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f14
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f24
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f28
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f2c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f30
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f34
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f38
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f3c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f40
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f48
			 0x55555555,                                                  // 4 random values                             /// 00f4c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f50
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f54
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f58
			 0x33333333,                                                  // 4 random values                             /// 00f5c
			 0x7f800000,                                                  // inf                                         /// 00f60
			 0xff800000,                                                  // -inf                                        /// 00f64
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f68
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f70
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f78
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f7c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f80
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f8c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f90
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f98
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f9c
			 0xbf028f5c,                                                  // -0.51                                       /// 00fa0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fa4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fa8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fb0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fb4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fb8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fbc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fc0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fc4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fc8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fcc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fd0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fd4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fe0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ff0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x0c7ffff0                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xddd640d3, /// 0x0
			 0x3d56741a, /// 0x4
			 0x89b42dba, /// 0x8
			 0x4d748682, /// 0xc
			 0x296eb7da, /// 0x10
			 0x283add91, /// 0x14
			 0xe94a371c, /// 0x18
			 0x5480d4dd, /// 0x1c
			 0x52b4dac3, /// 0x20
			 0x2a0d8bb1, /// 0x24
			 0xd8c31d0d, /// 0x28
			 0x19d6a2a8, /// 0x2c
			 0xb017e814, /// 0x30
			 0xbdff361b, /// 0x34
			 0x72080226, /// 0x38
			 0x2ee16f17, /// 0x3c
			 0x65e53069, /// 0x40
			 0x2eecec4c, /// 0x44
			 0xf9546c23, /// 0x48
			 0xd97058a7, /// 0x4c
			 0xb9c52b87, /// 0x50
			 0x7ca197a9, /// 0x54
			 0x05758ff2, /// 0x58
			 0x7ad36015, /// 0x5c
			 0x05adf576, /// 0x60
			 0x855ced38, /// 0x64
			 0xf21a0253, /// 0x68
			 0x52623dbb, /// 0x6c
			 0x44caec27, /// 0x70
			 0x5b769781, /// 0x74
			 0x1d2b12fc, /// 0x78
			 0x372e1630, /// 0x7c
			 0x91860076, /// 0x80
			 0xca294b7f, /// 0x84
			 0x5be7282c, /// 0x88
			 0x41e31ce0, /// 0x8c
			 0x1d583dfb, /// 0x90
			 0xf25680c9, /// 0x94
			 0x7c9f7e88, /// 0x98
			 0x58cb73dc, /// 0x9c
			 0xf0259902, /// 0xa0
			 0x6fdf634a, /// 0xa4
			 0x20612120, /// 0xa8
			 0x45b3ab0b, /// 0xac
			 0xe8cf169e, /// 0xb0
			 0xd836a06d, /// 0xb4
			 0xfffbd163, /// 0xb8
			 0xfe968f0b, /// 0xbc
			 0x50f2657d, /// 0xc0
			 0x9d0b78e2, /// 0xc4
			 0xab881471, /// 0xc8
			 0x0d029eda, /// 0xcc
			 0x80ea8d20, /// 0xd0
			 0xc5ee5162, /// 0xd4
			 0x80886105, /// 0xd8
			 0xc861a5a4, /// 0xdc
			 0x449c5ebb, /// 0xe0
			 0xd0738adb, /// 0xe4
			 0x954e07df, /// 0xe8
			 0xc79abe98, /// 0xec
			 0xaad83c04, /// 0xf0
			 0x0fcf286f, /// 0xf4
			 0xca34f54a, /// 0xf8
			 0xa908cb93, /// 0xfc
			 0x991e8066, /// 0x100
			 0x3cb1ef61, /// 0x104
			 0x6ce4fdf5, /// 0x108
			 0xac1e9cb3, /// 0x10c
			 0x65378293, /// 0x110
			 0xe524fdde, /// 0x114
			 0x211876fe, /// 0x118
			 0x2682850d, /// 0x11c
			 0x7280b606, /// 0x120
			 0xd6045e83, /// 0x124
			 0x36fb19ab, /// 0x128
			 0xfb29f204, /// 0x12c
			 0x86fbf41a, /// 0x130
			 0xfe68933e, /// 0x134
			 0x45c0bd36, /// 0x138
			 0xd9652511, /// 0x13c
			 0xd2307e3c, /// 0x140
			 0x1f31a937, /// 0x144
			 0x9445577c, /// 0x148
			 0x0de791c5, /// 0x14c
			 0xd2de0ede, /// 0x150
			 0xe57e8a51, /// 0x154
			 0xb16a5642, /// 0x158
			 0xf2e48ec2, /// 0x15c
			 0xe3f3b9d8, /// 0x160
			 0xac49faad, /// 0x164
			 0x61f40217, /// 0x168
			 0xc993f581, /// 0x16c
			 0x8de77d9e, /// 0x170
			 0x7a43702a, /// 0x174
			 0xdc8aefac, /// 0x178
			 0x8c2d07cd, /// 0x17c
			 0x71695331, /// 0x180
			 0x07fbbe40, /// 0x184
			 0x7fe3b39c, /// 0x188
			 0x85df0c48, /// 0x18c
			 0x2cbb201f, /// 0x190
			 0x141edc37, /// 0x194
			 0x003fcf9d, /// 0x198
			 0x7663241c, /// 0x19c
			 0xe2b08f70, /// 0x1a0
			 0x1795fb78, /// 0x1a4
			 0xd9633494, /// 0x1a8
			 0x934ee9ad, /// 0x1ac
			 0x604a5fcb, /// 0x1b0
			 0x83108e7b, /// 0x1b4
			 0xb5dfe891, /// 0x1b8
			 0x39f34ab9, /// 0x1bc
			 0x3404954e, /// 0x1c0
			 0x66750675, /// 0x1c4
			 0x3783e5a8, /// 0x1c8
			 0x71fa229c, /// 0x1cc
			 0xde30c34c, /// 0x1d0
			 0x9de08ded, /// 0x1d4
			 0xbe5e7824, /// 0x1d8
			 0xa0288831, /// 0x1dc
			 0xcb02f5f4, /// 0x1e0
			 0x1538b977, /// 0x1e4
			 0xcce5937b, /// 0x1e8
			 0x45f3e516, /// 0x1ec
			 0x4a93546e, /// 0x1f0
			 0xccf65bd7, /// 0x1f4
			 0x77579075, /// 0x1f8
			 0x4f8cac9a, /// 0x1fc
			 0x60719df1, /// 0x200
			 0xefb25286, /// 0x204
			 0xdbe8d43a, /// 0x208
			 0xcd5b07db, /// 0x20c
			 0x5666bb36, /// 0x210
			 0x3aee1fa6, /// 0x214
			 0x331cba3c, /// 0x218
			 0xe53d61c2, /// 0x21c
			 0x95a5a2ca, /// 0x220
			 0xf3703ac2, /// 0x224
			 0xa72a445d, /// 0x228
			 0x2757c5fb, /// 0x22c
			 0xf6da0528, /// 0x230
			 0xc8e4e43b, /// 0x234
			 0xe265045f, /// 0x238
			 0xc1f67250, /// 0x23c
			 0x8320c975, /// 0x240
			 0x62e93822, /// 0x244
			 0xea3501f0, /// 0x248
			 0x79d6f187, /// 0x24c
			 0xb2d82d2e, /// 0x250
			 0x2d492cfe, /// 0x254
			 0x18164131, /// 0x258
			 0xfca48276, /// 0x25c
			 0x2ac40d9d, /// 0x260
			 0x525bdeb8, /// 0x264
			 0x3f99ad13, /// 0x268
			 0xef6ad5fc, /// 0x26c
			 0xd6c54346, /// 0x270
			 0xe546025e, /// 0x274
			 0x1de1d9f9, /// 0x278
			 0xbf3e3903, /// 0x27c
			 0xa993bc75, /// 0x280
			 0x18bb59d3, /// 0x284
			 0x482e7087, /// 0x288
			 0x63ec408c, /// 0x28c
			 0xd7304841, /// 0x290
			 0x41c62abc, /// 0x294
			 0xa33d1197, /// 0x298
			 0xf0bb0e1c, /// 0x29c
			 0xbc4157a0, /// 0x2a0
			 0x88c9f393, /// 0x2a4
			 0xa77fd44c, /// 0x2a8
			 0x3f21b74d, /// 0x2ac
			 0xd3a63a51, /// 0x2b0
			 0xab53edc6, /// 0x2b4
			 0x2c59edc4, /// 0x2b8
			 0x8ba75a9a, /// 0x2bc
			 0xe67b646d, /// 0x2c0
			 0x62551320, /// 0x2c4
			 0x6daf1fc5, /// 0x2c8
			 0x274c3722, /// 0x2cc
			 0x90cdc380, /// 0x2d0
			 0xfda062fa, /// 0x2d4
			 0x3946cd74, /// 0x2d8
			 0x45a394bd, /// 0x2dc
			 0x4e885e17, /// 0x2e0
			 0x7cd788c0, /// 0x2e4
			 0xe4e84056, /// 0x2e8
			 0xc0a3a45e, /// 0x2ec
			 0xf07e4992, /// 0x2f0
			 0xc089d68f, /// 0x2f4
			 0x8f2e3e7b, /// 0x2f8
			 0x6c59336e, /// 0x2fc
			 0x3b26b9a6, /// 0x300
			 0xb78dbc93, /// 0x304
			 0xb78a3253, /// 0x308
			 0x077d6183, /// 0x30c
			 0x0f291c77, /// 0x310
			 0xc18ffd58, /// 0x314
			 0x62b54167, /// 0x318
			 0x6bf131cb, /// 0x31c
			 0xf5065061, /// 0x320
			 0x4cfccb86, /// 0x324
			 0x6ade7037, /// 0x328
			 0x8b1aa55f, /// 0x32c
			 0xd6b33e63, /// 0x330
			 0xd3a812da, /// 0x334
			 0x7eef0091, /// 0x338
			 0xc8e16735, /// 0x33c
			 0xe35f307c, /// 0x340
			 0x691c56e4, /// 0x344
			 0xa4f991bf, /// 0x348
			 0x6049c584, /// 0x34c
			 0xcdd1b1c7, /// 0x350
			 0x1adced60, /// 0x354
			 0x3d117fa9, /// 0x358
			 0xaa63977d, /// 0x35c
			 0xf2623fe3, /// 0x360
			 0xe7406d17, /// 0x364
			 0xb8959acc, /// 0x368
			 0xbac2e9e8, /// 0x36c
			 0xcfd12dae, /// 0x370
			 0xd9f4f4db, /// 0x374
			 0x6551b33d, /// 0x378
			 0x998d9792, /// 0x37c
			 0xb150b478, /// 0x380
			 0x1d6197cf, /// 0x384
			 0xd8de6247, /// 0x388
			 0x1ac4bdbf, /// 0x38c
			 0x8174c447, /// 0x390
			 0xd00e1579, /// 0x394
			 0x011c4afb, /// 0x398
			 0xe6c9ea61, /// 0x39c
			 0xb6c65e51, /// 0x3a0
			 0x180d5063, /// 0x3a4
			 0x59090d7c, /// 0x3a8
			 0x2315bab4, /// 0x3ac
			 0x05099751, /// 0x3b0
			 0xb5a732fa, /// 0x3b4
			 0xeed755ef, /// 0x3b8
			 0x30dac0b5, /// 0x3bc
			 0x0ac9026c, /// 0x3c0
			 0xbc54ea2e, /// 0x3c4
			 0x5d066373, /// 0x3c8
			 0x2869d6cb, /// 0x3cc
			 0xe2684b80, /// 0x3d0
			 0xd9699bc7, /// 0x3d4
			 0x207d60db, /// 0x3d8
			 0x742208fd, /// 0x3dc
			 0xbb6459ae, /// 0x3e0
			 0x6f25c4ef, /// 0x3e4
			 0x8d94bbab, /// 0x3e8
			 0x36740167, /// 0x3ec
			 0x766904d3, /// 0x3f0
			 0x8974ae70, /// 0x3f4
			 0x16a4b84e, /// 0x3f8
			 0xeac4e009, /// 0x3fc
			 0x02d91c12, /// 0x400
			 0xeebabf48, /// 0x404
			 0x1e75a74b, /// 0x408
			 0xd2ebe5e1, /// 0x40c
			 0x7e961c70, /// 0x410
			 0x9e99f1f6, /// 0x414
			 0xf4bf13e3, /// 0x418
			 0xe1bc4b01, /// 0x41c
			 0x6a700a36, /// 0x420
			 0xdd80b230, /// 0x424
			 0x6d64cfd0, /// 0x428
			 0x2e0e3851, /// 0x42c
			 0x00c9fa5a, /// 0x430
			 0xe203859b, /// 0x434
			 0x8636600c, /// 0x438
			 0xb95418c5, /// 0x43c
			 0x2411c71c, /// 0x440
			 0xf4f2612d, /// 0x444
			 0xb15719dc, /// 0x448
			 0x33cd30ab, /// 0x44c
			 0xd55604b2, /// 0x450
			 0x2745e0d2, /// 0x454
			 0x1b9df6ad, /// 0x458
			 0xc17e6016, /// 0x45c
			 0x674e2d3b, /// 0x460
			 0xfae2fb99, /// 0x464
			 0xc49f5e9c, /// 0x468
			 0xce443f9f, /// 0x46c
			 0x8eeae7b8, /// 0x470
			 0xbbb557dd, /// 0x474
			 0xfd0bce27, /// 0x478
			 0xf3a31fad, /// 0x47c
			 0xd6d9b43e, /// 0x480
			 0x09da689f, /// 0x484
			 0x6e33703b, /// 0x488
			 0xd1264833, /// 0x48c
			 0xa81af6c8, /// 0x490
			 0x8f8cb76c, /// 0x494
			 0xa1200ec0, /// 0x498
			 0x0321a748, /// 0x49c
			 0xda723757, /// 0x4a0
			 0xfb65ce2c, /// 0x4a4
			 0x2df0440a, /// 0x4a8
			 0x39d2e93e, /// 0x4ac
			 0xd19cc0c6, /// 0x4b0
			 0x9158787c, /// 0x4b4
			 0xa60e7187, /// 0x4b8
			 0x67672f6e, /// 0x4bc
			 0x90bc4bb8, /// 0x4c0
			 0x0bd5a2de, /// 0x4c4
			 0xb575a3d7, /// 0x4c8
			 0x6b4b9067, /// 0x4cc
			 0xa42a588c, /// 0x4d0
			 0x2daebb99, /// 0x4d4
			 0xbb354cd1, /// 0x4d8
			 0x49035dc1, /// 0x4dc
			 0x07bc16b6, /// 0x4e0
			 0x554b4fda, /// 0x4e4
			 0x59bad03f, /// 0x4e8
			 0x4bdd67bb, /// 0x4ec
			 0x16ba9208, /// 0x4f0
			 0xfc002b22, /// 0x4f4
			 0x4808f42a, /// 0x4f8
			 0xd2da2f1b, /// 0x4fc
			 0x2cca6556, /// 0x500
			 0x65192666, /// 0x504
			 0x04f44eee, /// 0x508
			 0x23e4f0f9, /// 0x50c
			 0xcb08da01, /// 0x510
			 0x3d760207, /// 0x514
			 0x751bf275, /// 0x518
			 0x387edb92, /// 0x51c
			 0xf15064e2, /// 0x520
			 0xb2be448a, /// 0x524
			 0xc1d22e71, /// 0x528
			 0x6536dc1c, /// 0x52c
			 0xc8493039, /// 0x530
			 0x3cfb8310, /// 0x534
			 0xeb6202f1, /// 0x538
			 0x5aef17f8, /// 0x53c
			 0x5b519429, /// 0x540
			 0x04571195, /// 0x544
			 0x86929f26, /// 0x548
			 0xecdd587c, /// 0x54c
			 0x636a56bc, /// 0x550
			 0x3aee866e, /// 0x554
			 0x041c40b1, /// 0x558
			 0x88906285, /// 0x55c
			 0x0d9d6d6b, /// 0x560
			 0xc570c093, /// 0x564
			 0x88df6552, /// 0x568
			 0x969ed357, /// 0x56c
			 0x63b2bd1b, /// 0x570
			 0x99ba73f6, /// 0x574
			 0x02c87e59, /// 0x578
			 0x0c36b183, /// 0x57c
			 0x9c26d7cf, /// 0x580
			 0xbd1361ae, /// 0x584
			 0xf98a5a91, /// 0x588
			 0x0c204fad, /// 0x58c
			 0xe129b861, /// 0x590
			 0xc6ee719d, /// 0x594
			 0x11771927, /// 0x598
			 0x398f41d9, /// 0x59c
			 0x68617149, /// 0x5a0
			 0x67717b2e, /// 0x5a4
			 0x40bc4640, /// 0x5a8
			 0x2c00ac40, /// 0x5ac
			 0xbd9a3747, /// 0x5b0
			 0xefde1e15, /// 0x5b4
			 0x4f7e64ca, /// 0x5b8
			 0x06a298bd, /// 0x5bc
			 0x39eb9575, /// 0x5c0
			 0x683e943a, /// 0x5c4
			 0x8e4e9af0, /// 0x5c8
			 0x1b10f16e, /// 0x5cc
			 0x20e48865, /// 0x5d0
			 0x81491e68, /// 0x5d4
			 0xa72a86d8, /// 0x5d8
			 0xdf927e6b, /// 0x5dc
			 0x3edea095, /// 0x5e0
			 0xc928cfab, /// 0x5e4
			 0xd1a9911c, /// 0x5e8
			 0x4d59b77d, /// 0x5ec
			 0x603b4f8b, /// 0x5f0
			 0x7cd65e2c, /// 0x5f4
			 0x96b37ebf, /// 0x5f8
			 0xda2f8f02, /// 0x5fc
			 0x7181ede9, /// 0x600
			 0x7d2adc77, /// 0x604
			 0x60ec53ac, /// 0x608
			 0xa89785c1, /// 0x60c
			 0x6150bcb7, /// 0x610
			 0x20a58d82, /// 0x614
			 0x991a2005, /// 0x618
			 0xf83f6042, /// 0x61c
			 0xcae0be6f, /// 0x620
			 0x870bc957, /// 0x624
			 0xca887ee2, /// 0x628
			 0x535fc1a6, /// 0x62c
			 0x8770929e, /// 0x630
			 0xeff46189, /// 0x634
			 0x43de3191, /// 0x638
			 0xdeb2db40, /// 0x63c
			 0xa8434d4f, /// 0x640
			 0xb01d8e92, /// 0x644
			 0x8459b624, /// 0x648
			 0xfceae17e, /// 0x64c
			 0x177f89e4, /// 0x650
			 0xf178274e, /// 0x654
			 0x3ab0ced0, /// 0x658
			 0xc9795a8b, /// 0x65c
			 0xcd7a28a7, /// 0x660
			 0x914a29ba, /// 0x664
			 0x52aa560e, /// 0x668
			 0x25a8e2b3, /// 0x66c
			 0xdc0302aa, /// 0x670
			 0x730b71a6, /// 0x674
			 0xf87db473, /// 0x678
			 0x93318c2a, /// 0x67c
			 0x31422d3f, /// 0x680
			 0xe15da0f6, /// 0x684
			 0xd50260b7, /// 0x688
			 0xbec14962, /// 0x68c
			 0xb8fe7c83, /// 0x690
			 0x3e0f8946, /// 0x694
			 0x5167a766, /// 0x698
			 0xb4024725, /// 0x69c
			 0xe589938e, /// 0x6a0
			 0xdd6ee107, /// 0x6a4
			 0xfab869d7, /// 0x6a8
			 0x8047c240, /// 0x6ac
			 0x2c2b2846, /// 0x6b0
			 0x7aaa02bc, /// 0x6b4
			 0x765d0d6b, /// 0x6b8
			 0xb2f16981, /// 0x6bc
			 0x748ba056, /// 0x6c0
			 0x4d290193, /// 0x6c4
			 0x1c56bb61, /// 0x6c8
			 0xc612d90b, /// 0x6cc
			 0x0878c834, /// 0x6d0
			 0x5a623ec0, /// 0x6d4
			 0xb44d31f9, /// 0x6d8
			 0xef4d0924, /// 0x6dc
			 0x02686d1a, /// 0x6e0
			 0x6885f199, /// 0x6e4
			 0xb8faa098, /// 0x6e8
			 0x7164c6e9, /// 0x6ec
			 0xf523fd9c, /// 0x6f0
			 0x19a3362f, /// 0x6f4
			 0xbc73d338, /// 0x6f8
			 0xb86cd27c, /// 0x6fc
			 0x879fb367, /// 0x700
			 0x80f14288, /// 0x704
			 0x98f8007a, /// 0x708
			 0xab5f5216, /// 0x70c
			 0x10dbcce8, /// 0x710
			 0x97238702, /// 0x714
			 0x34bc97b6, /// 0x718
			 0x1de4077a, /// 0x71c
			 0xa17aa8ab, /// 0x720
			 0xafeb7f11, /// 0x724
			 0xd7ee895b, /// 0x728
			 0x0d18ca7e, /// 0x72c
			 0x7b57210f, /// 0x730
			 0x0d1fd535, /// 0x734
			 0x6d09928b, /// 0x738
			 0xce8b7e7e, /// 0x73c
			 0x9541cb93, /// 0x740
			 0x070115c6, /// 0x744
			 0x271d9be3, /// 0x748
			 0xdcf1e557, /// 0x74c
			 0xb9481dc6, /// 0x750
			 0x9fa726fb, /// 0x754
			 0xd0222d64, /// 0x758
			 0x0b80c9bc, /// 0x75c
			 0x42773e0e, /// 0x760
			 0x4a0f09e3, /// 0x764
			 0x29a260a4, /// 0x768
			 0x5bcda0b1, /// 0x76c
			 0x62f572cd, /// 0x770
			 0x892cbf86, /// 0x774
			 0xfc66b39e, /// 0x778
			 0x7d743ad9, /// 0x77c
			 0xc3ca5dec, /// 0x780
			 0x6d24651f, /// 0x784
			 0x19d9692f, /// 0x788
			 0xfce140ad, /// 0x78c
			 0xc520c1f6, /// 0x790
			 0xedbbc3ca, /// 0x794
			 0x48ebe149, /// 0x798
			 0x167c96f5, /// 0x79c
			 0x79254281, /// 0x7a0
			 0x0142324d, /// 0x7a4
			 0x2914e4da, /// 0x7a8
			 0xc5eda6bd, /// 0x7ac
			 0x78d51de2, /// 0x7b0
			 0x1eb2c5bd, /// 0x7b4
			 0x70099a26, /// 0x7b8
			 0x2384319e, /// 0x7bc
			 0x84a2dbb9, /// 0x7c0
			 0xa64da373, /// 0x7c4
			 0x684005eb, /// 0x7c8
			 0x596c51c7, /// 0x7cc
			 0x14d80b20, /// 0x7d0
			 0xcfe853c0, /// 0x7d4
			 0xb159694e, /// 0x7d8
			 0xb125e0cd, /// 0x7dc
			 0x026173e4, /// 0x7e0
			 0xe869a3a8, /// 0x7e4
			 0xe1694124, /// 0x7e8
			 0x9217c3b1, /// 0x7ec
			 0x2b57c10d, /// 0x7f0
			 0x57fbeced, /// 0x7f4
			 0x0b4740d9, /// 0x7f8
			 0x2063b553, /// 0x7fc
			 0x8a57c1f7, /// 0x800
			 0x976e1a99, /// 0x804
			 0x0e1c105d, /// 0x808
			 0x5560b912, /// 0x80c
			 0xc1b2b8b3, /// 0x810
			 0x5a7c2d33, /// 0x814
			 0xf3d290b6, /// 0x818
			 0xebe2fb4a, /// 0x81c
			 0x475eb8cb, /// 0x820
			 0xe90e2990, /// 0x824
			 0xf9f7c3a7, /// 0x828
			 0xe423bbef, /// 0x82c
			 0x5d25efce, /// 0x830
			 0x4a7e5fd2, /// 0x834
			 0x8e9aeae4, /// 0x838
			 0x4344445c, /// 0x83c
			 0x20753b2a, /// 0x840
			 0x0d3239c8, /// 0x844
			 0x473ada37, /// 0x848
			 0x927fe289, /// 0x84c
			 0x2e3d7edf, /// 0x850
			 0xb5f03895, /// 0x854
			 0x077bfdc4, /// 0x858
			 0xe3225460, /// 0x85c
			 0x6bcf4717, /// 0x860
			 0xee3c2b84, /// 0x864
			 0x4b532f45, /// 0x868
			 0xffd37dcf, /// 0x86c
			 0x616d8665, /// 0x870
			 0x863a9fa7, /// 0x874
			 0x3cd609a2, /// 0x878
			 0xfeb4b4ae, /// 0x87c
			 0x00ac8c63, /// 0x880
			 0xfb5d548d, /// 0x884
			 0xb8518bd1, /// 0x888
			 0x71b745b7, /// 0x88c
			 0x6937084d, /// 0x890
			 0xd85acb06, /// 0x894
			 0xf26ff507, /// 0x898
			 0x72842fb1, /// 0x89c
			 0x867c7ae0, /// 0x8a0
			 0xf4463185, /// 0x8a4
			 0x08832292, /// 0x8a8
			 0x04dfde57, /// 0x8ac
			 0x97f5af45, /// 0x8b0
			 0x46bb98fa, /// 0x8b4
			 0xf3b1a893, /// 0x8b8
			 0x965064cd, /// 0x8bc
			 0x923b8bf0, /// 0x8c0
			 0xbde5d0ea, /// 0x8c4
			 0x98dc577f, /// 0x8c8
			 0x6fb579a5, /// 0x8cc
			 0x9d2254dd, /// 0x8d0
			 0xa7b4865c, /// 0x8d4
			 0x8199635c, /// 0x8d8
			 0x78507da4, /// 0x8dc
			 0xa2ba3ce2, /// 0x8e0
			 0x935d42f0, /// 0x8e4
			 0xe07ea7f7, /// 0x8e8
			 0x8b169368, /// 0x8ec
			 0x318460e4, /// 0x8f0
			 0x95382361, /// 0x8f4
			 0x071b516a, /// 0x8f8
			 0x39238eb8, /// 0x8fc
			 0x7e94a1d6, /// 0x900
			 0x08e67ad9, /// 0x904
			 0xcd5bc29a, /// 0x908
			 0x21a54b9b, /// 0x90c
			 0x1ad4bd18, /// 0x910
			 0x0dd376bb, /// 0x914
			 0x9246d176, /// 0x918
			 0xed79c74c, /// 0x91c
			 0x2940642d, /// 0x920
			 0x352bbb64, /// 0x924
			 0xb84b0546, /// 0x928
			 0xe4dd3f87, /// 0x92c
			 0x616744f8, /// 0x930
			 0x52f6764c, /// 0x934
			 0x6c112d94, /// 0x938
			 0x898173a5, /// 0x93c
			 0x48a408cc, /// 0x940
			 0x0a8146dd, /// 0x944
			 0x1bed7fe2, /// 0x948
			 0x48070ef9, /// 0x94c
			 0x67ff2a7b, /// 0x950
			 0xdc89d175, /// 0x954
			 0xc2bedf38, /// 0x958
			 0x9803bca1, /// 0x95c
			 0x828d1316, /// 0x960
			 0xa088eb99, /// 0x964
			 0x988d5419, /// 0x968
			 0xadd9278d, /// 0x96c
			 0x82bfb3c5, /// 0x970
			 0x64391ba8, /// 0x974
			 0x84111b77, /// 0x978
			 0x3d272cc6, /// 0x97c
			 0xf78b4a49, /// 0x980
			 0xe4781834, /// 0x984
			 0xf16ca5c6, /// 0x988
			 0xe334a981, /// 0x98c
			 0x061d41b7, /// 0x990
			 0x2ce99bda, /// 0x994
			 0x171b5bea, /// 0x998
			 0x3fc57bf8, /// 0x99c
			 0x723bbfaa, /// 0x9a0
			 0x34408d46, /// 0x9a4
			 0x7e515e2c, /// 0x9a8
			 0xb9330959, /// 0x9ac
			 0xcd2ea476, /// 0x9b0
			 0xccf72a43, /// 0x9b4
			 0xf3f43c28, /// 0x9b8
			 0x07fd95ca, /// 0x9bc
			 0x55150d1c, /// 0x9c0
			 0x1c850751, /// 0x9c4
			 0x894e4c87, /// 0x9c8
			 0x7f0c48a6, /// 0x9cc
			 0xd7d29b2d, /// 0x9d0
			 0x5cb28d46, /// 0x9d4
			 0x13a990d1, /// 0x9d8
			 0x9023eeb5, /// 0x9dc
			 0xcb3e9721, /// 0x9e0
			 0x260bd9bb, /// 0x9e4
			 0x460a3324, /// 0x9e8
			 0xbc7e73e1, /// 0x9ec
			 0x3220c057, /// 0x9f0
			 0x75b8a57a, /// 0x9f4
			 0x47014062, /// 0x9f8
			 0xe818aefe, /// 0x9fc
			 0x76f35462, /// 0xa00
			 0x73cbee65, /// 0xa04
			 0xd77d5a4a, /// 0xa08
			 0x47effd89, /// 0xa0c
			 0xa7174d3c, /// 0xa10
			 0xb4905e06, /// 0xa14
			 0x528b8b8d, /// 0xa18
			 0x30d8cd33, /// 0xa1c
			 0xe8778645, /// 0xa20
			 0xfbea09ee, /// 0xa24
			 0x130a2472, /// 0xa28
			 0x5a0886dc, /// 0xa2c
			 0x2b34d62e, /// 0xa30
			 0xdc6a4272, /// 0xa34
			 0x9717d411, /// 0xa38
			 0x56bc669d, /// 0xa3c
			 0x6f32e217, /// 0xa40
			 0x691e9a94, /// 0xa44
			 0x67e0cfa1, /// 0xa48
			 0xb0b50354, /// 0xa4c
			 0x328e18ba, /// 0xa50
			 0x2ff41c0c, /// 0xa54
			 0xd4a3cb2c, /// 0xa58
			 0xff84c2de, /// 0xa5c
			 0x7eda7050, /// 0xa60
			 0x28e3565d, /// 0xa64
			 0x02a17b9b, /// 0xa68
			 0x3290e21c, /// 0xa6c
			 0x08b9aaa0, /// 0xa70
			 0xf2412ab1, /// 0xa74
			 0x50d32d72, /// 0xa78
			 0x1503eca0, /// 0xa7c
			 0x79619631, /// 0xa80
			 0xc3a56fff, /// 0xa84
			 0x0de9418d, /// 0xa88
			 0x4d10edc2, /// 0xa8c
			 0xafd5cb28, /// 0xa90
			 0x6d2dcca8, /// 0xa94
			 0x0051cb1e, /// 0xa98
			 0x220f424f, /// 0xa9c
			 0xc60fbbcc, /// 0xaa0
			 0x777d95d3, /// 0xaa4
			 0x7bfd1fa8, /// 0xaa8
			 0x1d7c42c9, /// 0xaac
			 0x5cd01193, /// 0xab0
			 0xd0b9f08c, /// 0xab4
			 0xc243b0c0, /// 0xab8
			 0xd5854397, /// 0xabc
			 0x9c71033c, /// 0xac0
			 0x8e043deb, /// 0xac4
			 0xf834c5e8, /// 0xac8
			 0xf5fa22a9, /// 0xacc
			 0xc13ddd3c, /// 0xad0
			 0xd1e62fb3, /// 0xad4
			 0xd73e5ff2, /// 0xad8
			 0xea117414, /// 0xadc
			 0x2965a2d7, /// 0xae0
			 0x7916bbc0, /// 0xae4
			 0xf8e209c9, /// 0xae8
			 0xf0e62974, /// 0xaec
			 0x6c70c08f, /// 0xaf0
			 0xf642c677, /// 0xaf4
			 0xe356a37c, /// 0xaf8
			 0x04915f30, /// 0xafc
			 0xf2b734b8, /// 0xb00
			 0xa00b6ea8, /// 0xb04
			 0x4245130c, /// 0xb08
			 0x1e528f87, /// 0xb0c
			 0xd308ff44, /// 0xb10
			 0xc39f3efc, /// 0xb14
			 0xd0800aaf, /// 0xb18
			 0xf453dc67, /// 0xb1c
			 0xbf1a987d, /// 0xb20
			 0xc2aff0dc, /// 0xb24
			 0x9f8bf55e, /// 0xb28
			 0x98bf135a, /// 0xb2c
			 0x488d29cd, /// 0xb30
			 0x4f582dc8, /// 0xb34
			 0x91ff0d8f, /// 0xb38
			 0x7b419acf, /// 0xb3c
			 0x5149e0cf, /// 0xb40
			 0x3ece7e5d, /// 0xb44
			 0xe1138b37, /// 0xb48
			 0x18aa93cb, /// 0xb4c
			 0x83810370, /// 0xb50
			 0x8dbfb6f1, /// 0xb54
			 0x723384db, /// 0xb58
			 0x75e076c5, /// 0xb5c
			 0xe116c276, /// 0xb60
			 0xc44e3468, /// 0xb64
			 0xe33b73f5, /// 0xb68
			 0xc541676c, /// 0xb6c
			 0x061646f7, /// 0xb70
			 0x8af58845, /// 0xb74
			 0x5a7066a7, /// 0xb78
			 0x0c87657d, /// 0xb7c
			 0x0c7ecbea, /// 0xb80
			 0x6a9270da, /// 0xb84
			 0xec19dd07, /// 0xb88
			 0x479ab7b1, /// 0xb8c
			 0x1982205c, /// 0xb90
			 0x32be9c99, /// 0xb94
			 0xfed17b81, /// 0xb98
			 0x5425bbf6, /// 0xb9c
			 0x1418ea16, /// 0xba0
			 0xc763fb0d, /// 0xba4
			 0xb4a1821f, /// 0xba8
			 0x203c999f, /// 0xbac
			 0xcc36dcf7, /// 0xbb0
			 0x6ca9f2b7, /// 0xbb4
			 0x0df8d3e9, /// 0xbb8
			 0x793d29f6, /// 0xbbc
			 0x85e16741, /// 0xbc0
			 0x4dfdf11e, /// 0xbc4
			 0x1cf60184, /// 0xbc8
			 0x1a38c468, /// 0xbcc
			 0x500295e1, /// 0xbd0
			 0x8cd8dd71, /// 0xbd4
			 0x9d29ca80, /// 0xbd8
			 0xac46bcfc, /// 0xbdc
			 0x932daa21, /// 0xbe0
			 0xd1af5908, /// 0xbe4
			 0x4b6bc85c, /// 0xbe8
			 0xc208e570, /// 0xbec
			 0x34286df8, /// 0xbf0
			 0xab306f04, /// 0xbf4
			 0xe7910921, /// 0xbf8
			 0x6aeabf3a, /// 0xbfc
			 0x3ec36710, /// 0xc00
			 0x4cf63f20, /// 0xc04
			 0x96cdeef1, /// 0xc08
			 0x4816bfac, /// 0xc0c
			 0x9b1e7d6d, /// 0xc10
			 0xa2e59c95, /// 0xc14
			 0x28f4180e, /// 0xc18
			 0xd9ab1316, /// 0xc1c
			 0x8247ffb9, /// 0xc20
			 0x7db442bc, /// 0xc24
			 0x537305f3, /// 0xc28
			 0xb795e046, /// 0xc2c
			 0x7dd6b44d, /// 0xc30
			 0xa121a3d6, /// 0xc34
			 0xe5de3349, /// 0xc38
			 0x9f54dd01, /// 0xc3c
			 0x4c1ec925, /// 0xc40
			 0x43eab69f, /// 0xc44
			 0xb1152376, /// 0xc48
			 0x3accd3c2, /// 0xc4c
			 0xd7b112d7, /// 0xc50
			 0xd363e6e7, /// 0xc54
			 0xd0ee696c, /// 0xc58
			 0xb6377ff8, /// 0xc5c
			 0x8da990e7, /// 0xc60
			 0x195b0c52, /// 0xc64
			 0x710ca8bb, /// 0xc68
			 0x2aeda1a8, /// 0xc6c
			 0x96232176, /// 0xc70
			 0xb987c6f2, /// 0xc74
			 0x7112b704, /// 0xc78
			 0xdc2f3f4c, /// 0xc7c
			 0xd9126642, /// 0xc80
			 0x9ed18a19, /// 0xc84
			 0x7afd8b30, /// 0xc88
			 0x53b003a9, /// 0xc8c
			 0x0a4b731f, /// 0xc90
			 0xc140d207, /// 0xc94
			 0x4cfad8bd, /// 0xc98
			 0xf2fdca6a, /// 0xc9c
			 0xf3e29ae1, /// 0xca0
			 0xc741ff1c, /// 0xca4
			 0xf4cc0724, /// 0xca8
			 0x9c022609, /// 0xcac
			 0x264951a9, /// 0xcb0
			 0x49d5071d, /// 0xcb4
			 0xff94b548, /// 0xcb8
			 0x345818a2, /// 0xcbc
			 0xc2d0ddb0, /// 0xcc0
			 0xba0cae1f, /// 0xcc4
			 0x7a0edd1b, /// 0xcc8
			 0xd05cab5a, /// 0xccc
			 0x3d449704, /// 0xcd0
			 0xb3d7e478, /// 0xcd4
			 0x006e6e63, /// 0xcd8
			 0x89cf36ac, /// 0xcdc
			 0x53bcda98, /// 0xce0
			 0x69214842, /// 0xce4
			 0xb4879c31, /// 0xce8
			 0xa1f0740a, /// 0xcec
			 0x4e81ff98, /// 0xcf0
			 0x98e9db32, /// 0xcf4
			 0x23f9b3dc, /// 0xcf8
			 0x74bc0971, /// 0xcfc
			 0x140a6f56, /// 0xd00
			 0x125cebba, /// 0xd04
			 0xcc7d8673, /// 0xd08
			 0x5d9249cf, /// 0xd0c
			 0x33272a7a, /// 0xd10
			 0x64e34967, /// 0xd14
			 0x4c23603d, /// 0xd18
			 0xf64ceea2, /// 0xd1c
			 0xbf7687a3, /// 0xd20
			 0xebaea0b5, /// 0xd24
			 0xa723c272, /// 0xd28
			 0xde4c911d, /// 0xd2c
			 0x86acfa57, /// 0xd30
			 0x4908404d, /// 0xd34
			 0x73eceb9f, /// 0xd38
			 0xf9b0b31c, /// 0xd3c
			 0x946d0b5a, /// 0xd40
			 0x52be6e2c, /// 0xd44
			 0xe19a23f3, /// 0xd48
			 0x2036b307, /// 0xd4c
			 0x8a162582, /// 0xd50
			 0x78f54e86, /// 0xd54
			 0x5ae22006, /// 0xd58
			 0xf65b4cc2, /// 0xd5c
			 0x542a8d0a, /// 0xd60
			 0x7cc21b25, /// 0xd64
			 0xd23733a2, /// 0xd68
			 0x976630a2, /// 0xd6c
			 0xfe4523b1, /// 0xd70
			 0x6f1d7eaa, /// 0xd74
			 0x020c7743, /// 0xd78
			 0x0116e740, /// 0xd7c
			 0xa17443fd, /// 0xd80
			 0x26b88a85, /// 0xd84
			 0x05e4fb62, /// 0xd88
			 0x4721d754, /// 0xd8c
			 0xd4b05cdf, /// 0xd90
			 0x224822de, /// 0xd94
			 0x31805e61, /// 0xd98
			 0x15290fdd, /// 0xd9c
			 0x2ba36bc1, /// 0xda0
			 0xc50698e8, /// 0xda4
			 0xb56a0e91, /// 0xda8
			 0xb508bc89, /// 0xdac
			 0x7c4233c9, /// 0xdb0
			 0x8692f691, /// 0xdb4
			 0xb4e5a3e0, /// 0xdb8
			 0x7400dee7, /// 0xdbc
			 0x0db048d0, /// 0xdc0
			 0x001f08d9, /// 0xdc4
			 0xa4563406, /// 0xdc8
			 0xc486d11d, /// 0xdcc
			 0x83ae71c7, /// 0xdd0
			 0xcf16c955, /// 0xdd4
			 0x15ed53f8, /// 0xdd8
			 0xbd519909, /// 0xddc
			 0x40a6fb35, /// 0xde0
			 0x20e40775, /// 0xde4
			 0xa5954e32, /// 0xde8
			 0x517bf47c, /// 0xdec
			 0x9c9aa36b, /// 0xdf0
			 0x5052151e, /// 0xdf4
			 0x3d0a3170, /// 0xdf8
			 0xb7e8eefd, /// 0xdfc
			 0x44e08aa9, /// 0xe00
			 0xb6ef4f78, /// 0xe04
			 0xd8f5a0a0, /// 0xe08
			 0x8aff7eca, /// 0xe0c
			 0x5e340319, /// 0xe10
			 0x815f179f, /// 0xe14
			 0x1fd0bda2, /// 0xe18
			 0xa68c6c1b, /// 0xe1c
			 0x9ebb4b7f, /// 0xe20
			 0xb7bf65ea, /// 0xe24
			 0x3e5fac3f, /// 0xe28
			 0x0dd20e50, /// 0xe2c
			 0x90248b6c, /// 0xe30
			 0xc9b3d73c, /// 0xe34
			 0x5042b33e, /// 0xe38
			 0x57a2de10, /// 0xe3c
			 0x7226cb5c, /// 0xe40
			 0x05142a2e, /// 0xe44
			 0xe087ee0a, /// 0xe48
			 0x020d9258, /// 0xe4c
			 0xb4ff4b23, /// 0xe50
			 0x5ec8576f, /// 0xe54
			 0x867ed16b, /// 0xe58
			 0x91d88fcb, /// 0xe5c
			 0x411523eb, /// 0xe60
			 0xc7b48c83, /// 0xe64
			 0xd0e81649, /// 0xe68
			 0x338ab8a9, /// 0xe6c
			 0x0810ab0f, /// 0xe70
			 0x4c239a49, /// 0xe74
			 0x0f6a7a14, /// 0xe78
			 0xbf110759, /// 0xe7c
			 0xcd46e8ec, /// 0xe80
			 0x1761c1e3, /// 0xe84
			 0xe8eae563, /// 0xe88
			 0x788731a6, /// 0xe8c
			 0x9e1d3c56, /// 0xe90
			 0xd5cd3ef1, /// 0xe94
			 0xa6bf4052, /// 0xe98
			 0xe1f437c8, /// 0xe9c
			 0x10b412a5, /// 0xea0
			 0xa487e50b, /// 0xea4
			 0x6dd72554, /// 0xea8
			 0x7044490a, /// 0xeac
			 0xb5a49fdb, /// 0xeb0
			 0x945abac0, /// 0xeb4
			 0xa6ad22a5, /// 0xeb8
			 0xa68c38e3, /// 0xebc
			 0xac9b7e4e, /// 0xec0
			 0xbd245705, /// 0xec4
			 0x30ac6a55, /// 0xec8
			 0xa6b8e263, /// 0xecc
			 0xf5ef469b, /// 0xed0
			 0x5010a35f, /// 0xed4
			 0xc23d0ee3, /// 0xed8
			 0x62eb0237, /// 0xedc
			 0x3bc9a2ef, /// 0xee0
			 0xe562b14b, /// 0xee4
			 0x0ef219d8, /// 0xee8
			 0x676782e5, /// 0xeec
			 0xc57573e1, /// 0xef0
			 0x2afda8b1, /// 0xef4
			 0x920d9231, /// 0xef8
			 0x3c6b11d3, /// 0xefc
			 0x319c49d5, /// 0xf00
			 0xdf919c9a, /// 0xf04
			 0x9d6ce97a, /// 0xf08
			 0x7346c94f, /// 0xf0c
			 0xdcbc3330, /// 0xf10
			 0x5341644f, /// 0xf14
			 0x86c3ad94, /// 0xf18
			 0x39d36bff, /// 0xf1c
			 0xb75a1df2, /// 0xf20
			 0xe1401258, /// 0xf24
			 0xadfb8c19, /// 0xf28
			 0x620f5fa8, /// 0xf2c
			 0xba6fd381, /// 0xf30
			 0x374ffba8, /// 0xf34
			 0xe3666f47, /// 0xf38
			 0x5178e512, /// 0xf3c
			 0xb4198124, /// 0xf40
			 0xa753ba39, /// 0xf44
			 0x84e389d4, /// 0xf48
			 0xfb1ce7c3, /// 0xf4c
			 0x69f0d84d, /// 0xf50
			 0x0ed429e0, /// 0xf54
			 0x08089f4a, /// 0xf58
			 0x4fbfd470, /// 0xf5c
			 0xc9a5b7f8, /// 0xf60
			 0x926fdd18, /// 0xf64
			 0x61955d58, /// 0xf68
			 0x8e814c66, /// 0xf6c
			 0xe6e44152, /// 0xf70
			 0xc05aeb7b, /// 0xf74
			 0x0553619b, /// 0xf78
			 0xa1cc60a3, /// 0xf7c
			 0x0ca9b41a, /// 0xf80
			 0x1b1e4d18, /// 0xf84
			 0x25cb1d10, /// 0xf88
			 0xf0e73e47, /// 0xf8c
			 0xcff08b87, /// 0xf90
			 0x5cab760d, /// 0xf94
			 0x2aa6e5d9, /// 0xf98
			 0xaa11efd7, /// 0xf9c
			 0x0adf4928, /// 0xfa0
			 0xfc384181, /// 0xfa4
			 0xe7226eb0, /// 0xfa8
			 0x1e869a6d, /// 0xfac
			 0xe181cb48, /// 0xfb0
			 0xd00854ef, /// 0xfb4
			 0xcd837e09, /// 0xfb8
			 0x024c6f44, /// 0xfbc
			 0xedf5c4e7, /// 0xfc0
			 0xb0acedae, /// 0xfc4
			 0xf36e21f5, /// 0xfc8
			 0xcd752220, /// 0xfcc
			 0xb258aacf, /// 0xfd0
			 0x89ce7f6d, /// 0xfd4
			 0x7265ad5b, /// 0xfd8
			 0x228bdedb, /// 0xfdc
			 0x98bd1306, /// 0xfe0
			 0x4846fb56, /// 0xfe4
			 0x16f8ac6c, /// 0xfe8
			 0x0eabd6cb, /// 0xfec
			 0xee20ee2b, /// 0xff0
			 0x14246e94, /// 0xff4
			 0xa5325bdb, /// 0xff8
			 0x4da6eb35 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00000
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00004
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00008
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0000c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00010
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00014
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00018
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0001c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00020
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00024
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0002c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00030
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00034
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0003c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00040
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00044
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00048
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0004c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00050
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00054
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00058
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0005c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00060
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00064
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00068
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0006c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00070
			 0xbf028f5c,                                                  // -0.51                                       /// 00074
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00078
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0007c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00080
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00084
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00088
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0008c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00090
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00094
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00098
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0009c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000a0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000a4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000a8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000ac
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000b0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000b4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000bc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000c0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000c4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000cc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000d0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000d8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000e4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000fc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00100
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00108
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0010c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00110
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00114
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00118
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0011c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00120
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00124
			 0x00000000,                                                  // zero                                        /// 00128
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00130
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00134
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00138
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0013c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00140
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00144
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x3f028f5c,                                                  // 0.51                                        /// 0014c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00150
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00154
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00158
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0015c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00160
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00164
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0016c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00174
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00178
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0017c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00184
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00188
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0018c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00190
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00194
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00198
			 0x0e000003,                                                  // Trailing 1s:                                /// 0019c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001a0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001a4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001a8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001ac
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001b0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001b4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001b8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001c0
			 0x7f800000,                                                  // inf                                         /// 001c4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001c8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001d0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001d4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001dc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001e4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001e8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001f0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001f4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001f8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001fc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00200
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00208
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0020c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00210
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00214
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00220
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00224
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00228
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0022c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00230
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00234
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00238
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00240
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00244
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x0c780000,                                                  // Leading 1s:                                 /// 0024c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00250
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00254
			 0x0e000001,                                                  // Trailing 1s:                                /// 00258
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0025c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00260
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00264
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0026c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00270
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00278
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0027c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00280
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00284
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0028c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00290
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x0c400000,                                                  // Leading 1s:                                 /// 002a0
			 0x0c700000,                                                  // Leading 1s:                                 /// 002a4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002a8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002ac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002b0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002b4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002b8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002bc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002c0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002c4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002c8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002cc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002d0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002d4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002dc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002e0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002e4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002e8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002ec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002f4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002fc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00300
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00304
			 0x3f028f5c,                                                  // 0.51                                        /// 00308
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0030c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00314
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00318
			 0x4b000000,                                                  // 8388608.0                                   /// 0031c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00320
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00328
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0032c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00334
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00338
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00340
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00344
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00348
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00350
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00354
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00358
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0035c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00360
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00364
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00368
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0036c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0xbf028f5c,                                                  // -0.51                                       /// 00374
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00378
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0037c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00380
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0038c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00390
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00394
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00398
			 0x80000000,                                                  // -zero                                       /// 0039c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003a4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003ac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003b0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003b4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003b8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003bc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003c4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003cc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003d0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003e0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003e4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003ec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003f0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003f4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003fc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00404
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00408
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0040c
			 0x55555555,                                                  // 4 random values                             /// 00410
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00414
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00418
			 0x0c780000,                                                  // Leading 1s:                                 /// 0041c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00420
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00424
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00428
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0042c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00430
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00434
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00438
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00440
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00444
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00448
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0044c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0045c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00460
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00464
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00468
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00470
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00474
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00478
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0047c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00480
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00484
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00488
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0048c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00490
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00494
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00498
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0049c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004a0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004a4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004a8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004ac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004b4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004bc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004c4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004d4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004d8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004e4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x0c700000,                                                  // Leading 1s:                                 /// 004f0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00500
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00504
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00508
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00510
			 0x7f800000,                                                  // inf                                         /// 00514
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00518
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00520
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00524
			 0x0e000001,                                                  // Trailing 1s:                                /// 00528
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0052c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00530
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00534
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00538
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0053c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00544
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00550
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00554
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00558
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00564
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0056c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00570
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00574
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0057c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00580
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00584
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x4b000000,                                                  // 8388608.0                                   /// 0058c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00590
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00598
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0059c
			 0x0c700000,                                                  // Leading 1s:                                 /// 005a0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005a4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005ac
			 0x0c700000,                                                  // Leading 1s:                                 /// 005b0
			 0x0c700000,                                                  // Leading 1s:                                 /// 005b4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005bc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005c0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005c4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005d4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005dc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005ec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005f0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005f4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00604
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00608
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0060c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00614
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00618
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0061c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00620
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00624
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00628
			 0xffc00001,                                                  // -signaling NaN                              /// 0062c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00634
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00638
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0063c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00648
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0064c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00650
			 0x3f028f5c,                                                  // 0.51                                        /// 00654
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00658
			 0x33333333,                                                  // 4 random values                             /// 0065c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00660
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00664
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00668
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0066c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00670
			 0xff800000,                                                  // -inf                                        /// 00674
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00678
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0067c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00680
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00684
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00688
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00690
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00694
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00698
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0xcb000000,                                                  // -8388608.0                                  /// 006a0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006a4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006ac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006b8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006bc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006c4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006c8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x00011111,                                                  // 9.7958E-41                                  /// 006d0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 006d8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006dc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006e0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006e8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006f0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006f4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006f8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00700
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00704
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00708
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0070c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00710
			 0x55555555,                                                  // 4 random values                             /// 00714
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00718
			 0x33333333,                                                  // 4 random values                             /// 0071c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00720
			 0x33333333,                                                  // 4 random values                             /// 00724
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0072c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00734
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00738
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0073c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00748
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0074c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00750
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00758
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0075c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00768
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0076c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00774
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0077c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00780
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0078c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00790
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00794
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00798
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007a0
			 0x00000000,                                                  // zero                                        /// 007a4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007a8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007ac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007b0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007b4
			 0x0c400000,                                                  // Leading 1s:                                 /// 007b8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007cc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007d0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007d4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007dc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007e8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007ec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007f8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007fc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00804
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00808
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00810
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00814
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00818
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00820
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00824
			 0xcb000000,                                                  // -8388608.0                                  /// 00828
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0082c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00830
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00834
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0083c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00844
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00848
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0084c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00850
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00854
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00858
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0085c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00860
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00864
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0086c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00878
			 0xcb000000,                                                  // -8388608.0                                  /// 0087c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00880
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00884
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00888
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0088c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00890
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00898
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0089c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008a4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008b4
			 0x0e000007,                                                  // Trailing 1s:                                /// 008b8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008bc
			 0x7fc00001,                                                  // signaling NaN                               /// 008c0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008c4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008c8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008cc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008d0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008d4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008d8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008e0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008e4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008e8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008ec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008f0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008f4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008f8
			 0x80011111,                                                  // -9.7958E-41                                 /// 008fc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00900
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x0c400000,                                                  // Leading 1s:                                 /// 0090c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00910
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00914
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00918
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00924
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00928
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00930
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00934
			 0x0e000003,                                                  // Trailing 1s:                                /// 00938
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00940
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00944
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00948
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0094c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00950
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00964
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0096c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00970
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00974
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00978
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0097c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00980
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00984
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00988
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0098c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00990
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00998
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0099c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009a0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009a4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x0c780000,                                                  // Leading 1s:                                 /// 009ac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009b0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009b4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009bc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009c0
			 0x80000000,                                                  // -zero                                       /// 009c4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009c8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009cc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009d0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 009d4
			 0x3f028f5c,                                                  // 0.51                                        /// 009d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009e0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009e4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009ec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009f0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009fc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a00
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a04
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a08
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a0c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a10
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a14
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a18
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a20
			 0x00000000,                                                  // zero                                        /// 00a24
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a34
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a3c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a40
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a48
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a4c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a58
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a5c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a60
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a6c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a74
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a78
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a7c
			 0x80000000,                                                  // -zero                                       /// 00a80
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a84
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a90
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a94
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a98
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00aa0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00aa8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00aac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ab0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ab4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ab8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00abc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ac4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ac8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ad0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ad4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ad8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0xcb000000,                                                  // -8388608.0                                  /// 00ae0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ae4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00af0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00af4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00af8
			 0xffc00001,                                                  // -signaling NaN                              /// 00afc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b00
			 0x80000000,                                                  // -zero                                       /// 00b04
			 0x7fc00001,                                                  // signaling NaN                               /// 00b08
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b10
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b14
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b1c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b20
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b24
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b28
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b2c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b30
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b34
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b38
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b3c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b40
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b48
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b4c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b50
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b54
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b58
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b5c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b60
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b64
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b68
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b74
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b7c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b80
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b88
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b8c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b90
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b94
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b98
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b9c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bb0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bb4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bb8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bbc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bc0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bc4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bc8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bcc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bd0
			 0xff800000,                                                  // -inf                                        /// 00bd4
			 0xbf028f5c,                                                  // -0.51                                       /// 00bd8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00be0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bf0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bf4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bf8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c00
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c04
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c08
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c0c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c10
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c18
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c1c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c24
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c28
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c2c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c30
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c38
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c3c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c40
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c48
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c4c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c50
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c54
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c58
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c5c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c60
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c64
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c68
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c6c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c70
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c78
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c7c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c80
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c88
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c90
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c98
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c9c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ca4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ca8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cac
			 0xff800000,                                                  // -inf                                        /// 00cb0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cc0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cc4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ccc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cd4
			 0x3f028f5c,                                                  // 0.51                                        /// 00cd8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cdc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ce0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ce4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ce8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d00
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d04
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d0c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d10
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d1c
			 0xff800000,                                                  // -inf                                        /// 00d20
			 0x7fc00001,                                                  // signaling NaN                               /// 00d24
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d28
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d2c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d30
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d34
			 0xcb000000,                                                  // -8388608.0                                  /// 00d38
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d3c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d44
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d48
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d50
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d54
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d58
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d5c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d60
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d64
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d68
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d6c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d70
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d74
			 0x80000000,                                                  // -zero                                       /// 00d78
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d7c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d84
			 0xbf028f5c,                                                  // -0.51                                       /// 00d88
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d8c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d90
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d94
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d98
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d9c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00da4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00da8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00dac
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00db0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00db8
			 0xff800000,                                                  // -inf                                        /// 00dbc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00dc0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00dc4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00dc8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00dcc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00dd0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00dd8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ddc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00de4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00de8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00dec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00df0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00df8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00dfc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e00
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e08
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e0c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e10
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e14
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e18
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e1c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e28
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e38
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e3c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e40
			 0x4b000000,                                                  // 8388608.0                                   /// 00e44
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e48
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e5c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e60
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e64
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e68
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e6c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e70
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x3f028f5c,                                                  // 0.51                                        /// 00e78
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e7c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e80
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e84
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e88
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e8c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e90
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e94
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e98
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e9c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ea4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ea8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00eac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00eb0
			 0xffc00001,                                                  // -signaling NaN                              /// 00eb4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00eb8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ec0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ec8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ecc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ed4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ed8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00edc
			 0xbf028f5c,                                                  // -0.51                                       /// 00ee0
			 0x00000000,                                                  // zero                                        /// 00ee4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ee8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00eec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ef4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ef8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f00
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f08
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f0c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f14
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x55555555,                                                  // 4 random values                             /// 00f20
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f24
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f28
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f2c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0xff800000,                                                  // -inf                                        /// 00f38
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x4b000000,                                                  // 8388608.0                                   /// 00f40
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f44
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f4c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f50
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f58
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f64
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f6c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f70
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f74
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f78
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f7c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f80
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f84
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f88
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f8c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f98
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fa0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fa4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fa8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fb0
			 0x33333333,                                                  // 4 random values                             /// 00fb4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fb8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fbc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fc0
			 0xcb000000,                                                  // -8388608.0                                  /// 00fc4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fc8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fcc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fd0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fd4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fd8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fdc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fe0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fe4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ff0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ff4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ff8
			 0x7fc00000                                                  // cquiet NaN                                  /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00000
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00004
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00008
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0000c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00010
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00014
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00018
			 0xffc00001,                                                  // -signaling NaN                              /// 0001c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00020
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00024
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0002c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00030
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00034
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00038
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0003c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00040
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00044
			 0x3f028f5c,                                                  // 0.51                                        /// 00048
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0004c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00050
			 0x0c780000,                                                  // Leading 1s:                                 /// 00054
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00058
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0005c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x0e000007,                                                  // Trailing 1s:                                /// 00064
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00068
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0006c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00070
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00078
			 0x4b000000,                                                  // 8388608.0                                   /// 0007c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00080
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0xbf028f5c,                                                  // -0.51                                       /// 00088
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00090
			 0x7fc00001,                                                  // signaling NaN                               /// 00094
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00098
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0009c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000a0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000a4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000a8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000ac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000b4
			 0xff800000,                                                  // -inf                                        /// 000b8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000bc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000c0
			 0x80000000,                                                  // -zero                                       /// 000c4
			 0x00011111,                                                  // 9.7958E-41                                  /// 000c8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000cc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000d4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000e0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 000e4
			 0x0e000001,                                                  // Trailing 1s:                                /// 000e8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000ec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000f0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000f4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000f8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00100
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00104
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0010c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00110
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00118
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0011c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00120
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00124
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00128
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0012c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00130
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00134
			 0x00000000,                                                  // zero                                        /// 00138
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0013c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00140
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00144
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00148
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0014c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00150
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00154
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00158
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0015c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00160
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00164
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00168
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0016c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00170
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00174
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00178
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0017c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00180
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00184
			 0x0e000003,                                                  // Trailing 1s:                                /// 00188
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0018c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00190
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00194
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001a4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0xff800000,                                                  // -inf                                        /// 001ac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001b4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001bc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001c8
			 0x00011111,                                                  // 9.7958E-41                                  /// 001cc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001d0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001d4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001d8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001dc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001e8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001f0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001f4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001fc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00204
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00208
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00210
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00214
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00218
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0021c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00220
			 0x33333333,                                                  // 4 random values                             /// 00224
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00228
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0022c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00230
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00234
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00238
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0023c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00240
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00244
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00250
			 0x0c780000,                                                  // Leading 1s:                                 /// 00254
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00258
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0025c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x55555555,                                                  // 4 random values                             /// 00264
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00268
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0026c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00270
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00274
			 0x0e000001,                                                  // Trailing 1s:                                /// 00278
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0027c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00280
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00284
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00288
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0028c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00294
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0029c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002a0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x0e000001,                                                  // Trailing 1s:                                /// 002b0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002b4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002bc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002c0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002cc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002d4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002d8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 002dc
			 0x7f800000,                                                  // inf                                         /// 002e0
			 0x4b000000,                                                  // 8388608.0                                   /// 002e4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002e8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002ec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002f0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002f8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002fc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00300
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00304
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00308
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0030c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00314
			 0x0e000003,                                                  // Trailing 1s:                                /// 00318
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0031c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00320
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00324
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00328
			 0x0c700000,                                                  // Leading 1s:                                 /// 0032c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00330
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00334
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00338
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00340
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00348
			 0x0e000007,                                                  // Trailing 1s:                                /// 0034c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0xffc00001,                                                  // -signaling NaN                              /// 00354
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00358
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00360
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00364
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00368
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0036c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00370
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x0e000003,                                                  // Trailing 1s:                                /// 0037c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00380
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00384
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0038c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00390
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00394
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0039c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003a0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003ac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003b0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003b4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003b8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003bc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003c0
			 0x7f800000,                                                  // inf                                         /// 003c4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003c8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003cc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003d0
			 0x80000000,                                                  // -zero                                       /// 003d4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003d8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003dc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003e0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003e4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003ec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003f4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003f8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003fc
			 0x7f800000,                                                  // inf                                         /// 00400
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00404
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0040c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x0c700000,                                                  // Leading 1s:                                 /// 00418
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0041c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00420
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00428
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0042c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00430
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00434
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00438
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0043c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00444
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00448
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00450
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00454
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00458
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00460
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00464
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00468
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0046c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00478
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0047c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00480
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00484
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00488
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0048c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00490
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0xff800000,                                                  // -inf                                        /// 00498
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0049c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004a8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004ac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004b0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004b4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004b8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004bc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004c0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004c4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004d0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004d4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004dc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004e0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004e4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004e8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004ec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004f4
			 0x4b000000,                                                  // 8388608.0                                   /// 004f8
			 0x00011111,                                                  // 9.7958E-41                                  /// 004fc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x00011111,                                                  // 9.7958E-41                                  /// 00504
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00508
			 0xff800000,                                                  // -inf                                        /// 0050c
			 0x4b000000,                                                  // 8388608.0                                   /// 00510
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00514
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00518
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0051c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00520
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00528
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0052c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00530
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00534
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00538
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0053c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00540
			 0x0c780000,                                                  // Leading 1s:                                 /// 00544
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00548
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0054c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00550
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00554
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00560
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00564
			 0xbf028f5c,                                                  // -0.51                                       /// 00568
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0056c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00570
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0057c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00580
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00584
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00588
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00598
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0059c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005a4
			 0x80000000,                                                  // -zero                                       /// 005a8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005b4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005b8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005bc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005c0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005c4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005c8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005cc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005d4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005d8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005dc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005e4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005e8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005ec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005f0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005f4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005f8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005fc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00600
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00604
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00608
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0060c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00610
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00614
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00618
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00620
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00624
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0062c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00630
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00634
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00638
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0063c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00640
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00644
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00648
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0064c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00650
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00654
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00658
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0065c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00660
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00668
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00674
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00678
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0067c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00680
			 0x80011111,                                                  // -9.7958E-41                                 /// 00684
			 0x7fc00001,                                                  // signaling NaN                               /// 00688
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00690
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00694
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0069c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006a0
			 0x0e000001,                                                  // Trailing 1s:                                /// 006a4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006a8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006ac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006b0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006b4
			 0x0c700000,                                                  // Leading 1s:                                 /// 006b8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006c8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006cc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006d4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006d8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006dc
			 0x00000000,                                                  // zero                                        /// 006e0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006e4
			 0xffc00001,                                                  // -signaling NaN                              /// 006e8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006f8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006fc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00700
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00704
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00708
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0070c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00710
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00714
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0071c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00720
			 0x00011111,                                                  // 9.7958E-41                                  /// 00724
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00728
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00730
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00738
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0073c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00740
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00744
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00748
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0074c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00750
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00754
			 0xff800000,                                                  // -inf                                        /// 00758
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0075c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00760
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0xbf028f5c,                                                  // -0.51                                       /// 00768
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0076c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00774
			 0x00011111,                                                  // 9.7958E-41                                  /// 00778
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0077c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00780
			 0xff800000,                                                  // -inf                                        /// 00784
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0078c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00794
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0079c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007a0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007ac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0xbf028f5c,                                                  // -0.51                                       /// 007b8
			 0x0c400000,                                                  // Leading 1s:                                 /// 007bc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007c8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007cc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007d0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007d8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007dc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007e0
			 0x00011111,                                                  // 9.7958E-41                                  /// 007e4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007e8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007ec
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007f8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007fc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00800
			 0xff800000,                                                  // -inf                                        /// 00804
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0080c
			 0xbf028f5c,                                                  // -0.51                                       /// 00810
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00814
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00818
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0081c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00820
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00824
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00830
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00838
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00840
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00844
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00848
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0084c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00850
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00854
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00858
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0085c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00860
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00864
			 0x00011111,                                                  // 9.7958E-41                                  /// 00868
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0086c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x80000000,                                                  // -zero                                       /// 00874
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00878
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0087c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00880
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00884
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00888
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0088c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00890
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0089c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008a0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008a4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008a8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008ac
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008b0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008b8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008c4
			 0x0e000003,                                                  // Trailing 1s:                                /// 008c8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008cc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008d0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008d4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008d8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008e0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008e4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x7f800000,                                                  // inf                                         /// 008ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008f4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008f8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00900
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00904
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00908
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00910
			 0x33333333,                                                  // 4 random values                             /// 00914
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00918
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0091c
			 0x3f028f5c,                                                  // 0.51                                        /// 00920
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00924
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00928
			 0xffc00001,                                                  // -signaling NaN                              /// 0092c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00938
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0093c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00940
			 0xbf028f5c,                                                  // -0.51                                       /// 00944
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0094c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00958
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0095c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00960
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00964
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00970
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00974
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00978
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0097c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00980
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00984
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00988
			 0x0c400000,                                                  // Leading 1s:                                 /// 0098c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00990
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00998
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0099c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009a0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009b0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009b4
			 0xff800000,                                                  // -inf                                        /// 009b8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009bc
			 0x0e000003,                                                  // Trailing 1s:                                /// 009c0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009c4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009c8
			 0x0c780000,                                                  // Leading 1s:                                 /// 009cc
			 0x4b000000,                                                  // 8388608.0                                   /// 009d0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009d8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009e0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009e4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009f0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009f4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009f8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009fc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a08
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x7f800000,                                                  // inf                                         /// 00a10
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a1c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a24
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a28
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a30
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a34
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a38
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a3c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a40
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a44
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a48
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a50
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a54
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x7f800000,                                                  // inf                                         /// 00a5c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a60
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a6c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a70
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a74
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a78
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a7c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a80
			 0xcb000000,                                                  // -8388608.0                                  /// 00a84
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a88
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a8c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a90
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a94
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a98
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a9c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00aa8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ab0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ab4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ab8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00abc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ac0
			 0x33333333,                                                  // 4 random values                             /// 00ac4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ac8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00acc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ad8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00adc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ae8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00aec
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x4b000000,                                                  // 8388608.0                                   /// 00af4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b00
			 0xcb000000,                                                  // -8388608.0                                  /// 00b04
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b08
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b0c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b10
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b20
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b24
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b28
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b2c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b30
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b34
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b38
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b3c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b40
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b44
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b4c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b50
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b54
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b58
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b5c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b60
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b64
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b6c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b70
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b74
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b7c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b80
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b84
			 0x00000000,                                                  // zero                                        /// 00b88
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b8c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b90
			 0x80000000,                                                  // -zero                                       /// 00b94
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b98
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b9c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ba0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ba4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ba8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bb0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bb4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bb8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bbc
			 0x3f028f5c,                                                  // 0.51                                        /// 00bc0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bc4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bc8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bcc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bdc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00be0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00be4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bf0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bf4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bf8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bfc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c00
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c04
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c0c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c10
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c14
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c18
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c1c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c28
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c30
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c34
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c38
			 0xbf028f5c,                                                  // -0.51                                       /// 00c3c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c44
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c48
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c50
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c54
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c58
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c5c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c64
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c68
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c6c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c70
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c74
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c78
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c80
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c84
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c88
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c8c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c90
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c94
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c98
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c9c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ca0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ca4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cb0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cb8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cbc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cc0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00cc4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ccc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cd8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cdc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ce0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ce8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cf0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00cf4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cf8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d04
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d08
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d10
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d14
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d18
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d1c
			 0xffc00001,                                                  // -signaling NaN                              /// 00d20
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d28
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d2c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d30
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d34
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d38
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d44
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d4c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d50
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d58
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d5c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d60
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d64
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d68
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d6c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d74
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d78
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d7c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d80
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d84
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d88
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d8c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d9c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00da4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00da8
			 0x4b000000,                                                  // 8388608.0                                   /// 00dac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00db0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00db8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00dbc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00dc8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dcc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00dd0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00dd8
			 0xffc00001,                                                  // -signaling NaN                              /// 00ddc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00de0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00de4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00de8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00dec
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00df0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00df8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dfc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e04
			 0x4b000000,                                                  // 8388608.0                                   /// 00e08
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e0c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e10
			 0xbf028f5c,                                                  // -0.51                                       /// 00e14
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e18
			 0x00000000,                                                  // zero                                        /// 00e1c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e24
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e30
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e34
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e38
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e3c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e40
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e48
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e4c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e54
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e58
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e64
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e68
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e6c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e70
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e7c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e80
			 0xbf028f5c,                                                  // -0.51                                       /// 00e84
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e8c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e90
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e94
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e98
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e9c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ea0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ea4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ea8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00eac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00eb8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ebc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ec0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ec8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ed4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ed8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00edc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ee0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ee4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00eec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ef0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ef4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ef8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00efc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f00
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f08
			 0xcb000000,                                                  // -8388608.0                                  /// 00f0c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f10
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f14
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f18
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f1c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f24
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f28
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f30
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f34
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f38
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f40
			 0xff800000,                                                  // -inf                                        /// 00f44
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f48
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f4c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f50
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f54
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f58
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f5c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f60
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f64
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f6c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f70
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x33333333,                                                  // 4 random values                             /// 00f78
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f84
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f88
			 0x4b000000,                                                  // 8388608.0                                   /// 00f8c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f98
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fa0
			 0x7fc00001,                                                  // signaling NaN                               /// 00fa4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fa8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fb0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fb8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fc0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fc8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fcc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fd0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fd4
			 0x4b000000,                                                  // 8388608.0                                   /// 00fd8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fdc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fe0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fe4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ff0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ff4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x80800002 // min norm + 2ulp                               // subnormals -ve                              /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x0411cb72, /// 0x0
			 0x3609c6dd, /// 0x4
			 0x79e2ea48, /// 0x8
			 0x47916ea9, /// 0xc
			 0x2adf93c3, /// 0x10
			 0xaa604a61, /// 0x14
			 0xa7a26b14, /// 0x18
			 0xfda20830, /// 0x1c
			 0x6d156542, /// 0x20
			 0x14c2b575, /// 0x24
			 0xbeae1d35, /// 0x28
			 0x0382b90e, /// 0x2c
			 0x89428e81, /// 0x30
			 0x093f3f69, /// 0x34
			 0x6b64dbe9, /// 0x38
			 0x25806e95, /// 0x3c
			 0x997622ee, /// 0x40
			 0x98254223, /// 0x44
			 0xe6203cbe, /// 0x48
			 0x47696661, /// 0x4c
			 0x0414e9de, /// 0x50
			 0x6b846036, /// 0x54
			 0xf2c03072, /// 0x58
			 0x00cff8c0, /// 0x5c
			 0xf37d39a0, /// 0x60
			 0x8d48c273, /// 0x64
			 0x4f80539c, /// 0x68
			 0x4535a7bb, /// 0x6c
			 0xc3dbc56e, /// 0x70
			 0xaf8890e2, /// 0x74
			 0x582f2bf3, /// 0x78
			 0x2260e1a2, /// 0x7c
			 0x3a6ecda6, /// 0x80
			 0xdf12e30a, /// 0x84
			 0x54d0c82b, /// 0x88
			 0xffc02e40, /// 0x8c
			 0x8e68f833, /// 0x90
			 0xd49bcf33, /// 0x94
			 0x773608eb, /// 0x98
			 0x520728c3, /// 0x9c
			 0xfa0b5dd8, /// 0xa0
			 0x5b2d747c, /// 0xa4
			 0x1eeb547b, /// 0xa8
			 0x6de11503, /// 0xac
			 0xc3f85df6, /// 0xb0
			 0x8b7b01b6, /// 0xb4
			 0x78a96572, /// 0xb8
			 0x5dbdfa9d, /// 0xbc
			 0x010e1418, /// 0xc0
			 0x1ccbe93d, /// 0xc4
			 0xd71bc41c, /// 0xc8
			 0x19752bb4, /// 0xcc
			 0x442776e4, /// 0xd0
			 0x5eb202bd, /// 0xd4
			 0x1e1adfe9, /// 0xd8
			 0xb4a4092f, /// 0xdc
			 0x0f0259ba, /// 0xe0
			 0xcb40902b, /// 0xe4
			 0xe2d1cf54, /// 0xe8
			 0xf6cdcc8a, /// 0xec
			 0x8e32b4e6, /// 0xf0
			 0x1c7bdab9, /// 0xf4
			 0x97186a84, /// 0xf8
			 0xbda44033, /// 0xfc
			 0xa0166d93, /// 0x100
			 0x93a64619, /// 0x104
			 0xfb49de14, /// 0x108
			 0x33f80ca4, /// 0x10c
			 0x4ae57911, /// 0x110
			 0x55978e00, /// 0x114
			 0xecbc9341, /// 0x118
			 0x14b3fe1e, /// 0x11c
			 0x478be819, /// 0x120
			 0xa0cd18f4, /// 0x124
			 0xe8207ad0, /// 0x128
			 0x87ddc170, /// 0x12c
			 0xf2835d67, /// 0x130
			 0xeb6fc144, /// 0x134
			 0x4d2076b7, /// 0x138
			 0x1c65653d, /// 0x13c
			 0x0d0a3ed3, /// 0x140
			 0xf82ab736, /// 0x144
			 0x657ff07b, /// 0x148
			 0xc63acb24, /// 0x14c
			 0x91f492d0, /// 0x150
			 0x73906f80, /// 0x154
			 0x96a046a1, /// 0x158
			 0x8389da27, /// 0x15c
			 0xe68092df, /// 0x160
			 0x9f37cf70, /// 0x164
			 0x1f593438, /// 0x168
			 0x4c5d2bd0, /// 0x16c
			 0x363508b1, /// 0x170
			 0x0392a26c, /// 0x174
			 0x00e9ab4e, /// 0x178
			 0x14a0c6ab, /// 0x17c
			 0x75e821c6, /// 0x180
			 0x9a6002c5, /// 0x184
			 0xf8ed555c, /// 0x188
			 0x914e7e9b, /// 0x18c
			 0xa3f1c497, /// 0x190
			 0x83c3ec45, /// 0x194
			 0xd968a1fc, /// 0x198
			 0xec07f3c8, /// 0x19c
			 0x5a41cfae, /// 0x1a0
			 0xab4172fd, /// 0x1a4
			 0x26ece998, /// 0x1a8
			 0xf8beba85, /// 0x1ac
			 0x7bb09310, /// 0x1b0
			 0x7a37713e, /// 0x1b4
			 0xfdab4e57, /// 0x1b8
			 0x600c2c08, /// 0x1bc
			 0xd2302bd9, /// 0x1c0
			 0xca6adbc1, /// 0x1c4
			 0xef3c78ad, /// 0x1c8
			 0x6bdeaf4f, /// 0x1cc
			 0xc7359f6c, /// 0x1d0
			 0x3651eec5, /// 0x1d4
			 0x1c8b22d7, /// 0x1d8
			 0x52dba62d, /// 0x1dc
			 0x525adec4, /// 0x1e0
			 0xbe866b2f, /// 0x1e4
			 0xfec9c28c, /// 0x1e8
			 0x29d8272d, /// 0x1ec
			 0x99547504, /// 0x1f0
			 0x669171f5, /// 0x1f4
			 0x16e72c5a, /// 0x1f8
			 0x02858257, /// 0x1fc
			 0x1da9a99a, /// 0x200
			 0xc9d74011, /// 0x204
			 0x25bca2a6, /// 0x208
			 0x77a007f2, /// 0x20c
			 0xb0f597b4, /// 0x210
			 0x5adb6216, /// 0x214
			 0x931a3a9d, /// 0x218
			 0x6c2b7719, /// 0x21c
			 0xe4eed3f3, /// 0x220
			 0xfec49902, /// 0x224
			 0x9e382f21, /// 0x228
			 0x998cbfdf, /// 0x22c
			 0xecdb374c, /// 0x230
			 0x6d731bcd, /// 0x234
			 0x60cda9bb, /// 0x238
			 0x65f0c41d, /// 0x23c
			 0xa7fe4117, /// 0x240
			 0x98e9e886, /// 0x244
			 0xcaf868f3, /// 0x248
			 0xd03debaf, /// 0x24c
			 0x73432366, /// 0x250
			 0xda4635b2, /// 0x254
			 0x48697d81, /// 0x258
			 0x40d0364b, /// 0x25c
			 0x52181e5e, /// 0x260
			 0x2b32c9ba, /// 0x264
			 0xd7ef8cc8, /// 0x268
			 0xb2e5b503, /// 0x26c
			 0x9aaf3adc, /// 0x270
			 0xf6e83d83, /// 0x274
			 0xbcc89fd4, /// 0x278
			 0x068ea479, /// 0x27c
			 0xe18120fb, /// 0x280
			 0x08b51681, /// 0x284
			 0xf75a9a18, /// 0x288
			 0xbb32144a, /// 0x28c
			 0xa19ba06e, /// 0x290
			 0x317c9d9b, /// 0x294
			 0x6231f6dd, /// 0x298
			 0xf6360b2e, /// 0x29c
			 0x92d741d3, /// 0x2a0
			 0x3d56d9b4, /// 0x2a4
			 0x9c24b237, /// 0x2a8
			 0xdb93a8eb, /// 0x2ac
			 0x98b24323, /// 0x2b0
			 0x2298736d, /// 0x2b4
			 0xacea3311, /// 0x2b8
			 0x35b6fe8f, /// 0x2bc
			 0xe6f4239c, /// 0x2c0
			 0x1b27d50b, /// 0x2c4
			 0x33c739fd, /// 0x2c8
			 0x5fca599b, /// 0x2cc
			 0xa3ca826d, /// 0x2d0
			 0x10e862c7, /// 0x2d4
			 0xed0b3c98, /// 0x2d8
			 0x6376ed3a, /// 0x2dc
			 0x5d643e82, /// 0x2e0
			 0x7e63c2e8, /// 0x2e4
			 0x7df885b2, /// 0x2e8
			 0xc9855c2e, /// 0x2ec
			 0x4ededab8, /// 0x2f0
			 0x8db9207f, /// 0x2f4
			 0x7746a0e2, /// 0x2f8
			 0xf687403b, /// 0x2fc
			 0x1a01fc4e, /// 0x300
			 0x632c22fa, /// 0x304
			 0x2f1d5482, /// 0x308
			 0x19c8db38, /// 0x30c
			 0x59fa0211, /// 0x310
			 0xd2ec626f, /// 0x314
			 0xefa36a44, /// 0x318
			 0xb7dac3d8, /// 0x31c
			 0xa078b15f, /// 0x320
			 0x0e37ead8, /// 0x324
			 0x25868974, /// 0x328
			 0xe5245d9c, /// 0x32c
			 0xa44856cd, /// 0x330
			 0xd12345c1, /// 0x334
			 0x43b7dddd, /// 0x338
			 0xa2c6e1de, /// 0x33c
			 0x15aae3d9, /// 0x340
			 0xbb6878e5, /// 0x344
			 0x3af08743, /// 0x348
			 0xab9653d1, /// 0x34c
			 0x03ba9f40, /// 0x350
			 0xed008474, /// 0x354
			 0x8e439dba, /// 0x358
			 0x47695495, /// 0x35c
			 0xe63429db, /// 0x360
			 0x7f35c298, /// 0x364
			 0x7dd2fdc8, /// 0x368
			 0xaec182f2, /// 0x36c
			 0xf1e02e1d, /// 0x370
			 0xa53ce8ce, /// 0x374
			 0xcd796725, /// 0x378
			 0xbcb279e2, /// 0x37c
			 0xaa656f8a, /// 0x380
			 0x1f64f6de, /// 0x384
			 0x35f50fdc, /// 0x388
			 0x020814cd, /// 0x38c
			 0x8389d362, /// 0x390
			 0xf7047ba6, /// 0x394
			 0xe45c92c6, /// 0x398
			 0xb55ad2af, /// 0x39c
			 0x356c9e32, /// 0x3a0
			 0x565856dc, /// 0x3a4
			 0x72687cb9, /// 0x3a8
			 0x202b93bb, /// 0x3ac
			 0x7b081630, /// 0x3b0
			 0xbecde3a0, /// 0x3b4
			 0x7fad01b3, /// 0x3b8
			 0xd3b7be7d, /// 0x3bc
			 0xdf2e8e79, /// 0x3c0
			 0xfcff930b, /// 0x3c4
			 0x297048c8, /// 0x3c8
			 0xbd82a9cd, /// 0x3cc
			 0x1347a82b, /// 0x3d0
			 0x8839fde5, /// 0x3d4
			 0x91682825, /// 0x3d8
			 0xd7c8d705, /// 0x3dc
			 0x8c5281af, /// 0x3e0
			 0x87d4e45e, /// 0x3e4
			 0x180418d0, /// 0x3e8
			 0x29bbfeab, /// 0x3ec
			 0x4c216184, /// 0x3f0
			 0x7c7a0ce6, /// 0x3f4
			 0x80e18a3f, /// 0x3f8
			 0x78bd0f7c, /// 0x3fc
			 0xf531b85f, /// 0x400
			 0x0e716c73, /// 0x404
			 0x95a84b81, /// 0x408
			 0x9a12ca9a, /// 0x40c
			 0x16606942, /// 0x410
			 0xadc5ef2c, /// 0x414
			 0x9debf578, /// 0x418
			 0xed272e0b, /// 0x41c
			 0x853e97e4, /// 0x420
			 0xa834fd7f, /// 0x424
			 0x2d331161, /// 0x428
			 0x9bcd6a97, /// 0x42c
			 0xef489c5f, /// 0x430
			 0x9b75604f, /// 0x434
			 0x3fda6dde, /// 0x438
			 0xa7a5300e, /// 0x43c
			 0x17d9621b, /// 0x440
			 0x47c20732, /// 0x444
			 0xad2c03a9, /// 0x448
			 0xdbe6f64f, /// 0x44c
			 0x552662f7, /// 0x450
			 0x978710a8, /// 0x454
			 0xb4825536, /// 0x458
			 0x78f07931, /// 0x45c
			 0x85b5628c, /// 0x460
			 0x9b1ec714, /// 0x464
			 0x0acac752, /// 0x468
			 0xea1c9b4b, /// 0x46c
			 0x86844f74, /// 0x470
			 0x820c2a88, /// 0x474
			 0xc78b2bb9, /// 0x478
			 0xfb948104, /// 0x47c
			 0x7f750648, /// 0x480
			 0x466d0362, /// 0x484
			 0xaea95c26, /// 0x488
			 0x665d2de6, /// 0x48c
			 0x4dbea5f2, /// 0x490
			 0xa2e513da, /// 0x494
			 0x5d6c6d91, /// 0x498
			 0x0d616d85, /// 0x49c
			 0x3ab83ee9, /// 0x4a0
			 0x6748c6f7, /// 0x4a4
			 0xe494e587, /// 0x4a8
			 0xf9179364, /// 0x4ac
			 0xce55049a, /// 0x4b0
			 0x51a45bac, /// 0x4b4
			 0x8b59e392, /// 0x4b8
			 0x26d9b1f7, /// 0x4bc
			 0xd3655b33, /// 0x4c0
			 0xdd581c1c, /// 0x4c4
			 0x3c39e16d, /// 0x4c8
			 0x9370d6ee, /// 0x4cc
			 0xd0cfbb48, /// 0x4d0
			 0xeaad4307, /// 0x4d4
			 0x45c10eaf, /// 0x4d8
			 0x0ec59e7a, /// 0x4dc
			 0xedb9bee8, /// 0x4e0
			 0x87ce29f7, /// 0x4e4
			 0xdd8515aa, /// 0x4e8
			 0x002a5905, /// 0x4ec
			 0xb6337e6c, /// 0x4f0
			 0xab12b251, /// 0x4f4
			 0xb13cb6eb, /// 0x4f8
			 0xb979b741, /// 0x4fc
			 0x5479f280, /// 0x500
			 0xff2ae93c, /// 0x504
			 0xb7e99952, /// 0x508
			 0x11af7b00, /// 0x50c
			 0x8507955a, /// 0x510
			 0x75b1df6b, /// 0x514
			 0x5aeb3d52, /// 0x518
			 0xd976854c, /// 0x51c
			 0xabbe1ea1, /// 0x520
			 0x1ad06008, /// 0x524
			 0x5ee79edf, /// 0x528
			 0x9390c49c, /// 0x52c
			 0x9f6d8a92, /// 0x530
			 0x8a8859c1, /// 0x534
			 0x4098df95, /// 0x538
			 0xa396cbd5, /// 0x53c
			 0x1c8dc4ce, /// 0x540
			 0x38a8acc3, /// 0x544
			 0x01b85a78, /// 0x548
			 0x20a4e4db, /// 0x54c
			 0x59d5c7e8, /// 0x550
			 0xa97aaa14, /// 0x554
			 0x81f0f163, /// 0x558
			 0x688c2b81, /// 0x55c
			 0x712648cf, /// 0x560
			 0xaade0aa8, /// 0x564
			 0x77fbc269, /// 0x568
			 0xf6b46f0d, /// 0x56c
			 0x97d50850, /// 0x570
			 0x4e225f5f, /// 0x574
			 0xb51ec3ec, /// 0x578
			 0x71a00851, /// 0x57c
			 0xe5e911f7, /// 0x580
			 0x446e4e50, /// 0x584
			 0x508808ba, /// 0x588
			 0xfc0e9473, /// 0x58c
			 0xd1ae3acc, /// 0x590
			 0x7620905f, /// 0x594
			 0xf0a48b92, /// 0x598
			 0x4852b849, /// 0x59c
			 0xe44996a9, /// 0x5a0
			 0x9c8fd8a2, /// 0x5a4
			 0x7f3337f2, /// 0x5a8
			 0x35e62344, /// 0x5ac
			 0x9697ccd2, /// 0x5b0
			 0xc9b51757, /// 0x5b4
			 0xdb927272, /// 0x5b8
			 0x08541bd9, /// 0x5bc
			 0xe029b47d, /// 0x5c0
			 0xbc60a1b2, /// 0x5c4
			 0xdc0df948, /// 0x5c8
			 0x363f6dba, /// 0x5cc
			 0xfaa23173, /// 0x5d0
			 0xa0eabba2, /// 0x5d4
			 0x9f81af56, /// 0x5d8
			 0x1be196f9, /// 0x5dc
			 0xf4b21067, /// 0x5e0
			 0x6a2b3018, /// 0x5e4
			 0xb811f363, /// 0x5e8
			 0xf1c22be2, /// 0x5ec
			 0xc5019cba, /// 0x5f0
			 0xe81f0393, /// 0x5f4
			 0xaa688195, /// 0x5f8
			 0x2ce1dbc9, /// 0x5fc
			 0x60776a21, /// 0x600
			 0x43797186, /// 0x604
			 0x80a73e44, /// 0x608
			 0x5fd9471d, /// 0x60c
			 0xe3865218, /// 0x610
			 0x6d232ef8, /// 0x614
			 0x69e49e7c, /// 0x618
			 0x8926c88c, /// 0x61c
			 0x1a71151a, /// 0x620
			 0xf1fcfbf6, /// 0x624
			 0xec1821c9, /// 0x628
			 0x2c199bbc, /// 0x62c
			 0xe5e2cda5, /// 0x630
			 0x26617ad8, /// 0x634
			 0xd39695a1, /// 0x638
			 0x82caebe7, /// 0x63c
			 0xf005acab, /// 0x640
			 0xe898e1f7, /// 0x644
			 0x2b25f24a, /// 0x648
			 0xecee1428, /// 0x64c
			 0xd1fb7a80, /// 0x650
			 0x45456679, /// 0x654
			 0x18d20e6e, /// 0x658
			 0x502a21a7, /// 0x65c
			 0x9f79c2d2, /// 0x660
			 0xed1c86de, /// 0x664
			 0x0d13a952, /// 0x668
			 0x94f4bbf7, /// 0x66c
			 0x9c2cd7e0, /// 0x670
			 0x3f813b8e, /// 0x674
			 0x3d67d6e5, /// 0x678
			 0xbb2dd77a, /// 0x67c
			 0x228c4bcb, /// 0x680
			 0x0d6e4ba9, /// 0x684
			 0x62a1c01e, /// 0x688
			 0xe000ddf2, /// 0x68c
			 0xae57652d, /// 0x690
			 0x441e93e5, /// 0x694
			 0xf0651d71, /// 0x698
			 0x2c939e41, /// 0x69c
			 0xa861381a, /// 0x6a0
			 0x3b5f7b56, /// 0x6a4
			 0xea6c0ca3, /// 0x6a8
			 0x4c8e39cc, /// 0x6ac
			 0x9c2dae33, /// 0x6b0
			 0x6017fbc5, /// 0x6b4
			 0x74351382, /// 0x6b8
			 0xaa7ea31a, /// 0x6bc
			 0x59606f7a, /// 0x6c0
			 0x217615bd, /// 0x6c4
			 0x5a4aad4d, /// 0x6c8
			 0xce74d5e4, /// 0x6cc
			 0xd1e4cfd9, /// 0x6d0
			 0x7485c0e6, /// 0x6d4
			 0xd0f0f19d, /// 0x6d8
			 0xb8001983, /// 0x6dc
			 0x5d85a0ac, /// 0x6e0
			 0x8869022f, /// 0x6e4
			 0x17a2ffe2, /// 0x6e8
			 0xd52975b8, /// 0x6ec
			 0xa1852e56, /// 0x6f0
			 0x426a4220, /// 0x6f4
			 0x369872fe, /// 0x6f8
			 0x68439600, /// 0x6fc
			 0xa819bbb3, /// 0x700
			 0x9693d338, /// 0x704
			 0xdb72dbf9, /// 0x708
			 0x79ba822e, /// 0x70c
			 0x6cf0b6ba, /// 0x710
			 0x08417037, /// 0x714
			 0x76d437f9, /// 0x718
			 0x9d3a3d7f, /// 0x71c
			 0xd65caf75, /// 0x720
			 0x6477cf2b, /// 0x724
			 0x6a855bfd, /// 0x728
			 0xf7022540, /// 0x72c
			 0x4b6219f3, /// 0x730
			 0x7c0d863c, /// 0x734
			 0x5e48ffaa, /// 0x738
			 0x227c1154, /// 0x73c
			 0x41e2297a, /// 0x740
			 0xb3a4f957, /// 0x744
			 0x6e591031, /// 0x748
			 0x512a0814, /// 0x74c
			 0xba8b6b44, /// 0x750
			 0x79350bd3, /// 0x754
			 0x3378e58d, /// 0x758
			 0x2e50b049, /// 0x75c
			 0xe51d75bd, /// 0x760
			 0x5dd4ea89, /// 0x764
			 0xb430f7c0, /// 0x768
			 0x54961eef, /// 0x76c
			 0xe900b608, /// 0x770
			 0x35eb68a6, /// 0x774
			 0x8af93dbe, /// 0x778
			 0xe059e3ea, /// 0x77c
			 0x44110487, /// 0x780
			 0xd9016987, /// 0x784
			 0xb7ed96cb, /// 0x788
			 0x190385de, /// 0x78c
			 0x34966a51, /// 0x790
			 0xd0093c22, /// 0x794
			 0xfdead760, /// 0x798
			 0x7de5e2bd, /// 0x79c
			 0x319a8e36, /// 0x7a0
			 0xa2d475b2, /// 0x7a4
			 0x74110d24, /// 0x7a8
			 0x6f82f964, /// 0x7ac
			 0xbc6ae424, /// 0x7b0
			 0x4c0135f7, /// 0x7b4
			 0xeeb43dd6, /// 0x7b8
			 0x2954240b, /// 0x7bc
			 0x47298749, /// 0x7c0
			 0x3e1d1212, /// 0x7c4
			 0x714be3d6, /// 0x7c8
			 0x177b5ac9, /// 0x7cc
			 0x3999af25, /// 0x7d0
			 0x07512eee, /// 0x7d4
			 0x8e5349c6, /// 0x7d8
			 0x10464be6, /// 0x7dc
			 0x96fa2fff, /// 0x7e0
			 0xfb470840, /// 0x7e4
			 0xbf9a79bc, /// 0x7e8
			 0x3f3d8db5, /// 0x7ec
			 0x8bfebee1, /// 0x7f0
			 0xbe962af4, /// 0x7f4
			 0x8458a49c, /// 0x7f8
			 0x798e39b9, /// 0x7fc
			 0x86d6107d, /// 0x800
			 0xffbfdb1e, /// 0x804
			 0x5a2fc316, /// 0x808
			 0x40890f73, /// 0x80c
			 0x244d648a, /// 0x810
			 0xbc94b0a2, /// 0x814
			 0x3766a826, /// 0x818
			 0x796f8204, /// 0x81c
			 0x471657b4, /// 0x820
			 0xf55864b8, /// 0x824
			 0x7ba383b8, /// 0x828
			 0x29636240, /// 0x82c
			 0x7a176c72, /// 0x830
			 0x846c25c9, /// 0x834
			 0x6af6483d, /// 0x838
			 0x22180356, /// 0x83c
			 0xd5560762, /// 0x840
			 0x4aa9799e, /// 0x844
			 0x82f7180e, /// 0x848
			 0xb2a16059, /// 0x84c
			 0xe166e2b6, /// 0x850
			 0x7c2cb44b, /// 0x854
			 0x2816c876, /// 0x858
			 0x06bc1a18, /// 0x85c
			 0x8b3dfd0b, /// 0x860
			 0x650b6167, /// 0x864
			 0xf406e7de, /// 0x868
			 0x4082be24, /// 0x86c
			 0x0f4406e7, /// 0x870
			 0x73594b59, /// 0x874
			 0x989dab6e, /// 0x878
			 0x53464cfc, /// 0x87c
			 0xb242040e, /// 0x880
			 0x6bb3c2cd, /// 0x884
			 0x559aed41, /// 0x888
			 0x91bde609, /// 0x88c
			 0xd9593105, /// 0x890
			 0x6b4e0d2b, /// 0x894
			 0x0a63d220, /// 0x898
			 0xda832d59, /// 0x89c
			 0x2b975731, /// 0x8a0
			 0xdea58924, /// 0x8a4
			 0xee3aba1f, /// 0x8a8
			 0xc0e061b1, /// 0x8ac
			 0x35f8e14f, /// 0x8b0
			 0x048b4c77, /// 0x8b4
			 0xdf5cd458, /// 0x8b8
			 0x4f1bc073, /// 0x8bc
			 0x5b4c0b69, /// 0x8c0
			 0xba3536c4, /// 0x8c4
			 0x299e8427, /// 0x8c8
			 0xf5935d8c, /// 0x8cc
			 0xce556829, /// 0x8d0
			 0x6f5ad9d2, /// 0x8d4
			 0x2abe733c, /// 0x8d8
			 0x1e71a0a0, /// 0x8dc
			 0x40492086, /// 0x8e0
			 0xcde15b8e, /// 0x8e4
			 0x13b3e3b8, /// 0x8e8
			 0x34b06dc3, /// 0x8ec
			 0x53afaa2e, /// 0x8f0
			 0x9b15e532, /// 0x8f4
			 0x14941a3d, /// 0x8f8
			 0x00649a4d, /// 0x8fc
			 0x4bb74706, /// 0x900
			 0x33e01d30, /// 0x904
			 0x70323f23, /// 0x908
			 0x0427a49b, /// 0x90c
			 0x888ecd24, /// 0x910
			 0xf699e071, /// 0x914
			 0x5ea8b4bb, /// 0x918
			 0x97020da8, /// 0x91c
			 0x488f00f8, /// 0x920
			 0x8e5be101, /// 0x924
			 0x7ecacb3a, /// 0x928
			 0xf11bad29, /// 0x92c
			 0x27203adc, /// 0x930
			 0x9f1d67a4, /// 0x934
			 0xbc4a5e85, /// 0x938
			 0xf6e44fa5, /// 0x93c
			 0x76a6756b, /// 0x940
			 0xd07eb087, /// 0x944
			 0x455e3c0d, /// 0x948
			 0xae844813, /// 0x94c
			 0x506ae480, /// 0x950
			 0x4c32e19e, /// 0x954
			 0xbd70bae4, /// 0x958
			 0x1aed22b3, /// 0x95c
			 0xa0b70f81, /// 0x960
			 0xde09efac, /// 0x964
			 0xb5caae72, /// 0x968
			 0x0b04aa52, /// 0x96c
			 0x54f5ff0c, /// 0x970
			 0x9b12176a, /// 0x974
			 0x805aacff, /// 0x978
			 0x53a529b8, /// 0x97c
			 0x1ae24701, /// 0x980
			 0x122afe1b, /// 0x984
			 0x5e6d31b7, /// 0x988
			 0x73151cbb, /// 0x98c
			 0xe3668c03, /// 0x990
			 0xb009f18d, /// 0x994
			 0xb1c36f7c, /// 0x998
			 0xcc4a9f71, /// 0x99c
			 0x96d9c437, /// 0x9a0
			 0x543e1cc9, /// 0x9a4
			 0x87c88ac7, /// 0x9a8
			 0x65600123, /// 0x9ac
			 0xf8340925, /// 0x9b0
			 0xb38b8ab3, /// 0x9b4
			 0x8410f56f, /// 0x9b8
			 0xb667fa30, /// 0x9bc
			 0x7ad2d9d3, /// 0x9c0
			 0xc19b07e3, /// 0x9c4
			 0xa87c506a, /// 0x9c8
			 0x44850885, /// 0x9cc
			 0xbc061fc6, /// 0x9d0
			 0xab13e879, /// 0x9d4
			 0x96d2d9b2, /// 0x9d8
			 0x1dc35b3f, /// 0x9dc
			 0x90c45022, /// 0x9e0
			 0x87005ecf, /// 0x9e4
			 0x573f89fb, /// 0x9e8
			 0x039ea2d0, /// 0x9ec
			 0x8d5bde05, /// 0x9f0
			 0x8b6de905, /// 0x9f4
			 0x9ef6cd17, /// 0x9f8
			 0x8a654d04, /// 0x9fc
			 0x99cce43d, /// 0xa00
			 0xf5cf9be7, /// 0xa04
			 0x55cb8c45, /// 0xa08
			 0xedd4fc75, /// 0xa0c
			 0xbe16418d, /// 0xa10
			 0x4b51a4f7, /// 0xa14
			 0x1da8f2e2, /// 0xa18
			 0xd04b64cc, /// 0xa1c
			 0x1f2b6c44, /// 0xa20
			 0xc354a9e7, /// 0xa24
			 0xe6802473, /// 0xa28
			 0x33bed8a7, /// 0xa2c
			 0x70c46c9f, /// 0xa30
			 0x46efe66a, /// 0xa34
			 0x3e904150, /// 0xa38
			 0x6842e39b, /// 0xa3c
			 0xd9f5e357, /// 0xa40
			 0xd441cc68, /// 0xa44
			 0xe5708545, /// 0xa48
			 0x47e62225, /// 0xa4c
			 0x674ede7d, /// 0xa50
			 0x65c8d87b, /// 0xa54
			 0xdc308beb, /// 0xa58
			 0x2331325b, /// 0xa5c
			 0xd1dd2491, /// 0xa60
			 0xc069bca9, /// 0xa64
			 0xbd9b2a85, /// 0xa68
			 0x1dcdf917, /// 0xa6c
			 0x848808ed, /// 0xa70
			 0x0b15db07, /// 0xa74
			 0xa6da4ce9, /// 0xa78
			 0xe35f7e25, /// 0xa7c
			 0x4cf4d713, /// 0xa80
			 0x5775bd56, /// 0xa84
			 0xacab305e, /// 0xa88
			 0x022379a7, /// 0xa8c
			 0x854f0d2c, /// 0xa90
			 0xbf0d9e23, /// 0xa94
			 0xce3be9da, /// 0xa98
			 0x29549802, /// 0xa9c
			 0x72c49439, /// 0xaa0
			 0xe130eb2a, /// 0xaa4
			 0x41fc9e97, /// 0xaa8
			 0x9d584c65, /// 0xaac
			 0x608b9bdf, /// 0xab0
			 0xa48d1bb7, /// 0xab4
			 0xdb3f3b1e, /// 0xab8
			 0x7a91626e, /// 0xabc
			 0xb5afdc64, /// 0xac0
			 0x16cf7ebc, /// 0xac4
			 0xf3f915b4, /// 0xac8
			 0xf79d2c56, /// 0xacc
			 0xd23889e3, /// 0xad0
			 0x9eb0f09a, /// 0xad4
			 0xc0c11dfc, /// 0xad8
			 0xd8299b29, /// 0xadc
			 0x931c9689, /// 0xae0
			 0x86a07683, /// 0xae4
			 0x66faebdd, /// 0xae8
			 0x6440453a, /// 0xaec
			 0x9b3b0c86, /// 0xaf0
			 0x4352606c, /// 0xaf4
			 0xe6817ad8, /// 0xaf8
			 0x400702fb, /// 0xafc
			 0x527ba72f, /// 0xb00
			 0x1b0aaa9b, /// 0xb04
			 0xaef2305a, /// 0xb08
			 0xab05b882, /// 0xb0c
			 0x2f92053d, /// 0xb10
			 0xc81f62bc, /// 0xb14
			 0x7210e720, /// 0xb18
			 0x44b0863e, /// 0xb1c
			 0x3bb250be, /// 0xb20
			 0x3387423c, /// 0xb24
			 0x3159ff47, /// 0xb28
			 0x44cee95b, /// 0xb2c
			 0x1da98942, /// 0xb30
			 0x6a0f77c1, /// 0xb34
			 0x632c78f5, /// 0xb38
			 0xe79fbe32, /// 0xb3c
			 0xe419f900, /// 0xb40
			 0x6efe0567, /// 0xb44
			 0x8b3a8e50, /// 0xb48
			 0x3cd1c5f8, /// 0xb4c
			 0xbf00752c, /// 0xb50
			 0xe8a60b8e, /// 0xb54
			 0x701ae2ee, /// 0xb58
			 0xf10c805c, /// 0xb5c
			 0x500e3bd1, /// 0xb60
			 0x9a4b12a2, /// 0xb64
			 0x3a29dbe0, /// 0xb68
			 0xf915affd, /// 0xb6c
			 0xe7bbb170, /// 0xb70
			 0xc127886a, /// 0xb74
			 0xec73f5aa, /// 0xb78
			 0xca5aa900, /// 0xb7c
			 0x8427a928, /// 0xb80
			 0x54f47ff9, /// 0xb84
			 0x338e36bc, /// 0xb88
			 0xc793e060, /// 0xb8c
			 0x2025c343, /// 0xb90
			 0x6f6d0d56, /// 0xb94
			 0x0a08d42b, /// 0xb98
			 0x83067859, /// 0xb9c
			 0xd6b66ae7, /// 0xba0
			 0x22c6a4bd, /// 0xba4
			 0x8aa178d5, /// 0xba8
			 0xcc84f549, /// 0xbac
			 0xc717be6a, /// 0xbb0
			 0xa78bb44f, /// 0xbb4
			 0x76a28f86, /// 0xbb8
			 0x544a5797, /// 0xbbc
			 0xdee9bd59, /// 0xbc0
			 0x2126ad17, /// 0xbc4
			 0x3ced39bd, /// 0xbc8
			 0xf5e2a079, /// 0xbcc
			 0x3cc47ea6, /// 0xbd0
			 0xa9c40277, /// 0xbd4
			 0xb200f647, /// 0xbd8
			 0x73c0e7ca, /// 0xbdc
			 0x2309d306, /// 0xbe0
			 0xb2b1093c, /// 0xbe4
			 0x9f13cd3a, /// 0xbe8
			 0x5538f3f9, /// 0xbec
			 0xcddaeb11, /// 0xbf0
			 0x07ef8cbe, /// 0xbf4
			 0xde7b502c, /// 0xbf8
			 0x5e9343f0, /// 0xbfc
			 0x0b2553e4, /// 0xc00
			 0xcb26a7b9, /// 0xc04
			 0xe75fb040, /// 0xc08
			 0x48e68331, /// 0xc0c
			 0xfc366010, /// 0xc10
			 0x344cc546, /// 0xc14
			 0xded01d94, /// 0xc18
			 0x20515f7f, /// 0xc1c
			 0x4b6cbca5, /// 0xc20
			 0x7f1dcc0a, /// 0xc24
			 0x19a9bce0, /// 0xc28
			 0x242fd435, /// 0xc2c
			 0x557ecc6e, /// 0xc30
			 0xaefb8e51, /// 0xc34
			 0x504307fc, /// 0xc38
			 0xc7053f5f, /// 0xc3c
			 0xf12887ea, /// 0xc40
			 0xe63da211, /// 0xc44
			 0x05673d41, /// 0xc48
			 0x73e6de66, /// 0xc4c
			 0x5a907e7a, /// 0xc50
			 0xbd96090a, /// 0xc54
			 0xcaff86fb, /// 0xc58
			 0xea8bb3e4, /// 0xc5c
			 0x6d83bef9, /// 0xc60
			 0xf94fcfa6, /// 0xc64
			 0x4c9fcc26, /// 0xc68
			 0x0d6c980c, /// 0xc6c
			 0xbb0a2637, /// 0xc70
			 0xf1b3f2cd, /// 0xc74
			 0x89101673, /// 0xc78
			 0xa56b4159, /// 0xc7c
			 0xcd625a84, /// 0xc80
			 0x324c0971, /// 0xc84
			 0xe7e01925, /// 0xc88
			 0x9e03a5cd, /// 0xc8c
			 0x5926d469, /// 0xc90
			 0xa804d640, /// 0xc94
			 0x6d35885c, /// 0xc98
			 0x11b732ac, /// 0xc9c
			 0x0d7fba8d, /// 0xca0
			 0xa7ff826b, /// 0xca4
			 0x1323411f, /// 0xca8
			 0x7b4c8c26, /// 0xcac
			 0xb9c4bd50, /// 0xcb0
			 0x17c11f9a, /// 0xcb4
			 0x4e70335d, /// 0xcb8
			 0xcff0ae92, /// 0xcbc
			 0x8d40b10b, /// 0xcc0
			 0x952b5ff6, /// 0xcc4
			 0xc48fde69, /// 0xcc8
			 0x49964687, /// 0xccc
			 0x76858d29, /// 0xcd0
			 0x1a9c9a2c, /// 0xcd4
			 0xc312882a, /// 0xcd8
			 0xc703b619, /// 0xcdc
			 0x76069c77, /// 0xce0
			 0xae6ed3be, /// 0xce4
			 0xdce30c85, /// 0xce8
			 0x9eaad329, /// 0xcec
			 0x43132357, /// 0xcf0
			 0x4816b067, /// 0xcf4
			 0x19c326da, /// 0xcf8
			 0x52ca834e, /// 0xcfc
			 0x5618c16e, /// 0xd00
			 0x84d2a2b7, /// 0xd04
			 0x7e12e8cf, /// 0xd08
			 0x32506d79, /// 0xd0c
			 0x3c34924f, /// 0xd10
			 0x5c067e4c, /// 0xd14
			 0xa4423686, /// 0xd18
			 0x1aff6177, /// 0xd1c
			 0x4170a526, /// 0xd20
			 0xdbf5839c, /// 0xd24
			 0x32aa9191, /// 0xd28
			 0xbd436ceb, /// 0xd2c
			 0x3e813199, /// 0xd30
			 0xbe5c51dd, /// 0xd34
			 0x375b30fd, /// 0xd38
			 0x0bed66db, /// 0xd3c
			 0x3c8fa19c, /// 0xd40
			 0x80d3ca9f, /// 0xd44
			 0xe1c7e8f0, /// 0xd48
			 0x68e2b32b, /// 0xd4c
			 0x3258b208, /// 0xd50
			 0x54d569cf, /// 0xd54
			 0xc8224755, /// 0xd58
			 0xfae32172, /// 0xd5c
			 0x74f1ddc8, /// 0xd60
			 0x0a96a698, /// 0xd64
			 0xf0939554, /// 0xd68
			 0xd7647a98, /// 0xd6c
			 0x6fc00b5e, /// 0xd70
			 0x7392ba6e, /// 0xd74
			 0x38a549fb, /// 0xd78
			 0xc48b0aae, /// 0xd7c
			 0xa6f192e6, /// 0xd80
			 0xdafb6fa2, /// 0xd84
			 0xe2839a02, /// 0xd88
			 0xe0d3d2a6, /// 0xd8c
			 0x35a8a035, /// 0xd90
			 0x5c1f3d1c, /// 0xd94
			 0xf0c27200, /// 0xd98
			 0x75739bc4, /// 0xd9c
			 0x6ba86e66, /// 0xda0
			 0x8970906b, /// 0xda4
			 0xd04e8601, /// 0xda8
			 0x1e75af80, /// 0xdac
			 0x309752c7, /// 0xdb0
			 0xe4e28209, /// 0xdb4
			 0xd8409acf, /// 0xdb8
			 0x732b6361, /// 0xdbc
			 0xed6ee872, /// 0xdc0
			 0x5d6f888a, /// 0xdc4
			 0xc4c07e86, /// 0xdc8
			 0x7ec76cdd, /// 0xdcc
			 0x364b4e7f, /// 0xdd0
			 0xb9c5bfd4, /// 0xdd4
			 0x4ddad171, /// 0xdd8
			 0xf1196a5b, /// 0xddc
			 0xb9040d8d, /// 0xde0
			 0xe1b8c67b, /// 0xde4
			 0xc94a6262, /// 0xde8
			 0xf692ec02, /// 0xdec
			 0xc21ae192, /// 0xdf0
			 0xaf57888e, /// 0xdf4
			 0x24488fa6, /// 0xdf8
			 0xbcc04bcb, /// 0xdfc
			 0x6043847d, /// 0xe00
			 0xa9d1b090, /// 0xe04
			 0x93a13dd0, /// 0xe08
			 0xa93405ec, /// 0xe0c
			 0xb40b99f3, /// 0xe10
			 0xb6fd497d, /// 0xe14
			 0x55916ff3, /// 0xe18
			 0xd6e1ce74, /// 0xe1c
			 0xc17a7c0f, /// 0xe20
			 0x7fd76142, /// 0xe24
			 0x33cc6362, /// 0xe28
			 0xea5194cc, /// 0xe2c
			 0x7d2938ea, /// 0xe30
			 0xa62f487b, /// 0xe34
			 0x75f77fe2, /// 0xe38
			 0xf9b9b247, /// 0xe3c
			 0xafdfd2fd, /// 0xe40
			 0x6dcc8bcf, /// 0xe44
			 0x03b58065, /// 0xe48
			 0x5858e46b, /// 0xe4c
			 0x40ba1c5a, /// 0xe50
			 0xf9ebb743, /// 0xe54
			 0x1a5b021c, /// 0xe58
			 0x86f24d44, /// 0xe5c
			 0x18d5ec18, /// 0xe60
			 0xc69d5190, /// 0xe64
			 0xcb9c814e, /// 0xe68
			 0xb391bb97, /// 0xe6c
			 0x82866a58, /// 0xe70
			 0x2a4213ff, /// 0xe74
			 0x8526eff1, /// 0xe78
			 0xbf83e3fd, /// 0xe7c
			 0xfe1baa1c, /// 0xe80
			 0xdaec890f, /// 0xe84
			 0x8e8736d1, /// 0xe88
			 0x67b32eac, /// 0xe8c
			 0x1c7da643, /// 0xe90
			 0xb9cfc486, /// 0xe94
			 0x64bf700d, /// 0xe98
			 0x1b046beb, /// 0xe9c
			 0x05483ef9, /// 0xea0
			 0x505b6c03, /// 0xea4
			 0x49cc90f1, /// 0xea8
			 0x8354ea6b, /// 0xeac
			 0xcc4b4cd2, /// 0xeb0
			 0x91265919, /// 0xeb4
			 0x1f52b372, /// 0xeb8
			 0x06d6b86b, /// 0xebc
			 0xc68eb8c8, /// 0xec0
			 0x7a9ebabb, /// 0xec4
			 0x24cd4bd5, /// 0xec8
			 0x6df62f9c, /// 0xecc
			 0x4e196158, /// 0xed0
			 0x3532d8a2, /// 0xed4
			 0xc9461817, /// 0xed8
			 0xfc39b006, /// 0xedc
			 0xea33eec1, /// 0xee0
			 0x56123e1c, /// 0xee4
			 0x8b68fe06, /// 0xee8
			 0x456a9136, /// 0xeec
			 0x499b85dd, /// 0xef0
			 0x79a1b9fa, /// 0xef4
			 0x107b4564, /// 0xef8
			 0x0858f763, /// 0xefc
			 0xac2599c3, /// 0xf00
			 0x6ec73eab, /// 0xf04
			 0x32bb4208, /// 0xf08
			 0x63bcc307, /// 0xf0c
			 0xa63fc654, /// 0xf10
			 0xc65e6e7c, /// 0xf14
			 0x7329bc24, /// 0xf18
			 0x21845892, /// 0xf1c
			 0xc5573cc8, /// 0xf20
			 0xd7bd5066, /// 0xf24
			 0xc472274e, /// 0xf28
			 0x4685f79b, /// 0xf2c
			 0x8744b3da, /// 0xf30
			 0x236ea515, /// 0xf34
			 0x77acef56, /// 0xf38
			 0x569712c6, /// 0xf3c
			 0x8f9c015a, /// 0xf40
			 0x8c30e863, /// 0xf44
			 0xf715df38, /// 0xf48
			 0x000c739a, /// 0xf4c
			 0xf3571c6c, /// 0xf50
			 0x7d5f16ba, /// 0xf54
			 0xec985fae, /// 0xf58
			 0x604f827c, /// 0xf5c
			 0x090aadcc, /// 0xf60
			 0xe9444e54, /// 0xf64
			 0x3e548ef6, /// 0xf68
			 0x11f451d1, /// 0xf6c
			 0x95f1a224, /// 0xf70
			 0x59472d9c, /// 0xf74
			 0xf0eaec37, /// 0xf78
			 0xcb972b34, /// 0xf7c
			 0x07c0b3b5, /// 0xf80
			 0xd8768b6d, /// 0xf84
			 0xecc98278, /// 0xf88
			 0x3f3edf91, /// 0xf8c
			 0xa85848a2, /// 0xf90
			 0xb2220a78, /// 0xf94
			 0x09565012, /// 0xf98
			 0x2435b5d7, /// 0xf9c
			 0x2009029b, /// 0xfa0
			 0xc99453d1, /// 0xfa4
			 0x396300ee, /// 0xfa8
			 0xc2de331b, /// 0xfac
			 0x4d721b8e, /// 0xfb0
			 0x1d99c15a, /// 0xfb4
			 0x66d35be8, /// 0xfb8
			 0x2f362fc5, /// 0xfbc
			 0x0cfeca80, /// 0xfc0
			 0x75196e37, /// 0xfc4
			 0x40306f36, /// 0xfc8
			 0x6d6e13bb, /// 0xfcc
			 0x95e13884, /// 0xfd0
			 0xea30e854, /// 0xfd4
			 0xb312618d, /// 0xfd8
			 0x37e50b61, /// 0xfdc
			 0x48f93a97, /// 0xfe0
			 0x120dae1a, /// 0xfe4
			 0x2e28d880, /// 0xfe8
			 0x5c9a1282, /// 0xfec
			 0x6b6f30ed, /// 0xff0
			 0x0ab99b64, /// 0xff4
			 0x4388e771, /// 0xff8
			 0xbdfc377e /// last
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
			 0x00000214,
			 0x000006a0,
			 0x000003e8,
			 0x000003d8,
			 0x00000694,
			 0x000002ac,
			 0x00000384,
			 0x000004c0,

			 /// vpu register f2
			 0x40e00000,
			 0x40c00000,
			 0x41d00000,
			 0x41980000,
			 0x41d80000,
			 0x41400000,
			 0x41f80000,
			 0x41f00000,

			 /// vpu register f3
			 0x41f80000,
			 0x41c80000,
			 0x40e00000,
			 0x41300000,
			 0x41e80000,
			 0x41f00000,
			 0x41a80000,
			 0x40800000,

			 /// vpu register f4
			 0x41a80000,
			 0x41500000,
			 0x41c80000,
			 0x41e80000,
			 0x41500000,
			 0x41a80000,
			 0x41980000,
			 0x41880000,

			 /// vpu register f5
			 0x41f80000,
			 0x41200000,
			 0x3f800000,
			 0x41400000,
			 0x41500000,
			 0x40800000,
			 0x40000000,
			 0x41c00000,

			 /// vpu register f6
			 0x40e00000,
			 0x41300000,
			 0x41400000,
			 0x40400000,
			 0x41200000,
			 0x41400000,
			 0x41a00000,
			 0x41d00000,

			 /// vpu register f7
			 0x41d00000,
			 0x41800000,
			 0x41f00000,
			 0x41980000,
			 0x41f80000,
			 0x41a00000,
			 0x41e00000,
			 0x40a00000,

			 /// vpu register f8
			 0x41b80000,
			 0x41b00000,
			 0x41700000,
			 0x41300000,
			 0x41f80000,
			 0x41700000,
			 0x41c80000,
			 0x42000000,

			 /// vpu register f9
			 0x41a80000,
			 0x40e00000,
			 0x40400000,
			 0x41f80000,
			 0x41600000,
			 0x41e80000,
			 0x41d00000,
			 0x41e80000,

			 /// vpu register f10
			 0x41f00000,
			 0x41d00000,
			 0x42000000,
			 0x40000000,
			 0x41300000,
			 0x40400000,
			 0x40a00000,
			 0x41000000,

			 /// vpu register f11
			 0x41e80000,
			 0x41100000,
			 0x41880000,
			 0x41c80000,
			 0x40a00000,
			 0x40c00000,
			 0x40000000,
			 0x41b80000,

			 /// vpu register f12
			 0x41200000,
			 0x41d80000,
			 0x42000000,
			 0x40800000,
			 0x41700000,
			 0x41c80000,
			 0x40c00000,
			 0x41700000,

			 /// vpu register f13
			 0x40000000,
			 0x40c00000,
			 0x41900000,
			 0x41900000,
			 0x41000000,
			 0x41a80000,
			 0x41c00000,
			 0x41e80000,

			 /// vpu register f14
			 0x40400000,
			 0x41f00000,
			 0x41000000,
			 0x41500000,
			 0x41980000,
			 0x41b80000,
			 0x41300000,
			 0x40800000,

			 /// vpu register f15
			 0x41300000,
			 0x41c00000,
			 0x41b00000,
			 0x41b80000,
			 0x41c80000,
			 0x41b80000,
			 0x40400000,
			 0x41980000,

			 /// vpu register f16
			 0x41000000,
			 0x41300000,
			 0x41b00000,
			 0x41800000,
			 0x41100000,
			 0x41800000,
			 0x41b00000,
			 0x41b80000,

			 /// vpu register f17
			 0x41500000,
			 0x41980000,
			 0x40800000,
			 0x41c80000,
			 0x40400000,
			 0x41200000,
			 0x41400000,
			 0x41a80000,

			 /// vpu register f18
			 0x41200000,
			 0x41c80000,
			 0x41980000,
			 0x41800000,
			 0x41900000,
			 0x41e80000,
			 0x41a80000,
			 0x41900000,

			 /// vpu register f19
			 0x40000000,
			 0x40e00000,
			 0x41200000,
			 0x41400000,
			 0x3f800000,
			 0x41900000,
			 0x41e80000,
			 0x41880000,

			 /// vpu register f20
			 0x41500000,
			 0x40a00000,
			 0x41d00000,
			 0x41d80000,
			 0x41900000,
			 0x41900000,
			 0x41000000,
			 0x40800000,

			 /// vpu register f21
			 0x41b80000,
			 0x41600000,
			 0x42000000,
			 0x41c80000,
			 0x41500000,
			 0x41500000,
			 0x41f80000,
			 0x41100000,

			 /// vpu register f22
			 0x3f800000,
			 0x41d00000,
			 0x41f80000,
			 0x41b80000,
			 0x42000000,
			 0x41b00000,
			 0x41f00000,
			 0x41d80000,

			 /// vpu register f23
			 0x41200000,
			 0x41b00000,
			 0x41f00000,
			 0x41880000,
			 0x41e00000,
			 0x41400000,
			 0x41f80000,
			 0x41700000,

			 /// vpu register f24
			 0x41700000,
			 0x41000000,
			 0x40800000,
			 0x41d00000,
			 0x40c00000,
			 0x41e80000,
			 0x41000000,
			 0x41600000,

			 /// vpu register f25
			 0x41980000,
			 0x42000000,
			 0x41200000,
			 0x40c00000,
			 0x41300000,
			 0x40c00000,
			 0x3f800000,
			 0x41f80000,

			 /// vpu register f26
			 0x41200000,
			 0x41300000,
			 0x41e80000,
			 0x41200000,
			 0x41e00000,
			 0x41100000,
			 0x41d00000,
			 0x41700000,

			 /// vpu register f27
			 0x41200000,
			 0x3f800000,
			 0x41f00000,
			 0x41900000,
			 0x41800000,
			 0x41f80000,
			 0x41800000,
			 0x40e00000,

			 /// vpu register f28
			 0x41880000,
			 0x40800000,
			 0x41500000,
			 0x41d00000,
			 0x42000000,
			 0x40000000,
			 0x42000000,
			 0x41d00000,

			 /// vpu register f29
			 0x40400000,
			 0x42000000,
			 0x41e00000,
			 0x41200000,
			 0x41f80000,
			 0x41980000,
			 0x42000000,
			 0x42000000,

			 /// vpu register f30
			 0x3f800000,
			 0x3f800000,
			 0x41c00000,
			 0x41980000,
			 0x41200000,
			 0x41700000,
			 0x41880000,
			 0x41200000,

			 /// vpu register f31
			 0x41d80000,
			 0x41700000,
			 0x41a80000,
			 0x41b80000,
			 0x41f80000,
			 0x40800000,
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
		"nop\n"
		"nop\n"
		"fgh.ps f3, f1 (x11)\n"                               ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f19, f1 (x15)\n"                              ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f9, f1 (x13)\n"                               ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f30, f1 (x13)\n"                              ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f7, f1 (x11)\n"                               ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f30, f1 (x12)\n"                              ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f23, f1 (x14)\n"                              ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f9, f1 (x11)\n"                               ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f27, f1 (x14)\n"                              ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f9, f1 (x11)\n"                               ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f30, f1 (x12)\n"                              ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f14, f1 (x11)\n"                              ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f3, f1 (x15)\n"                               ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f28, f1 (x12)\n"                              ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f17, f1 (x11)\n"                              ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f14, f1 (x12)\n"                              ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f24, f1 (x15)\n"                              ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f16, f1 (x13)\n"                              ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f19, f1 (x12)\n"                              ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f7, f1 (x15)\n"                               ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f9, f1 (x15)\n"                               ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f25, f1 (x11)\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f10, f1 (x14)\n"                              ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f26, f1 (x14)\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f16, f1 (x12)\n"                              ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f28, f1 (x13)\n"                              ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f5, f1 (x12)\n"                               ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f5, f1 (x12)\n"                               ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f6, f1 (x11)\n"                               ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f29, f1 (x11)\n"                              ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f12, f1 (x13)\n"                              ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f11, f1 (x12)\n"                              ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f17, f1 (x11)\n"                              ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f13, f1 (x11)\n"                              ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f5, f1 (x14)\n"                               ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f2, f1 (x14)\n"                               ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f29, f1 (x15)\n"                              ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f25, f1 (x15)\n"                              ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f8, f1 (x14)\n"                               ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f16, f1 (x12)\n"                              ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f11, f1 (x11)\n"                              ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f23, f1 (x13)\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f22, f1 (x12)\n"                              ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f2, f1 (x14)\n"                               ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f16, f1 (x15)\n"                              ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f15, f1 (x13)\n"                              ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f21, f1 (x14)\n"                              ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f26, f1 (x15)\n"                              ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f30, f1 (x15)\n"                              ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f27, f1 (x12)\n"                              ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f22, f1 (x15)\n"                              ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f24, f1 (x15)\n"                              ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f3, f1 (x12)\n"                               ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f18, f1 (x15)\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f15, f1 (x14)\n"                              ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f22, f1 (x11)\n"                              ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f16, f1 (x14)\n"                              ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f22, f1 (x15)\n"                              ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f15, f1 (x11)\n"                              ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f14, f1 (x14)\n"                              ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f6, f1 (x12)\n"                               ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f21, f1 (x14)\n"                              ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f18, f1 (x14)\n"                              ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f30, f1 (x11)\n"                              ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f29, f1 (x12)\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f22, f1 (x11)\n"                              ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f15, f1 (x11)\n"                              ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f17, f1 (x11)\n"                              ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f29, f1 (x11)\n"                              ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f6, f1 (x13)\n"                               ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f26, f1 (x15)\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f9, f1 (x11)\n"                               ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f2, f1 (x15)\n"                               ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f4, f1 (x12)\n"                               ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f8, f1 (x15)\n"                               ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f9, f1 (x15)\n"                               ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f21, f1 (x14)\n"                              ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f4, f1 (x12)\n"                               ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f11, f1 (x12)\n"                              ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f18, f1 (x12)\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f19, f1 (x11)\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f8, f1 (x15)\n"                               ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f7, f1 (x14)\n"                               ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f7, f1 (x14)\n"                               ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f29, f1 (x12)\n"                              ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f30, f1 (x15)\n"                              ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f8, f1 (x13)\n"                               ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f23, f1 (x12)\n"                              ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f22, f1 (x12)\n"                              ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f3, f1 (x13)\n"                               ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f12, f1 (x14)\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f21, f1 (x12)\n"                              ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f9, f1 (x11)\n"                               ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f11, f1 (x14)\n"                              ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f6, f1 (x15)\n"                               ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f30, f1 (x13)\n"                              ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f15, f1 (x14)\n"                              ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f7, f1 (x13)\n"                               ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fgh.ps f15, f1 (x14)\n"                              ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
