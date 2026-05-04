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
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00000
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00010
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00014
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0001c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00020
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00024
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00028
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0002c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00038
			 0xcb000000,                                                  // -8388608.0                                  /// 0003c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00048
			 0x80000000,                                                  // -zero                                       /// 0004c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00050
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00054
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0005c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00064
			 0x0e000007,                                                  // Trailing 1s:                                /// 00068
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0006c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00070
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00074
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00078
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0007c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00080
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00084
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00090
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00094
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00098
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0009c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000a0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x7f800000,                                                  // inf                                         /// 000a8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000b0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000b4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000b8
			 0x33333333,                                                  // 4 random values                             /// 000bc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000c0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000c8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000cc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000d0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000d8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000e0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000e4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000e8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000fc
			 0x00000000,                                                  // zero                                        /// 00100
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00104
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00108
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0010c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00110
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00118
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0011c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00120
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00124
			 0xff800000,                                                  // -inf                                        /// 00128
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00130
			 0x00000000,                                                  // zero                                        /// 00134
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00138
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0013c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00140
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00144
			 0xbf028f5c,                                                  // -0.51                                       /// 00148
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0014c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00154
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00158
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0015c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00160
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0xffc00001,                                                  // -signaling NaN                              /// 00168
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00178
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00180
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00184
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0018c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00190
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0019c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001a0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001a8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001b4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 001c0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001c8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001d4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001d8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001dc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001e4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001f0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 001f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001fc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00200
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00204
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00210
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00218
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0021c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00224
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00230
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00234
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00240
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00244
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00250
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00254
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00260
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00264
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00268
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0026c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00270
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00274
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00278
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0027c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00280
			 0xbf028f5c,                                                  // -0.51                                       /// 00284
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00288
			 0x0c400000,                                                  // Leading 1s:                                 /// 0028c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00290
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00294
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00298
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0029c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002a0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002a4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002a8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002b4
			 0x80000000,                                                  // -zero                                       /// 002b8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002bc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002c0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002c4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002c8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002d4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002d8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002dc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002e4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002f8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002fc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00304
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00308
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00310
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00318
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00324
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00328
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0032c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00330
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00334
			 0x0c400000,                                                  // Leading 1s:                                 /// 00338
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0033c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00340
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00344
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00348
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0034c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00354
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00358
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0035c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00360
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00364
			 0xffc00001,                                                  // -signaling NaN                              /// 00368
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x7f800000,                                                  // inf                                         /// 00370
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00374
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00378
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0037c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00380
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00384
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0038c
			 0x00000000,                                                  // zero                                        /// 00390
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00398
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0039c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003a8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003ac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003b4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003b8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003c0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003c8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003cc
			 0x7fc00001,                                                  // signaling NaN                               /// 003d0
			 0x4b000000,                                                  // 8388608.0                                   /// 003d4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003d8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003dc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003e8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003f4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003f8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00400
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00414
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00418
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00420
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00424
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00428
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0042c
			 0xcb000000,                                                  // -8388608.0                                  /// 00430
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00434
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00438
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00440
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x0e000003,                                                  // Trailing 1s:                                /// 00448
			 0x0c600000,                                                  // Leading 1s:                                 /// 0044c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00450
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00454
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00458
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00460
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00464
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00468
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0046c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00470
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00478
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00480
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x0c400000,                                                  // Leading 1s:                                 /// 00488
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0048c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00498
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0049c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x7fc00001,                                                  // signaling NaN                               /// 004a4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x0c780000,                                                  // Leading 1s:                                 /// 004ac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004b0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004b4
			 0x0c700000,                                                  // Leading 1s:                                 /// 004b8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004c4
			 0x7fc00001,                                                  // signaling NaN                               /// 004c8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004d4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004d8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004dc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004e4
			 0xcb000000,                                                  // -8388608.0                                  /// 004e8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004f4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004f8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004fc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00504
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00510
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00514
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00518
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0051c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00520
			 0x0e000001,                                                  // Trailing 1s:                                /// 00524
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00528
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0052c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00530
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x80000000,                                                  // -zero                                       /// 00538
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0053c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00540
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00548
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0054c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00550
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00554
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0055c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00560
			 0xcb000000,                                                  // -8388608.0                                  /// 00564
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00568
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0056c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00570
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00574
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0057c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00580
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00584
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x0e000007,                                                  // Trailing 1s:                                /// 0058c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00590
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00594
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0059c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005a0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005b4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005b8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005bc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005c0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005c4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005c8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005d4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005d8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005dc
			 0x0c780000,                                                  // Leading 1s:                                 /// 005e0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005e8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005ec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005f4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005f8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005fc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00600
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00604
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00608
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0060c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00610
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00614
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00618
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00624
			 0x7fc00001,                                                  // signaling NaN                               /// 00628
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0062c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00630
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00638
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0063c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00644
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0064c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00650
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00654
			 0xbf028f5c,                                                  // -0.51                                       /// 00658
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0065c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00660
			 0x0c780000,                                                  // Leading 1s:                                 /// 00664
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00668
			 0x0e000001,                                                  // Trailing 1s:                                /// 0066c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00670
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00674
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x33333333,                                                  // 4 random values                             /// 0067c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00680
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00684
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00688
			 0x4b000000,                                                  // 8388608.0                                   /// 0068c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00690
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00694
			 0x7f800000,                                                  // inf                                         /// 00698
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0069c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006a0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006a4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006a8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006b0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006b8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006c4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006c8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006d4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006dc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006e0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x0c700000,                                                  // Leading 1s:                                 /// 006e8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006ec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006f0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006f4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006f8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0xffc00001,                                                  // -signaling NaN                              /// 00704
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00708
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00710
			 0x0c700000,                                                  // Leading 1s:                                 /// 00714
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00718
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00720
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00728
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0072c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00730
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00740
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00750
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00754
			 0x3f028f5c,                                                  // 0.51                                        /// 00758
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0075c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00760
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00764
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00768
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x80000000,                                                  // -zero                                       /// 00770
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00774
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00778
			 0x0c400000,                                                  // Leading 1s:                                 /// 0077c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00780
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00784
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00788
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0078c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00790
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00794
			 0x0e000007,                                                  // Trailing 1s:                                /// 00798
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0079c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007a0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007ac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007b0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007b4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007b8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007bc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007c8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007cc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007d0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 007d4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007d8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007dc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007ec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007f4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007f8
			 0xbf028f5c,                                                  // -0.51                                       /// 007fc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00804
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00810
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00814
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00824
			 0x0e000007,                                                  // Trailing 1s:                                /// 00828
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0082c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00834
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00838
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00840
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00844
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00848
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00850
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00854
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00858
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0085c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00860
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00864
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00868
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0086c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00870
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00878
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0087c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00880
			 0x0e000007,                                                  // Trailing 1s:                                /// 00884
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0088c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00890
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00894
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00898
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0089c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008a0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008a4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008a8
			 0x80011111,                                                  // -9.7958E-41                                 /// 008ac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008b0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008b4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008bc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008c0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008c4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008cc
			 0x4b000000,                                                  // 8388608.0                                   /// 008d0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008d8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008e0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008e4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008e8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008f0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008f4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00900
			 0x80011111,                                                  // -9.7958E-41                                 /// 00904
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00908
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00914
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00918
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x3f028f5c,                                                  // 0.51                                        /// 00920
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00924
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00928
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0092c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00930
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00934
			 0x7f800000,                                                  // inf                                         /// 00938
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0093c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00940
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00944
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00948
			 0x00000000,                                                  // zero                                        /// 0094c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00950
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00954
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00958
			 0x00000000,                                                  // zero                                        /// 0095c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00960
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00964
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00968
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0096c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00970
			 0x55555555,                                                  // 4 random values                             /// 00974
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00978
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0097c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00980
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00984
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00988
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0098c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00990
			 0x0c600000,                                                  // Leading 1s:                                 /// 00994
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00998
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0099c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009a0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009a4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009a8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009b4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0xffc00001,                                                  // -signaling NaN                              /// 009c4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009c8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009d4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009d8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009e4
			 0x80000000,                                                  // -zero                                       /// 009e8
			 0x55555555,                                                  // 4 random values                             /// 009ec
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009f8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0xff800000,                                                  // -inf                                        /// 00a04
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a08
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a0c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a10
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a14
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a1c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a20
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a28
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a2c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a38
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a40
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a44
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a48
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a4c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a50
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a54
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a5c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a60
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a64
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a68
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a6c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a74
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a7c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a80
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a84
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a88
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a8c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a90
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a94
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a98
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00aa0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00aa4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00aac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ab0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ab4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00abc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ac0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00acc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ad0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ad4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00adc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ae0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ae4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00af0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00af4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00af8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x4b000000,                                                  // 8388608.0                                   /// 00b00
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b04
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x7f800000,                                                  // inf                                         /// 00b0c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b10
			 0x55555555,                                                  // 4 random values                             /// 00b14
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b18
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b1c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b24
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b28
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b2c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b30
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b34
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b3c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b40
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b44
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b48
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b4c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b50
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x3f028f5c,                                                  // 0.51                                        /// 00b5c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x4b000000,                                                  // 8388608.0                                   /// 00b64
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b68
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b70
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b78
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b80
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b84
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b88
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b8c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b90
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b94
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ba0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ba4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ba8
			 0xffc00001,                                                  // -signaling NaN                              /// 00bac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bb0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bb4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bb8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00bbc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bc8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bcc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bd4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bd8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bdc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00be0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00be4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00be8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bf0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bf4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bf8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bfc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c00
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c04
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c08
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c0c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c10
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c1c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c24
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c28
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c30
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c34
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c38
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c3c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c40
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c44
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c4c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c54
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c60
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c64
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c6c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c70
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c74
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c78
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c84
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c88
			 0xff800000,                                                  // -inf                                        /// 00c8c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c90
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c94
			 0xbf028f5c,                                                  // -0.51                                       /// 00c98
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c9c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ca4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ca8
			 0x7fc00001,                                                  // signaling NaN                               /// 00cac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cb4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cb8
			 0xcb000000,                                                  // -8388608.0                                  /// 00cbc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cc0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00cc8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ccc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cd0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cd8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cdc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ce0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ce4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ce8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0xbf028f5c,                                                  // -0.51                                       /// 00cf0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cfc
			 0xbf028f5c,                                                  // -0.51                                       /// 00d00
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d04
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d08
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d0c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d10
			 0x7fc00001,                                                  // signaling NaN                               /// 00d14
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d18
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d1c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d20
			 0x7fc00001,                                                  // signaling NaN                               /// 00d24
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d2c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d30
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d38
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d40
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d44
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d4c
			 0xbf028f5c,                                                  // -0.51                                       /// 00d50
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d54
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d5c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d60
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d64
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d68
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d6c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d78
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d7c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d80
			 0x33333333,                                                  // 4 random values                             /// 00d84
			 0xcb000000,                                                  // -8388608.0                                  /// 00d88
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d8c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d90
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d94
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d98
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00da0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00da4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00dac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00db4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00db8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00dbc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dc0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00dc4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00dcc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0xbf028f5c,                                                  // -0.51                                       /// 00dd8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ddc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00de4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00de8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00dec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00df0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00df4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00df8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dfc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e00
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e04
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e08
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e0c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e14
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e18
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e1c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e20
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e24
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e28
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e2c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e30
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e34
			 0x4b000000,                                                  // 8388608.0                                   /// 00e38
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e44
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e50
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e58
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e5c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e60
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e64
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e70
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e74
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e84
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e88
			 0xff800000,                                                  // -inf                                        /// 00e8c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e94
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e98
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e9c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ea0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ea4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ea8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00eb0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ec8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ecc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ed8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00edc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00eec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ef0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ef4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00efc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f00
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f04
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f10
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f1c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f20
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f24
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f28
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f2c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f30
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f3c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f44
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f48
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f4c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f54
			 0x55555555,                                                  // 4 random values                             /// 00f58
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f5c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f60
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f64
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f70
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f74
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f78
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f7c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f84
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f88
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f8c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f94
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f98
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f9c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fa0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fac
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fb0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fb4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fb8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fbc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fc4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fc8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fd0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fd4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fd8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fdc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fe0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fe4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fe8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ff0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ff4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x7fffffff // QNan                                          // SP +ve numbers                              /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00000
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00004
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00008
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0000c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00010
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00014
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0001c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00020
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00024
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00030
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00034
			 0xcb000000,                                                  // -8388608.0                                  /// 00038
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0003c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00044
			 0xcb000000,                                                  // -8388608.0                                  /// 00048
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0004c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00054
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00058
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00060
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00064
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0006c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00070
			 0x0c600000,                                                  // Leading 1s:                                 /// 00074
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0007c
			 0x3f028f5c,                                                  // 0.51                                        /// 00080
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00084
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00088
			 0x0c780000,                                                  // Leading 1s:                                 /// 0008c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00094
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0009c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000a0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000a4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000ac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 000b0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000b8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000c0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000c4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000c8
			 0xffc00001,                                                  // -signaling NaN                              /// 000cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000d8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000dc
			 0x00000000,                                                  // zero                                        /// 000e0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000e4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000e8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000ec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 000f4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000f8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00104
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00108
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0010c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00114
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00118
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00120
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00124
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00128
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00130
			 0x0e000003,                                                  // Trailing 1s:                                /// 00134
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0013c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00140
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00148
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00150
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00158
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0015c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00160
			 0x7f800000,                                                  // inf                                         /// 00164
			 0x0e000001,                                                  // Trailing 1s:                                /// 00168
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0016c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00170
			 0x0e000007,                                                  // Trailing 1s:                                /// 00174
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00178
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0017c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00180
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00184
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00188
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0018c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00194
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00198
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0019c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001a4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001ac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001b0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001b4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001b8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001bc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001c4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001cc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001d4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001d8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001e4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001e8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001f0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001f8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001fc
			 0x7f800000,                                                  // inf                                         /// 00200
			 0x0e000007,                                                  // Trailing 1s:                                /// 00204
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00208
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0020c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00214
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00218
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0021c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00220
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00224
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00228
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0022c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00230
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00234
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00238
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00240
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00244
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00248
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00250
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00254
			 0x0e000003,                                                  // Trailing 1s:                                /// 00258
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0025c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00260
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00268
			 0x33333333,                                                  // 4 random values                             /// 0026c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00270
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00274
			 0x0e000007,                                                  // Trailing 1s:                                /// 00278
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00280
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x0e000001,                                                  // Trailing 1s:                                /// 00288
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0028c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00290
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00298
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0029c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002a4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002a8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002b0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002b4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002b8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002c0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002c8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002d4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002d8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002dc
			 0xcb000000,                                                  // -8388608.0                                  /// 002e0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002e4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002e8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002f4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002f8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002fc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00300
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00304
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00308
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0030c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00310
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00314
			 0x0c600000,                                                  // Leading 1s:                                 /// 00318
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0031c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00320
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00324
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00328
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00330
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00334
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00338
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0033c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00340
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00344
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00348
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0034c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00350
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00354
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00358
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00360
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00364
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0036c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00370
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00374
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00378
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0xbf028f5c,                                                  // -0.51                                       /// 00380
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00384
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00388
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0038c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00390
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00394
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00398
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0039c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003a0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003a4
			 0x80000000,                                                  // -zero                                       /// 003a8
			 0x80011111,                                                  // -9.7958E-41                                 /// 003ac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003b0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003b4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003b8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003bc
			 0x0c780000,                                                  // Leading 1s:                                 /// 003c0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003c8
			 0x00000000,                                                  // zero                                        /// 003cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003dc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x0c700000,                                                  // Leading 1s:                                 /// 003e4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003e8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003ec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003f0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003f4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003f8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003fc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00404
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00408
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0040c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00410
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00414
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00418
			 0x00011111,                                                  // 9.7958E-41                                  /// 0041c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00428
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00438
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00440
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00444
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0044c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00454
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00458
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0045c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00460
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00464
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00468
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00470
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00474
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00478
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0047c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00484
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00488
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00490
			 0x0c400000,                                                  // Leading 1s:                                 /// 00494
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00498
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0049c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004a0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004a8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004ac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004b0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004b4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004bc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004c0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004c4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004d4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004e0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004e4
			 0x0c700000,                                                  // Leading 1s:                                 /// 004e8
			 0x0e000007,                                                  // Trailing 1s:                                /// 004ec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004f0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004f8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004fc
			 0x7fc00001,                                                  // signaling NaN                               /// 00500
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00504
			 0x80000000,                                                  // -zero                                       /// 00508
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0050c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00510
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00514
			 0x3f028f5c,                                                  // 0.51                                        /// 00518
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0051c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x0e000001,                                                  // Trailing 1s:                                /// 00524
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00530
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00534
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00538
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0053c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00548
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0054c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00550
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00554
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0xffc00001,                                                  // -signaling NaN                              /// 0055c
			 0x80000000,                                                  // -zero                                       /// 00560
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00564
			 0x3f028f5c,                                                  // 0.51                                        /// 00568
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00570
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x0c600000,                                                  // Leading 1s:                                 /// 00578
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0057c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00580
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00584
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00588
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0058c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00590
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00594
			 0x0e000001,                                                  // Trailing 1s:                                /// 00598
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0059c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005a0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005a8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005b4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005bc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005c0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005c4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0xff800000,                                                  // -inf                                        /// 005cc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005d0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005d4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005d8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005e0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005e8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005ec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005f0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005f4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005f8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005fc
			 0xff800000,                                                  // -inf                                        /// 00600
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00604
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00608
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0060c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00614
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00618
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0061c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00620
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00624
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00628
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0062c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00630
			 0xff800000,                                                  // -inf                                        /// 00634
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00638
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00640
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00644
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00648
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0064c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0xbf028f5c,                                                  // -0.51                                       /// 00654
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0065c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00660
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00664
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00668
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00670
			 0x7f800000,                                                  // inf                                         /// 00674
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00678
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0067c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00680
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00684
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00688
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0068c
			 0xffc00001,                                                  // -signaling NaN                              /// 00690
			 0x7fc00001,                                                  // signaling NaN                               /// 00694
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00698
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0069c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006a4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006ac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x0c400000,                                                  // Leading 1s:                                 /// 006c0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006c4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006c8
			 0x00011111,                                                  // 9.7958E-41                                  /// 006cc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006d0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006d4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006d8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006e0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006e4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006e8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006ec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 006fc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00700
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00708
			 0x0c700000,                                                  // Leading 1s:                                 /// 0070c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x55555555,                                                  // 4 random values                             /// 00714
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00718
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0071c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00720
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00724
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0072c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00730
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00738
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00740
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00744
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00748
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0074c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00754
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0075c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00760
			 0x7f800000,                                                  // inf                                         /// 00764
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00768
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0076c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00774
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00778
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0077c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00780
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00784
			 0xcb000000,                                                  // -8388608.0                                  /// 00788
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0078c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00790
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00794
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00798
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0079c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007a4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007a8
			 0x33333333,                                                  // 4 random values                             /// 007ac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007b4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007b8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007bc
			 0x0c600000,                                                  // Leading 1s:                                 /// 007c0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007c8
			 0xcb000000,                                                  // -8388608.0                                  /// 007cc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007d0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007d4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007d8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007e0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007e4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007e8
			 0x0e000003,                                                  // Trailing 1s:                                /// 007ec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x0e000003,                                                  // Trailing 1s:                                /// 007f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007fc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00800
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00804
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00808
			 0xcb000000,                                                  // -8388608.0                                  /// 0080c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00810
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00814
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00818
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0081c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00820
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00824
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00828
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0082c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00830
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x7fc00001,                                                  // signaling NaN                               /// 0083c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00844
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00848
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0084c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00850
			 0x0c780000,                                                  // Leading 1s:                                 /// 00854
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00858
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0085c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00864
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00868
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0086c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00870
			 0x0c600000,                                                  // Leading 1s:                                 /// 00874
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0087c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00880
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00888
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0088c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00890
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00894
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0089c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008a4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008a8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008ac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008b8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008bc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008c4
			 0x33333333,                                                  // 4 random values                             /// 008c8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008cc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008d0
			 0x4b000000,                                                  // 8388608.0                                   /// 008d4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008d8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008e0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008e8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008ec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008f4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008f8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008fc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00900
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00904
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00908
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0090c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00910
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00918
			 0xcb000000,                                                  // -8388608.0                                  /// 0091c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00920
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00924
			 0x0c780000,                                                  // Leading 1s:                                 /// 00928
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0092c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00930
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00934
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00938
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0093c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00940
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00944
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00948
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00950
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0095c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00964
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00968
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00970
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x00011111,                                                  // 9.7958E-41                                  /// 00978
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0097c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00980
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00984
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00988
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0098c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00990
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00994
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00998
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009a0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009a8
			 0x0c700000,                                                  // Leading 1s:                                 /// 009ac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009b0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009b8
			 0x80011111,                                                  // -9.7958E-41                                 /// 009bc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009c0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009c8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009cc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009d8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009e0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009e4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009e8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009f0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009f4
			 0x3f028f5c,                                                  // 0.51                                        /// 009f8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009fc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a00
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a04
			 0xff800000,                                                  // -inf                                        /// 00a08
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a10
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a14
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a18
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a1c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a24
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a28
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a2c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a30
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a38
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a3c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a40
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a44
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a48
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a4c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a50
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a58
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a60
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a64
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a6c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a70
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a78
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a80
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a84
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a88
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a8c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a94
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a98
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a9c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00aa0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00aa4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00aac
			 0x3f028f5c,                                                  // 0.51                                        /// 00ab0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0xbf028f5c,                                                  // -0.51                                       /// 00ab8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ac0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ad0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ad4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ad8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ae0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ae4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ae8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00aec
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00af8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00afc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b04
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b08
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b0c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b10
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b18
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b1c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b20
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b24
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b2c
			 0x55555555,                                                  // 4 random values                             /// 00b30
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b3c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b40
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b44
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b48
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b50
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b54
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0xbf028f5c,                                                  // -0.51                                       /// 00b60
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b64
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b68
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b70
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b74
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b78
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b7c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b80
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b84
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b88
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b8c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b90
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b94
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b9c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ba4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bb0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bb4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bb8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bbc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bcc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bd0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bd4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bd8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bdc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00be4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00be8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x7fc00001,                                                  // signaling NaN                               /// 00bf4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bfc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c00
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c04
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c08
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c0c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c10
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c14
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c20
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c24
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c28
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0xff800000,                                                  // -inf                                        /// 00c30
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c38
			 0x7f800000,                                                  // inf                                         /// 00c3c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c48
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c4c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c60
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c64
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c68
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c70
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c74
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c78
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c7c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c84
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c8c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c90
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c94
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c98
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ca0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ca8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cb0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cb4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cb8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cc0
			 0xbf028f5c,                                                  // -0.51                                       /// 00cc4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cc8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cd0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cd4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ce0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ce4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ce8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00cec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cf0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cf4
			 0x00000000,                                                  // zero                                        /// 00cf8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d00
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d04
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x00000000,                                                  // zero                                        /// 00d0c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d14
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d1c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d20
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d24
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d2c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d34
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d40
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d48
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d4c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d50
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d54
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d5c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d60
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d64
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d68
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d7c
			 0xffc00001,                                                  // -signaling NaN                              /// 00d80
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d84
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d88
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d8c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d90
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d94
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d98
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00da0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00da4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00da8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00dac
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00db4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00db8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00dbc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dc8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00dd0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00dd4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00dd8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ddc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00de4
			 0xbf028f5c,                                                  // -0.51                                       /// 00de8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00df0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00df4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00dfc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e00
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e04
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e0c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e14
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e18
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e1c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e20
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e28
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e2c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e30
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e34
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e38
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e3c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e44
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e4c
			 0x4b000000,                                                  // 8388608.0                                   /// 00e50
			 0xbf028f5c,                                                  // -0.51                                       /// 00e54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e5c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e64
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e68
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e6c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e74
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e78
			 0x3f028f5c,                                                  // 0.51                                        /// 00e7c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e8c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e90
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e9c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ea0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ea4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ea8
			 0x55555555,                                                  // 4 random values                             /// 00eac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00eb8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ebc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ec0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ec4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ed4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ed8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00edc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ee0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ee4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ee8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x00000000,                                                  // zero                                        /// 00ef0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ef8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00efc
			 0x33333333,                                                  // 4 random values                             /// 00f00
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f08
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f0c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f10
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f20
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f28
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f30
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f34
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f38
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f3c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f40
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f44
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f4c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f50
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f54
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f5c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f64
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f6c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f70
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f78
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f7c
			 0x55555555,                                                  // 4 random values                             /// 00f80
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x3f028f5c,                                                  // 0.51                                        /// 00f8c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f90
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f94
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f9c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fa4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fa8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fb0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fbc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fc0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fc4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fcc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fd0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fd4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fd8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fdc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0xbf028f5c,                                                  // -0.51                                       /// 00fe4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fe8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ff4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ff8
			 0x0c7ffe00                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0xc3279951, /// 0x0
			 0xb4c4a99e, /// 0x4
			 0x669646db, /// 0x8
			 0xcbe1d114, /// 0xc
			 0xf088f22c, /// 0x10
			 0x4d9a477e, /// 0x14
			 0xfc6948c9, /// 0x18
			 0xa9e860b1, /// 0x1c
			 0x7262a752, /// 0x20
			 0x9a112778, /// 0x24
			 0x6e671158, /// 0x28
			 0x3ba12f80, /// 0x2c
			 0x42dfb253, /// 0x30
			 0xd6c4a4ba, /// 0x34
			 0x3ca57831, /// 0x38
			 0xfeadadf5, /// 0x3c
			 0xc55405a3, /// 0x40
			 0x0e61df38, /// 0x44
			 0x52608f05, /// 0x48
			 0x1c1bb535, /// 0x4c
			 0x5a891852, /// 0x50
			 0xf5fdc7fa, /// 0x54
			 0xf39fbb2c, /// 0x58
			 0x3753a8f4, /// 0x5c
			 0xc778f6ac, /// 0x60
			 0x239d44b7, /// 0x64
			 0x3959283c, /// 0x68
			 0x71b295b1, /// 0x6c
			 0x2a6aed30, /// 0x70
			 0xf6521e8c, /// 0x74
			 0x3449f0ae, /// 0x78
			 0x674491e3, /// 0x7c
			 0xfe8d5b56, /// 0x80
			 0xbabec98b, /// 0x84
			 0x2002e87d, /// 0x88
			 0x9688ce04, /// 0x8c
			 0xc876c7fe, /// 0x90
			 0x1f6b7cb9, /// 0x94
			 0xd26257d8, /// 0x98
			 0x0f6dd308, /// 0x9c
			 0x564608c0, /// 0xa0
			 0x3a4fd6f8, /// 0xa4
			 0x154a04ae, /// 0xa8
			 0x487b7d5a, /// 0xac
			 0xfe05af12, /// 0xb0
			 0x0a289684, /// 0xb4
			 0xa20c6579, /// 0xb8
			 0x0691122d, /// 0xbc
			 0x027964b8, /// 0xc0
			 0x784c8b50, /// 0xc4
			 0xab579118, /// 0xc8
			 0x9f7f4ad5, /// 0xcc
			 0xf493ad38, /// 0xd0
			 0xb2fba37e, /// 0xd4
			 0xf1e3248f, /// 0xd8
			 0x500d0e6d, /// 0xdc
			 0x8b36d56c, /// 0xe0
			 0x55691163, /// 0xe4
			 0x95a9b7cd, /// 0xe8
			 0x4d7438a9, /// 0xec
			 0xcd696dcc, /// 0xf0
			 0x7e8b8333, /// 0xf4
			 0xf982f6e8, /// 0xf8
			 0x7023013c, /// 0xfc
			 0x6226e8e8, /// 0x100
			 0xf37c8c89, /// 0x104
			 0x8ce55bb2, /// 0x108
			 0x0a0ffeb3, /// 0x10c
			 0xa990d5db, /// 0x110
			 0xf6d45fa2, /// 0x114
			 0x4ac76999, /// 0x118
			 0x7f9a2a84, /// 0x11c
			 0x921abd13, /// 0x120
			 0x6d8c0d90, /// 0x124
			 0x26021263, /// 0x128
			 0x821239b7, /// 0x12c
			 0xc499a959, /// 0x130
			 0x2dc96425, /// 0x134
			 0xb0c944ee, /// 0x138
			 0x82bc3d72, /// 0x13c
			 0x93f59b7c, /// 0x140
			 0x4b0aea35, /// 0x144
			 0x74598301, /// 0x148
			 0xc7b788dc, /// 0x14c
			 0x8c2305e7, /// 0x150
			 0x41b55a42, /// 0x154
			 0x8a03ca88, /// 0x158
			 0xc0e224e1, /// 0x15c
			 0x1c085899, /// 0x160
			 0x65f544a5, /// 0x164
			 0xf76c3fe3, /// 0x168
			 0xc4fceb09, /// 0x16c
			 0xd37638b7, /// 0x170
			 0x6fa2ae54, /// 0x174
			 0x232fcc54, /// 0x178
			 0x9688a953, /// 0x17c
			 0x47c8ff04, /// 0x180
			 0x6f0297f2, /// 0x184
			 0x0c12bd64, /// 0x188
			 0x3edb2252, /// 0x18c
			 0xddb4a0bb, /// 0x190
			 0xb30af768, /// 0x194
			 0x7df154fc, /// 0x198
			 0xfda2235b, /// 0x19c
			 0x553efed8, /// 0x1a0
			 0x98b97341, /// 0x1a4
			 0x9b7efc93, /// 0x1a8
			 0xa2d46c53, /// 0x1ac
			 0x399ee6a5, /// 0x1b0
			 0xc840089c, /// 0x1b4
			 0x8af90c47, /// 0x1b8
			 0x391fcf5f, /// 0x1bc
			 0xd4adc5f3, /// 0x1c0
			 0x4efa5fe6, /// 0x1c4
			 0xc3ea16f0, /// 0x1c8
			 0x981c2e59, /// 0x1cc
			 0xc4fa0ca8, /// 0x1d0
			 0x941da02c, /// 0x1d4
			 0xc007cc9e, /// 0x1d8
			 0x8b8e20be, /// 0x1dc
			 0xbb6e164c, /// 0x1e0
			 0xb1faceeb, /// 0x1e4
			 0x607a5d16, /// 0x1e8
			 0xb82a3597, /// 0x1ec
			 0xbeb79046, /// 0x1f0
			 0x076f3c01, /// 0x1f4
			 0x6c9b30ef, /// 0x1f8
			 0xdd243342, /// 0x1fc
			 0x74834f76, /// 0x200
			 0xddb1fd31, /// 0x204
			 0xc1d08e6f, /// 0x208
			 0x9dc88d83, /// 0x20c
			 0xe62ed8bd, /// 0x210
			 0xb63f34c5, /// 0x214
			 0xcae641b4, /// 0x218
			 0x5d534592, /// 0x21c
			 0x02592ff6, /// 0x220
			 0x8750e764, /// 0x224
			 0x068dbd87, /// 0x228
			 0x8cbfa8e3, /// 0x22c
			 0xfae3dce9, /// 0x230
			 0x48831ec9, /// 0x234
			 0xc43accb6, /// 0x238
			 0x83fe3bcf, /// 0x23c
			 0x55690761, /// 0x240
			 0x49cf24d5, /// 0x244
			 0x39a98dea, /// 0x248
			 0x5d1807c7, /// 0x24c
			 0x487884d6, /// 0x250
			 0x14d247f8, /// 0x254
			 0xe4a767aa, /// 0x258
			 0x653bdceb, /// 0x25c
			 0x71ea1be6, /// 0x260
			 0xbc67be16, /// 0x264
			 0x8035c4aa, /// 0x268
			 0xb41ff9b2, /// 0x26c
			 0x9d2d87cc, /// 0x270
			 0x1ff252b9, /// 0x274
			 0xd18f84b7, /// 0x278
			 0x86bc860c, /// 0x27c
			 0x93531a76, /// 0x280
			 0x1c400153, /// 0x284
			 0x693bfde7, /// 0x288
			 0xf49b782b, /// 0x28c
			 0x64084ad7, /// 0x290
			 0x0ca89ab6, /// 0x294
			 0x8200d409, /// 0x298
			 0x7b3f59ce, /// 0x29c
			 0x40cf124a, /// 0x2a0
			 0x989bfe7b, /// 0x2a4
			 0x839d93f9, /// 0x2a8
			 0x28195d9a, /// 0x2ac
			 0x30a0eb51, /// 0x2b0
			 0x3a4712b1, /// 0x2b4
			 0x51bce3d3, /// 0x2b8
			 0xc4a418df, /// 0x2bc
			 0xe07bc0bc, /// 0x2c0
			 0x38351fdb, /// 0x2c4
			 0x8fa8dcee, /// 0x2c8
			 0xc8063d37, /// 0x2cc
			 0x8baea28f, /// 0x2d0
			 0x2b094cd9, /// 0x2d4
			 0x574fc371, /// 0x2d8
			 0x6f8139b8, /// 0x2dc
			 0x534ab7ce, /// 0x2e0
			 0x40204817, /// 0x2e4
			 0x9893a315, /// 0x2e8
			 0x3a711283, /// 0x2ec
			 0xc5786ff6, /// 0x2f0
			 0x2b0ca13f, /// 0x2f4
			 0xd8387261, /// 0x2f8
			 0x46de2e89, /// 0x2fc
			 0xd57d2d7e, /// 0x300
			 0xc1706c50, /// 0x304
			 0x2ce6dee3, /// 0x308
			 0x1e632c8f, /// 0x30c
			 0x901e7cab, /// 0x310
			 0x5e7623d0, /// 0x314
			 0x33439d71, /// 0x318
			 0x43816093, /// 0x31c
			 0x3fc141ae, /// 0x320
			 0xd9db871e, /// 0x324
			 0x9ef59d78, /// 0x328
			 0x7fbc6b5d, /// 0x32c
			 0x680abe08, /// 0x330
			 0x7bb0dc24, /// 0x334
			 0xf48311cc, /// 0x338
			 0x52049bd4, /// 0x33c
			 0xa6d828e0, /// 0x340
			 0x16b4c0d1, /// 0x344
			 0xc9579bf6, /// 0x348
			 0x6a17bb7a, /// 0x34c
			 0x89f36fb0, /// 0x350
			 0x2e4521d5, /// 0x354
			 0xdbeb2c83, /// 0x358
			 0xac2650ae, /// 0x35c
			 0xa8f6e39e, /// 0x360
			 0x650de04d, /// 0x364
			 0x174e8052, /// 0x368
			 0xb6a7c9d8, /// 0x36c
			 0x32d1ff5e, /// 0x370
			 0x68331d1e, /// 0x374
			 0xa55856a0, /// 0x378
			 0x474b6daa, /// 0x37c
			 0x7e1c2289, /// 0x380
			 0x26f17bce, /// 0x384
			 0xe936e37c, /// 0x388
			 0x8833d0a1, /// 0x38c
			 0xaef0833b, /// 0x390
			 0x9796f90c, /// 0x394
			 0xa213c5ea, /// 0x398
			 0xda301f13, /// 0x39c
			 0xd7f5bc44, /// 0x3a0
			 0x4a6ad8ac, /// 0x3a4
			 0x3145b527, /// 0x3a8
			 0x1d19d209, /// 0x3ac
			 0xa6f9b4a3, /// 0x3b0
			 0x43c4077e, /// 0x3b4
			 0xff5722a8, /// 0x3b8
			 0x620f6721, /// 0x3bc
			 0xee2e4477, /// 0x3c0
			 0x6348ab9d, /// 0x3c4
			 0x14ed77f4, /// 0x3c8
			 0xe07712fa, /// 0x3cc
			 0xd67a2cfd, /// 0x3d0
			 0xcb97679c, /// 0x3d4
			 0xe7adb2bd, /// 0x3d8
			 0x98184514, /// 0x3dc
			 0x552ac089, /// 0x3e0
			 0x23db1934, /// 0x3e4
			 0x11ffe550, /// 0x3e8
			 0xb5cd9ef7, /// 0x3ec
			 0x6c395c8c, /// 0x3f0
			 0x1480ae89, /// 0x3f4
			 0x27ce5c9f, /// 0x3f8
			 0x9cf927de, /// 0x3fc
			 0xc59667de, /// 0x400
			 0x8f6ea6cf, /// 0x404
			 0xc9d03650, /// 0x408
			 0x0a8a523b, /// 0x40c
			 0xb23ef38c, /// 0x410
			 0x3461b497, /// 0x414
			 0x92f90e65, /// 0x418
			 0xeeaabd3b, /// 0x41c
			 0x8c665ec1, /// 0x420
			 0xbdcd9a86, /// 0x424
			 0xad2ebdba, /// 0x428
			 0x5d75ca0c, /// 0x42c
			 0xdb042336, /// 0x430
			 0xac2381a2, /// 0x434
			 0x56b4d831, /// 0x438
			 0xd5388abc, /// 0x43c
			 0x62ada3c7, /// 0x440
			 0x18cfdb73, /// 0x444
			 0x2c813f80, /// 0x448
			 0x8351a5ff, /// 0x44c
			 0x6815e60b, /// 0x450
			 0x0321d989, /// 0x454
			 0x4b7db7ff, /// 0x458
			 0x6b391b86, /// 0x45c
			 0x9c90a1de, /// 0x460
			 0x6429f4fd, /// 0x464
			 0xc3068a63, /// 0x468
			 0x0d677c63, /// 0x46c
			 0xcc1816c9, /// 0x470
			 0x234e79ec, /// 0x474
			 0x636b6c43, /// 0x478
			 0x4079e3f7, /// 0x47c
			 0x301de56d, /// 0x480
			 0x0d2c9b73, /// 0x484
			 0xe776ef93, /// 0x488
			 0x51524b02, /// 0x48c
			 0xa8a41002, /// 0x490
			 0x1e6b82e9, /// 0x494
			 0xdb902a7f, /// 0x498
			 0x79ad6e36, /// 0x49c
			 0x8f843b0f, /// 0x4a0
			 0x20d320a6, /// 0x4a4
			 0x3f2fc3cb, /// 0x4a8
			 0x2bb9fe1d, /// 0x4ac
			 0xfeef32ce, /// 0x4b0
			 0x232eaae5, /// 0x4b4
			 0x6dec7d01, /// 0x4b8
			 0xf8a18749, /// 0x4bc
			 0x67d504c9, /// 0x4c0
			 0x6ac137e4, /// 0x4c4
			 0x0ba2a2ed, /// 0x4c8
			 0xd249f8f0, /// 0x4cc
			 0xa7ca3b51, /// 0x4d0
			 0x593441ec, /// 0x4d4
			 0xfffbf9a4, /// 0x4d8
			 0x884bdc2d, /// 0x4dc
			 0xcc09923e, /// 0x4e0
			 0xfd914855, /// 0x4e4
			 0xe57ec172, /// 0x4e8
			 0xb168bd51, /// 0x4ec
			 0xb771143a, /// 0x4f0
			 0x053ca148, /// 0x4f4
			 0x0265fb48, /// 0x4f8
			 0xe1391d97, /// 0x4fc
			 0x9f3891bf, /// 0x500
			 0x699f636c, /// 0x504
			 0x237cdbd3, /// 0x508
			 0xeba029dd, /// 0x50c
			 0xb0fd4025, /// 0x510
			 0x734d192b, /// 0x514
			 0xfe8364b2, /// 0x518
			 0x91c7b99d, /// 0x51c
			 0x5bd12f96, /// 0x520
			 0x288d7dce, /// 0x524
			 0xb3196e6a, /// 0x528
			 0x03de7adf, /// 0x52c
			 0x8e4bc83d, /// 0x530
			 0x4e17dcb7, /// 0x534
			 0xb3b54138, /// 0x538
			 0x32e8954e, /// 0x53c
			 0x09264f70, /// 0x540
			 0x145c700d, /// 0x544
			 0xcb762b13, /// 0x548
			 0xa85e34a7, /// 0x54c
			 0x6cc13eb4, /// 0x550
			 0x166d01c0, /// 0x554
			 0xbe0a3beb, /// 0x558
			 0x494214e1, /// 0x55c
			 0x38b0d4b1, /// 0x560
			 0x7a28f5b4, /// 0x564
			 0x7d978979, /// 0x568
			 0xb9a11a04, /// 0x56c
			 0xdbb6744f, /// 0x570
			 0x7f290331, /// 0x574
			 0x59c2e66b, /// 0x578
			 0x89d6302c, /// 0x57c
			 0x134a9468, /// 0x580
			 0xf5117923, /// 0x584
			 0xf17b004c, /// 0x588
			 0x7ef3e15c, /// 0x58c
			 0x578177a3, /// 0x590
			 0x52a84320, /// 0x594
			 0x246b4737, /// 0x598
			 0xf36fd966, /// 0x59c
			 0x78c30132, /// 0x5a0
			 0x948cf277, /// 0x5a4
			 0x00583353, /// 0x5a8
			 0xf444ea8c, /// 0x5ac
			 0x58273cc2, /// 0x5b0
			 0x36f0811a, /// 0x5b4
			 0xf33f8a3b, /// 0x5b8
			 0xc48ea2dd, /// 0x5bc
			 0x1b393446, /// 0x5c0
			 0x8283ca59, /// 0x5c4
			 0x7494c1ef, /// 0x5c8
			 0x4597c6b1, /// 0x5cc
			 0xcba3098e, /// 0x5d0
			 0xd8290975, /// 0x5d4
			 0x0df31fe6, /// 0x5d8
			 0x4bd3d748, /// 0x5dc
			 0x88d275cb, /// 0x5e0
			 0xbc575add, /// 0x5e4
			 0x1a051b19, /// 0x5e8
			 0xeaae38fe, /// 0x5ec
			 0xb3e133ff, /// 0x5f0
			 0xeb3fe60a, /// 0x5f4
			 0x33f57ac3, /// 0x5f8
			 0x71b51f76, /// 0x5fc
			 0xf18bcb1f, /// 0x600
			 0x3d949953, /// 0x604
			 0x032f5db8, /// 0x608
			 0x4cf17b16, /// 0x60c
			 0x64b60054, /// 0x610
			 0x8abdc83d, /// 0x614
			 0x073c0a19, /// 0x618
			 0x78f75c1f, /// 0x61c
			 0x916c0e2c, /// 0x620
			 0x7cb3f8ca, /// 0x624
			 0x3ae32c6d, /// 0x628
			 0x6b8f6649, /// 0x62c
			 0x612efedd, /// 0x630
			 0x6c07615b, /// 0x634
			 0x9c0720d3, /// 0x638
			 0x60995c9b, /// 0x63c
			 0xb3705fbc, /// 0x640
			 0x8ded92be, /// 0x644
			 0x464b6088, /// 0x648
			 0x9625bbfb, /// 0x64c
			 0x72bd0a3b, /// 0x650
			 0x589ffeb8, /// 0x654
			 0x7a4415f7, /// 0x658
			 0x75a221d6, /// 0x65c
			 0x70d73a72, /// 0x660
			 0x723e4696, /// 0x664
			 0xa7677220, /// 0x668
			 0x82adb9d9, /// 0x66c
			 0x1940de08, /// 0x670
			 0xf97a9cfc, /// 0x674
			 0xddc0d80c, /// 0x678
			 0xa50a7e2e, /// 0x67c
			 0xfa0805fb, /// 0x680
			 0x2aae5a99, /// 0x684
			 0x69d7b3da, /// 0x688
			 0x14a6b94c, /// 0x68c
			 0xa8175d0d, /// 0x690
			 0x780d6578, /// 0x694
			 0x557b9c6c, /// 0x698
			 0x5d507174, /// 0x69c
			 0xd4bc4b10, /// 0x6a0
			 0x128ebc13, /// 0x6a4
			 0xe14873d7, /// 0x6a8
			 0xd6a35fd2, /// 0x6ac
			 0xb7c4d791, /// 0x6b0
			 0x687744d2, /// 0x6b4
			 0x123d8fd1, /// 0x6b8
			 0x401b9efd, /// 0x6bc
			 0xa890dbda, /// 0x6c0
			 0xaca9deaa, /// 0x6c4
			 0x3744b033, /// 0x6c8
			 0x7cb9c4ec, /// 0x6cc
			 0x99b21eca, /// 0x6d0
			 0x6b48041d, /// 0x6d4
			 0xaa999a70, /// 0x6d8
			 0x433f60fe, /// 0x6dc
			 0xbea0fd9a, /// 0x6e0
			 0xbb0b4da3, /// 0x6e4
			 0xc7d4d1e1, /// 0x6e8
			 0x71219e31, /// 0x6ec
			 0x4b5f56a7, /// 0x6f0
			 0xcafc779f, /// 0x6f4
			 0x858e01c1, /// 0x6f8
			 0x6b4ce277, /// 0x6fc
			 0xa7a8d1f1, /// 0x700
			 0x95c07d28, /// 0x704
			 0x3111ffc9, /// 0x708
			 0x4e6c4f71, /// 0x70c
			 0xc85b202f, /// 0x710
			 0x66e3f9b6, /// 0x714
			 0x072b8ab7, /// 0x718
			 0xfa3dc862, /// 0x71c
			 0xcca83b1e, /// 0x720
			 0x56ab148b, /// 0x724
			 0x24fdff08, /// 0x728
			 0xdcc0f499, /// 0x72c
			 0xb3238ce8, /// 0x730
			 0x3496c2a0, /// 0x734
			 0x249cce26, /// 0x738
			 0x1f7b8be9, /// 0x73c
			 0xcf1bb0be, /// 0x740
			 0x1abec6da, /// 0x744
			 0x55ededf2, /// 0x748
			 0xff6bf191, /// 0x74c
			 0x4cf6fa62, /// 0x750
			 0xdcdeada1, /// 0x754
			 0x83046d75, /// 0x758
			 0x69bd9faa, /// 0x75c
			 0x6caf401b, /// 0x760
			 0xa7fc0edf, /// 0x764
			 0x6e4d2db6, /// 0x768
			 0x16318a07, /// 0x76c
			 0x446033be, /// 0x770
			 0x524d7c67, /// 0x774
			 0x3c210907, /// 0x778
			 0x104a7902, /// 0x77c
			 0x4318c505, /// 0x780
			 0x87cc760f, /// 0x784
			 0x8815e0b6, /// 0x788
			 0x92d965a3, /// 0x78c
			 0xcb4e55a6, /// 0x790
			 0xc3a4ab66, /// 0x794
			 0x86dcf5f9, /// 0x798
			 0x327cf8bc, /// 0x79c
			 0x43258548, /// 0x7a0
			 0x54037486, /// 0x7a4
			 0x674ded88, /// 0x7a8
			 0x6bdbd4bf, /// 0x7ac
			 0x76569edf, /// 0x7b0
			 0x0902f7d4, /// 0x7b4
			 0x543e2fba, /// 0x7b8
			 0x773ecd10, /// 0x7bc
			 0xeff126e6, /// 0x7c0
			 0x3628daf0, /// 0x7c4
			 0x0e1dbfcd, /// 0x7c8
			 0xd76fd8e5, /// 0x7cc
			 0x2fd6324d, /// 0x7d0
			 0x7b80697b, /// 0x7d4
			 0x4ef13067, /// 0x7d8
			 0x8de24f9a, /// 0x7dc
			 0xba788e63, /// 0x7e0
			 0xc4ffe75a, /// 0x7e4
			 0x2b53f335, /// 0x7e8
			 0x5410a48d, /// 0x7ec
			 0xc4c00475, /// 0x7f0
			 0x65e9de41, /// 0x7f4
			 0x7bf5f721, /// 0x7f8
			 0x2a7c321a, /// 0x7fc
			 0x45d9e410, /// 0x800
			 0xb80bed3f, /// 0x804
			 0x091cd6ba, /// 0x808
			 0xaae0ffee, /// 0x80c
			 0x7199c619, /// 0x810
			 0x661bf4dd, /// 0x814
			 0x7d293680, /// 0x818
			 0x2ed66998, /// 0x81c
			 0x98aebe78, /// 0x820
			 0x024dd8f7, /// 0x824
			 0xc88eb967, /// 0x828
			 0xd418dce6, /// 0x82c
			 0xb5dbf55f, /// 0x830
			 0xaf8e2efd, /// 0x834
			 0x6876fe07, /// 0x838
			 0x6fc64634, /// 0x83c
			 0x4c8de1f4, /// 0x840
			 0x64e07752, /// 0x844
			 0x83c59a9a, /// 0x848
			 0x1d44c616, /// 0x84c
			 0x958b39c3, /// 0x850
			 0xd4c5428e, /// 0x854
			 0xf775dabf, /// 0x858
			 0x47e29f4f, /// 0x85c
			 0xdfcf8f1f, /// 0x860
			 0xb682c280, /// 0x864
			 0x66d5bc56, /// 0x868
			 0x6f8c212c, /// 0x86c
			 0xc5484ebe, /// 0x870
			 0x03a9db1b, /// 0x874
			 0x3ab1f994, /// 0x878
			 0xf315f24d, /// 0x87c
			 0x0fae2d50, /// 0x880
			 0xfa13134e, /// 0x884
			 0xa9f83895, /// 0x888
			 0xa556f2ff, /// 0x88c
			 0xfb9e0215, /// 0x890
			 0x8bd914d4, /// 0x894
			 0x6b15069f, /// 0x898
			 0xc7849720, /// 0x89c
			 0x6ca8f810, /// 0x8a0
			 0x7ffe8e11, /// 0x8a4
			 0x9808cfc8, /// 0x8a8
			 0x40cd6685, /// 0x8ac
			 0xa5a2ba59, /// 0x8b0
			 0xc2497f93, /// 0x8b4
			 0xfa0e0a3d, /// 0x8b8
			 0x2bc4ca2a, /// 0x8bc
			 0xc6632616, /// 0x8c0
			 0xf96221d7, /// 0x8c4
			 0x67ba90a7, /// 0x8c8
			 0x61b589bd, /// 0x8cc
			 0x71166232, /// 0x8d0
			 0x331a8d2c, /// 0x8d4
			 0x33f5d138, /// 0x8d8
			 0x2619769f, /// 0x8dc
			 0xefd3255b, /// 0x8e0
			 0xb4ea39c5, /// 0x8e4
			 0xecae66f9, /// 0x8e8
			 0xeaa6b22f, /// 0x8ec
			 0xb1e5704b, /// 0x8f0
			 0x2ba511c7, /// 0x8f4
			 0x2d8c6214, /// 0x8f8
			 0x180dab76, /// 0x8fc
			 0xdc61073e, /// 0x900
			 0x0cbbb5e3, /// 0x904
			 0xc27b0c34, /// 0x908
			 0xaa179b49, /// 0x90c
			 0xc37db575, /// 0x910
			 0xc0f46d28, /// 0x914
			 0x0c025215, /// 0x918
			 0x13d870e8, /// 0x91c
			 0x1224f642, /// 0x920
			 0xeb58216c, /// 0x924
			 0x10209330, /// 0x928
			 0x230310e4, /// 0x92c
			 0x55da86bd, /// 0x930
			 0x059623ee, /// 0x934
			 0x3a787cf7, /// 0x938
			 0x64f33465, /// 0x93c
			 0xda443017, /// 0x940
			 0x42475eae, /// 0x944
			 0x4132764a, /// 0x948
			 0x3cd19555, /// 0x94c
			 0x7d9a9ba4, /// 0x950
			 0x0bf41fc8, /// 0x954
			 0x474fd1f6, /// 0x958
			 0x2744314c, /// 0x95c
			 0xedcd8f59, /// 0x960
			 0x32f9ed9c, /// 0x964
			 0x20329530, /// 0x968
			 0x43bed5d5, /// 0x96c
			 0x63058e8e, /// 0x970
			 0xf86c711d, /// 0x974
			 0x143fa83b, /// 0x978
			 0x6b3ef181, /// 0x97c
			 0xe5afce0a, /// 0x980
			 0xb69579ab, /// 0x984
			 0x81d911cf, /// 0x988
			 0x530ae639, /// 0x98c
			 0x9160ca71, /// 0x990
			 0xbb33cfd9, /// 0x994
			 0x1bb1e6e5, /// 0x998
			 0xcec005e0, /// 0x99c
			 0x6336d33a, /// 0x9a0
			 0xe37317e9, /// 0x9a4
			 0x327132c3, /// 0x9a8
			 0x5011297a, /// 0x9ac
			 0x5d26eac9, /// 0x9b0
			 0x7d4b1c63, /// 0x9b4
			 0x3fea7b29, /// 0x9b8
			 0x3cbe1216, /// 0x9bc
			 0x4565f9f4, /// 0x9c0
			 0xe2929492, /// 0x9c4
			 0x31663224, /// 0x9c8
			 0xea5178c3, /// 0x9cc
			 0x36468086, /// 0x9d0
			 0xabf1be2f, /// 0x9d4
			 0x4898ae75, /// 0x9d8
			 0x32a1c493, /// 0x9dc
			 0xce07a5d3, /// 0x9e0
			 0xc659bdbd, /// 0x9e4
			 0xec4a32d9, /// 0x9e8
			 0x7f4fa191, /// 0x9ec
			 0x173ce669, /// 0x9f0
			 0x6a61fca5, /// 0x9f4
			 0xda403308, /// 0x9f8
			 0x72d1b82a, /// 0x9fc
			 0xfe0fdecd, /// 0xa00
			 0x7859ffc2, /// 0xa04
			 0x29644984, /// 0xa08
			 0xc0ba34db, /// 0xa0c
			 0xf4e116a7, /// 0xa10
			 0xa552293e, /// 0xa14
			 0xdb127249, /// 0xa18
			 0x074e717b, /// 0xa1c
			 0x0df0104d, /// 0xa20
			 0xd7e72f9c, /// 0xa24
			 0xe4b3f690, /// 0xa28
			 0xdc12cff1, /// 0xa2c
			 0x3cf120b7, /// 0xa30
			 0x23960590, /// 0xa34
			 0xf3e5385c, /// 0xa38
			 0x59f90b73, /// 0xa3c
			 0x5a011632, /// 0xa40
			 0x874d2d5f, /// 0xa44
			 0xe81b57f1, /// 0xa48
			 0x998ba25d, /// 0xa4c
			 0xfd873f82, /// 0xa50
			 0xfaf75ef5, /// 0xa54
			 0xf608360d, /// 0xa58
			 0xce6d1be0, /// 0xa5c
			 0xd9916670, /// 0xa60
			 0xe850fcce, /// 0xa64
			 0x8b4c3667, /// 0xa68
			 0xbf7bb7f4, /// 0xa6c
			 0xcaabfb15, /// 0xa70
			 0x4085ca97, /// 0xa74
			 0xe40df964, /// 0xa78
			 0x64afc603, /// 0xa7c
			 0x2b49cb94, /// 0xa80
			 0x8b1a3ed8, /// 0xa84
			 0xa9f742e3, /// 0xa88
			 0xec03075d, /// 0xa8c
			 0x68c9207b, /// 0xa90
			 0xf3b4c195, /// 0xa94
			 0x8ec631aa, /// 0xa98
			 0xeb6c5588, /// 0xa9c
			 0x9516841a, /// 0xaa0
			 0x9137cc84, /// 0xaa4
			 0x10304975, /// 0xaa8
			 0xdfa7d205, /// 0xaac
			 0xf936346f, /// 0xab0
			 0xc7e5f707, /// 0xab4
			 0xca207457, /// 0xab8
			 0xf840a988, /// 0xabc
			 0x6e926db7, /// 0xac0
			 0xf18636f8, /// 0xac4
			 0xcb9b9c30, /// 0xac8
			 0x1b88a85f, /// 0xacc
			 0x0fc7394d, /// 0xad0
			 0xe1476639, /// 0xad4
			 0x9808e99e, /// 0xad8
			 0xf3d02036, /// 0xadc
			 0xdf541e75, /// 0xae0
			 0xd0b0a55d, /// 0xae4
			 0xea41004a, /// 0xae8
			 0x18595c47, /// 0xaec
			 0x8ad157e8, /// 0xaf0
			 0xd50fc1fa, /// 0xaf4
			 0x56e47558, /// 0xaf8
			 0x34cf12d6, /// 0xafc
			 0x8b9117c0, /// 0xb00
			 0x43c5c951, /// 0xb04
			 0xcc1608e3, /// 0xb08
			 0xc928f19b, /// 0xb0c
			 0x94596939, /// 0xb10
			 0x3875f6d5, /// 0xb14
			 0x8136f12c, /// 0xb18
			 0x1ee69aca, /// 0xb1c
			 0xf60548b5, /// 0xb20
			 0x020d3554, /// 0xb24
			 0x826dec50, /// 0xb28
			 0xc8b1910c, /// 0xb2c
			 0x2998486d, /// 0xb30
			 0x0840de35, /// 0xb34
			 0x6fd8c231, /// 0xb38
			 0xc75e79c1, /// 0xb3c
			 0x6a867c20, /// 0xb40
			 0x6b9963a7, /// 0xb44
			 0x4f5bdf32, /// 0xb48
			 0xe68380d9, /// 0xb4c
			 0xca1045c1, /// 0xb50
			 0x966cfb81, /// 0xb54
			 0x439ed8a2, /// 0xb58
			 0xcecc37cf, /// 0xb5c
			 0xa245414a, /// 0xb60
			 0xd24c51d5, /// 0xb64
			 0xe8f72268, /// 0xb68
			 0xfe29631a, /// 0xb6c
			 0x847f7eec, /// 0xb70
			 0x1a8e439a, /// 0xb74
			 0xb71a2352, /// 0xb78
			 0xc29b8e63, /// 0xb7c
			 0xad841dcc, /// 0xb80
			 0xf72c4cf9, /// 0xb84
			 0x0d066196, /// 0xb88
			 0x851eb1ef, /// 0xb8c
			 0xefdbf557, /// 0xb90
			 0x8367ebfe, /// 0xb94
			 0xd2f8208a, /// 0xb98
			 0xcf80ec43, /// 0xb9c
			 0x6381b803, /// 0xba0
			 0xe6e7f550, /// 0xba4
			 0xc26b6756, /// 0xba8
			 0x838aafba, /// 0xbac
			 0xbd4af11c, /// 0xbb0
			 0xc9bdbe69, /// 0xbb4
			 0x88a270f5, /// 0xbb8
			 0x1dcf8fbe, /// 0xbbc
			 0x892ff389, /// 0xbc0
			 0xdcd7c432, /// 0xbc4
			 0x2f34a0ab, /// 0xbc8
			 0x5b8a8b17, /// 0xbcc
			 0xf3d6493d, /// 0xbd0
			 0x913dc79e, /// 0xbd4
			 0xe30a14c8, /// 0xbd8
			 0xc208e3ee, /// 0xbdc
			 0xbf537b0e, /// 0xbe0
			 0xdd46260c, /// 0xbe4
			 0x6209aa78, /// 0xbe8
			 0x09b52909, /// 0xbec
			 0x4a166663, /// 0xbf0
			 0x346d0000, /// 0xbf4
			 0x995259fe, /// 0xbf8
			 0x0a77f4cf, /// 0xbfc
			 0x21b89f5b, /// 0xc00
			 0x48543ce7, /// 0xc04
			 0xba7396e5, /// 0xc08
			 0x22cc5c91, /// 0xc0c
			 0x60e53611, /// 0xc10
			 0xe09a374d, /// 0xc14
			 0x6e6c57d4, /// 0xc18
			 0xb0decfc4, /// 0xc1c
			 0x60bda782, /// 0xc20
			 0x3799c342, /// 0xc24
			 0xc2e1d5b9, /// 0xc28
			 0x2bf4a154, /// 0xc2c
			 0x2d2dd664, /// 0xc30
			 0xc66e3e67, /// 0xc34
			 0x55c2a3c5, /// 0xc38
			 0xa01a80f3, /// 0xc3c
			 0x0fa7a8b2, /// 0xc40
			 0x97e04ce5, /// 0xc44
			 0x5697b4b2, /// 0xc48
			 0x9c5a278d, /// 0xc4c
			 0x05e949c3, /// 0xc50
			 0x49a21045, /// 0xc54
			 0x1ee94665, /// 0xc58
			 0xe7df0a45, /// 0xc5c
			 0x279f9af8, /// 0xc60
			 0x0930f4be, /// 0xc64
			 0x83660124, /// 0xc68
			 0xe848f806, /// 0xc6c
			 0xe29dfc30, /// 0xc70
			 0x7b6150ca, /// 0xc74
			 0xdf59f6ef, /// 0xc78
			 0x3aab8b58, /// 0xc7c
			 0x89185f85, /// 0xc80
			 0xe73b9768, /// 0xc84
			 0x3802f159, /// 0xc88
			 0x0ca63146, /// 0xc8c
			 0x090be178, /// 0xc90
			 0xd4b63a20, /// 0xc94
			 0x170d1b01, /// 0xc98
			 0xe1646925, /// 0xc9c
			 0xe5ac4e82, /// 0xca0
			 0x767827b5, /// 0xca4
			 0x440096bc, /// 0xca8
			 0x08f9f0c6, /// 0xcac
			 0x92961af0, /// 0xcb0
			 0xb81a78f5, /// 0xcb4
			 0x292d2a3e, /// 0xcb8
			 0x1b697879, /// 0xcbc
			 0x8a8c9749, /// 0xcc0
			 0x285ccb06, /// 0xcc4
			 0xb48570f6, /// 0xcc8
			 0xff5436d0, /// 0xccc
			 0x8dbf436b, /// 0xcd0
			 0x92d90ac7, /// 0xcd4
			 0xf4b6c786, /// 0xcd8
			 0x519b87c7, /// 0xcdc
			 0xbf1cd437, /// 0xce0
			 0x6f403757, /// 0xce4
			 0x0f8ddfac, /// 0xce8
			 0x063317bf, /// 0xcec
			 0x86d9c3f5, /// 0xcf0
			 0x76e7c8df, /// 0xcf4
			 0x2fbdfb57, /// 0xcf8
			 0xb389194a, /// 0xcfc
			 0x1290fe23, /// 0xd00
			 0xf1f2ec69, /// 0xd04
			 0x8293d821, /// 0xd08
			 0x8d134d84, /// 0xd0c
			 0x89ceeddb, /// 0xd10
			 0xc9676393, /// 0xd14
			 0xdd203969, /// 0xd18
			 0xf344d5f6, /// 0xd1c
			 0x4ef44ffc, /// 0xd20
			 0x9b452620, /// 0xd24
			 0x140a069a, /// 0xd28
			 0xe2a05995, /// 0xd2c
			 0x109d84b5, /// 0xd30
			 0xc88d25c8, /// 0xd34
			 0xe17d2ca2, /// 0xd38
			 0x68cd922d, /// 0xd3c
			 0x92950c71, /// 0xd40
			 0x7dac81ec, /// 0xd44
			 0x8973f8db, /// 0xd48
			 0x0796ed08, /// 0xd4c
			 0x81239d92, /// 0xd50
			 0x2587c405, /// 0xd54
			 0x5c27e6d4, /// 0xd58
			 0xa53c2e4a, /// 0xd5c
			 0x0d5fa436, /// 0xd60
			 0xf28061ae, /// 0xd64
			 0xf29f51f0, /// 0xd68
			 0x350f9c6d, /// 0xd6c
			 0x50ccbdc0, /// 0xd70
			 0xaddfde2a, /// 0xd74
			 0x169f367e, /// 0xd78
			 0x58c4a3b0, /// 0xd7c
			 0xa0c137c7, /// 0xd80
			 0xdf7c8bdc, /// 0xd84
			 0xc2eeb710, /// 0xd88
			 0xd6b0d865, /// 0xd8c
			 0x8e40d7eb, /// 0xd90
			 0xc519e393, /// 0xd94
			 0xda2b5c0a, /// 0xd98
			 0xdcde1e71, /// 0xd9c
			 0xeecac214, /// 0xda0
			 0x6928dcf8, /// 0xda4
			 0xe376f273, /// 0xda8
			 0x1b3a5a58, /// 0xdac
			 0xfc5028d3, /// 0xdb0
			 0x986095e8, /// 0xdb4
			 0x4f0ad8ef, /// 0xdb8
			 0xb573e030, /// 0xdbc
			 0x8ee9035a, /// 0xdc0
			 0xfd615265, /// 0xdc4
			 0xd551b647, /// 0xdc8
			 0xef57f470, /// 0xdcc
			 0x81f46e54, /// 0xdd0
			 0xff11f5a6, /// 0xdd4
			 0xa940e24b, /// 0xdd8
			 0x05a6d3b3, /// 0xddc
			 0x20776c18, /// 0xde0
			 0x6fa88b0e, /// 0xde4
			 0x3c03e2f8, /// 0xde8
			 0x43f4b5d5, /// 0xdec
			 0x66e283e4, /// 0xdf0
			 0x53157a77, /// 0xdf4
			 0x4397320a, /// 0xdf8
			 0x3f1fbfdd, /// 0xdfc
			 0x46901f8b, /// 0xe00
			 0xce363085, /// 0xe04
			 0x3d18e1ed, /// 0xe08
			 0xf4fdaf17, /// 0xe0c
			 0x550e273f, /// 0xe10
			 0x426dc386, /// 0xe14
			 0xaa75c14e, /// 0xe18
			 0x072f405a, /// 0xe1c
			 0xd0df0126, /// 0xe20
			 0x00a490ca, /// 0xe24
			 0x864cf2e9, /// 0xe28
			 0x35be2f9c, /// 0xe2c
			 0x972f46f0, /// 0xe30
			 0xf717523b, /// 0xe34
			 0xdd030fd3, /// 0xe38
			 0xd3ab0337, /// 0xe3c
			 0xf3a280ba, /// 0xe40
			 0xe8bb835f, /// 0xe44
			 0x0876f3e6, /// 0xe48
			 0x74bc862d, /// 0xe4c
			 0xa46487cc, /// 0xe50
			 0x4ae2afd8, /// 0xe54
			 0x571f86af, /// 0xe58
			 0x8184f224, /// 0xe5c
			 0x8695e306, /// 0xe60
			 0x3bf24d3f, /// 0xe64
			 0x8d6a078b, /// 0xe68
			 0xe2611af9, /// 0xe6c
			 0x7694ee18, /// 0xe70
			 0xb59d9921, /// 0xe74
			 0x3654f097, /// 0xe78
			 0x672091ed, /// 0xe7c
			 0x58f35c75, /// 0xe80
			 0x2df6496f, /// 0xe84
			 0xb5324027, /// 0xe88
			 0x9cf0f940, /// 0xe8c
			 0x907d8b55, /// 0xe90
			 0xc708e7b5, /// 0xe94
			 0x20def1b0, /// 0xe98
			 0x11556663, /// 0xe9c
			 0x1d1490a1, /// 0xea0
			 0xd00eb76d, /// 0xea4
			 0x69498d65, /// 0xea8
			 0xad0d2d7e, /// 0xeac
			 0x5e6f5a0f, /// 0xeb0
			 0x4a2a4d68, /// 0xeb4
			 0x39244107, /// 0xeb8
			 0x01a76326, /// 0xebc
			 0x5699685b, /// 0xec0
			 0xe6938df3, /// 0xec4
			 0x96ad5e7e, /// 0xec8
			 0xf0390d7c, /// 0xecc
			 0x8f484de7, /// 0xed0
			 0xe526b465, /// 0xed4
			 0xb5f689d5, /// 0xed8
			 0x5b309917, /// 0xedc
			 0xc6abb33d, /// 0xee0
			 0xc0ca1899, /// 0xee4
			 0xe6c0f073, /// 0xee8
			 0x9e52aef3, /// 0xeec
			 0x4cac7e99, /// 0xef0
			 0x0a178511, /// 0xef4
			 0x1544183b, /// 0xef8
			 0xc9b68937, /// 0xefc
			 0xa6b3bcfb, /// 0xf00
			 0xe9f26370, /// 0xf04
			 0x2649443e, /// 0xf08
			 0xeaae4c98, /// 0xf0c
			 0x2d736b80, /// 0xf10
			 0x2d2c2901, /// 0xf14
			 0xf7aa3bc3, /// 0xf18
			 0xbe6292c7, /// 0xf1c
			 0x41573e21, /// 0xf20
			 0x8129e3ea, /// 0xf24
			 0x19f131c9, /// 0xf28
			 0xfc688cf1, /// 0xf2c
			 0xd820838d, /// 0xf30
			 0xeec4734b, /// 0xf34
			 0x6239258a, /// 0xf38
			 0x3a776bc2, /// 0xf3c
			 0xf435d8e9, /// 0xf40
			 0xf277c162, /// 0xf44
			 0x1a034ba4, /// 0xf48
			 0xd9be13f0, /// 0xf4c
			 0x5c4d5af0, /// 0xf50
			 0x701cb068, /// 0xf54
			 0xbebd4c24, /// 0xf58
			 0x3f53959e, /// 0xf5c
			 0xf80a4855, /// 0xf60
			 0xa5bd5329, /// 0xf64
			 0xf73183a4, /// 0xf68
			 0xa4dc7aa3, /// 0xf6c
			 0x1f70ed60, /// 0xf70
			 0x8fc388b0, /// 0xf74
			 0xe57db84a, /// 0xf78
			 0xd6a7cec3, /// 0xf7c
			 0xdd362e06, /// 0xf80
			 0x9492ea80, /// 0xf84
			 0x309c9ede, /// 0xf88
			 0xbca08de0, /// 0xf8c
			 0xf8ade07d, /// 0xf90
			 0x83a0d9b1, /// 0xf94
			 0x2df4e36c, /// 0xf98
			 0x9c647a23, /// 0xf9c
			 0x8b49339e, /// 0xfa0
			 0xec420010, /// 0xfa4
			 0x2c4d9217, /// 0xfa8
			 0x084fc99a, /// 0xfac
			 0x55dca59d, /// 0xfb0
			 0x6c0b8ee9, /// 0xfb4
			 0x1daa0d1a, /// 0xfb8
			 0x637a6e0c, /// 0xfbc
			 0x763bb87c, /// 0xfc0
			 0x245a4416, /// 0xfc4
			 0x394fd452, /// 0xfc8
			 0xac829986, /// 0xfcc
			 0xd85bfa9e, /// 0xfd0
			 0x2c7a7b75, /// 0xfd4
			 0x38283468, /// 0xfd8
			 0x81db0f86, /// 0xfdc
			 0x4efdc895, /// 0xfe0
			 0x6bf50056, /// 0xfe4
			 0x266267ca, /// 0xfe8
			 0x8dd2bb51, /// 0xfec
			 0x9161f032, /// 0xff0
			 0x673be4d2, /// 0xff4
			 0xaaca41e7, /// 0xff8
			 0xb3d94d96 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x3a4ad8eb, /// 0x0
			 0x7fa37dd4, /// 0x4
			 0x1dc57e3d, /// 0x8
			 0x9667f414, /// 0xc
			 0x12783a8a, /// 0x10
			 0xb2e0d7f2, /// 0x14
			 0x0e718a98, /// 0x18
			 0xac71ea4a, /// 0x1c
			 0x13a135df, /// 0x20
			 0xffa3a6f0, /// 0x24
			 0xca8271e9, /// 0x28
			 0x905b8909, /// 0x2c
			 0xdc171f26, /// 0x30
			 0x2279faae, /// 0x34
			 0xa97a419d, /// 0x38
			 0xae9fe24b, /// 0x3c
			 0x2fe4ace6, /// 0x40
			 0xedbb0448, /// 0x44
			 0x83ad5d71, /// 0x48
			 0xf89226f6, /// 0x4c
			 0x2f6b58bd, /// 0x50
			 0xe4fb0280, /// 0x54
			 0xef5dd2d4, /// 0x58
			 0x8d1c379e, /// 0x5c
			 0x94a9b26b, /// 0x60
			 0xe6e32ea9, /// 0x64
			 0xb3156632, /// 0x68
			 0x85d36590, /// 0x6c
			 0xf3fdb263, /// 0x70
			 0xda089386, /// 0x74
			 0xc0b06fa3, /// 0x78
			 0x8983137f, /// 0x7c
			 0x902af2f4, /// 0x80
			 0xf91a0af5, /// 0x84
			 0x1aabdbd2, /// 0x88
			 0x896797cc, /// 0x8c
			 0x7c436adb, /// 0x90
			 0x14016cc2, /// 0x94
			 0xc399e161, /// 0x98
			 0xa5e3635c, /// 0x9c
			 0xe66f36b4, /// 0xa0
			 0x3217cd2e, /// 0xa4
			 0x6e3689fb, /// 0xa8
			 0x1088a566, /// 0xac
			 0x9ec6a4dd, /// 0xb0
			 0x03f21fef, /// 0xb4
			 0x1ec639dc, /// 0xb8
			 0x02fb3c08, /// 0xbc
			 0x9446e8a4, /// 0xc0
			 0x970d4457, /// 0xc4
			 0xf8aa34d1, /// 0xc8
			 0x19bb05fa, /// 0xcc
			 0x288e3027, /// 0xd0
			 0x2ad5e24d, /// 0xd4
			 0x1845dab6, /// 0xd8
			 0x74d9a63a, /// 0xdc
			 0xd45618ba, /// 0xe0
			 0x4b7bf4cd, /// 0xe4
			 0x4d8c9cd1, /// 0xe8
			 0x796eb648, /// 0xec
			 0x5db11bd1, /// 0xf0
			 0xb50c1d7e, /// 0xf4
			 0x0801f40e, /// 0xf8
			 0xbec563af, /// 0xfc
			 0x39d7b854, /// 0x100
			 0x2e70e591, /// 0x104
			 0x7393a0aa, /// 0x108
			 0x44f64b48, /// 0x10c
			 0x6d0001c6, /// 0x110
			 0x9c8886b1, /// 0x114
			 0x4ee53ed4, /// 0x118
			 0x45f554a8, /// 0x11c
			 0x7cc552cd, /// 0x120
			 0x5eac253f, /// 0x124
			 0x14f1dc50, /// 0x128
			 0x8ca0e72d, /// 0x12c
			 0xab49187b, /// 0x130
			 0x603d72bb, /// 0x134
			 0x86f5d6cf, /// 0x138
			 0x1e183b53, /// 0x13c
			 0x7a476866, /// 0x140
			 0x4eccc232, /// 0x144
			 0x257400d0, /// 0x148
			 0x91dac930, /// 0x14c
			 0xa346e9ea, /// 0x150
			 0x6087c935, /// 0x154
			 0xdcd8e0d8, /// 0x158
			 0x9dab7f07, /// 0x15c
			 0xd1fa7251, /// 0x160
			 0xd4a60eb7, /// 0x164
			 0xb9ee4867, /// 0x168
			 0x9b775989, /// 0x16c
			 0xf51ea410, /// 0x170
			 0xadaf0474, /// 0x174
			 0x48a76794, /// 0x178
			 0x4c5e23a2, /// 0x17c
			 0xa59384c2, /// 0x180
			 0x346801bb, /// 0x184
			 0x37cce4b4, /// 0x188
			 0x0a4e5a1f, /// 0x18c
			 0x80792c0e, /// 0x190
			 0xa67a577f, /// 0x194
			 0x6c487fa0, /// 0x198
			 0xbf32afd1, /// 0x19c
			 0x719af73e, /// 0x1a0
			 0xb4442dd5, /// 0x1a4
			 0xc1456915, /// 0x1a8
			 0xe72524ca, /// 0x1ac
			 0x46f6fb37, /// 0x1b0
			 0xf897c4dd, /// 0x1b4
			 0xa1d79656, /// 0x1b8
			 0x7cae77cd, /// 0x1bc
			 0x37ae60db, /// 0x1c0
			 0x28cb8887, /// 0x1c4
			 0x8aa8f2e3, /// 0x1c8
			 0x7409f0bc, /// 0x1cc
			 0x4c31cb29, /// 0x1d0
			 0xce9f5dd9, /// 0x1d4
			 0x591d8d02, /// 0x1d8
			 0xa23f888a, /// 0x1dc
			 0xb2edb544, /// 0x1e0
			 0xfddb531c, /// 0x1e4
			 0xb6d15311, /// 0x1e8
			 0xe3cdcc55, /// 0x1ec
			 0xb6bfd634, /// 0x1f0
			 0x73d7e013, /// 0x1f4
			 0x78fb7d3a, /// 0x1f8
			 0x02ad1941, /// 0x1fc
			 0x42a78612, /// 0x200
			 0x36a64e69, /// 0x204
			 0xe9802140, /// 0x208
			 0x5f9c6c03, /// 0x20c
			 0xe9651ac7, /// 0x210
			 0xdb60120f, /// 0x214
			 0x3723c5a3, /// 0x218
			 0x2b66ac97, /// 0x21c
			 0xfcb68fc1, /// 0x220
			 0xadcd7915, /// 0x224
			 0xa40088a1, /// 0x228
			 0x7bfad8cb, /// 0x22c
			 0x465c5c01, /// 0x230
			 0xd272df48, /// 0x234
			 0x05043215, /// 0x238
			 0x126560d2, /// 0x23c
			 0xfb0c4ef8, /// 0x240
			 0x2a659869, /// 0x244
			 0x7b2addfd, /// 0x248
			 0xdc1b56f2, /// 0x24c
			 0x73036b33, /// 0x250
			 0x9018ab48, /// 0x254
			 0x0595976e, /// 0x258
			 0x6dfa22c5, /// 0x25c
			 0x980120f5, /// 0x260
			 0x401262f9, /// 0x264
			 0x06e86879, /// 0x268
			 0x72b72504, /// 0x26c
			 0x9cadb076, /// 0x270
			 0xe04e8923, /// 0x274
			 0xbe4c55ec, /// 0x278
			 0x96aa2440, /// 0x27c
			 0xa7176a55, /// 0x280
			 0xcb674bd6, /// 0x284
			 0xf893674e, /// 0x288
			 0xd14255a6, /// 0x28c
			 0x7b4d27ad, /// 0x290
			 0x170553b1, /// 0x294
			 0xa710ff12, /// 0x298
			 0x610b80f2, /// 0x29c
			 0x420dabd4, /// 0x2a0
			 0xbe066a5c, /// 0x2a4
			 0x4b136f4b, /// 0x2a8
			 0x3ba03df0, /// 0x2ac
			 0x43683b65, /// 0x2b0
			 0x4a663652, /// 0x2b4
			 0x08563503, /// 0x2b8
			 0xcf0699a2, /// 0x2bc
			 0xce788244, /// 0x2c0
			 0x51fbc255, /// 0x2c4
			 0x8c711659, /// 0x2c8
			 0x4901a7d5, /// 0x2cc
			 0x85c96eb9, /// 0x2d0
			 0x896fc3f9, /// 0x2d4
			 0x5af5d5ee, /// 0x2d8
			 0x7d6d2587, /// 0x2dc
			 0xfd01b14f, /// 0x2e0
			 0x1727aa5d, /// 0x2e4
			 0xe2c1aaa0, /// 0x2e8
			 0x46c1bffc, /// 0x2ec
			 0x36d833d2, /// 0x2f0
			 0xf71b6a38, /// 0x2f4
			 0xd0fc73c5, /// 0x2f8
			 0x30a29c8c, /// 0x2fc
			 0x57ee717e, /// 0x300
			 0xe00effb7, /// 0x304
			 0x21d12ffa, /// 0x308
			 0x93844472, /// 0x30c
			 0x559ca385, /// 0x310
			 0xf0641f78, /// 0x314
			 0x48115a42, /// 0x318
			 0x254cfec2, /// 0x31c
			 0x9a980c27, /// 0x320
			 0xc75fd1d8, /// 0x324
			 0xeb3ad77f, /// 0x328
			 0x6e754242, /// 0x32c
			 0x260e5653, /// 0x330
			 0x5c81f1b6, /// 0x334
			 0xffb1a522, /// 0x338
			 0x7fdf9881, /// 0x33c
			 0x3e8222b1, /// 0x340
			 0xcf73ad15, /// 0x344
			 0xe7aa7f31, /// 0x348
			 0xc2045c34, /// 0x34c
			 0xb8232dc7, /// 0x350
			 0x46c8fdb3, /// 0x354
			 0x2c20f8d2, /// 0x358
			 0x2ed9eb27, /// 0x35c
			 0xd5fe7e05, /// 0x360
			 0x3f536b44, /// 0x364
			 0xc181fa7b, /// 0x368
			 0xa8d87e35, /// 0x36c
			 0x53dd2d4e, /// 0x370
			 0x1f2d3575, /// 0x374
			 0x410edff6, /// 0x378
			 0x7fa2a1c0, /// 0x37c
			 0x3776e3a0, /// 0x380
			 0x15ed096b, /// 0x384
			 0x65480036, /// 0x388
			 0x52b33f0c, /// 0x38c
			 0x2ff296f6, /// 0x390
			 0x23fd7a36, /// 0x394
			 0x4f08fc74, /// 0x398
			 0xf8a16d7c, /// 0x39c
			 0x47e75f86, /// 0x3a0
			 0xf10d1980, /// 0x3a4
			 0xa1d20e06, /// 0x3a8
			 0xeda45581, /// 0x3ac
			 0x98a62f9b, /// 0x3b0
			 0x992ffebe, /// 0x3b4
			 0x82863531, /// 0x3b8
			 0x9f35fdac, /// 0x3bc
			 0x69276554, /// 0x3c0
			 0x1ca0b0dd, /// 0x3c4
			 0x5cfa2f8c, /// 0x3c8
			 0x2914fd8c, /// 0x3cc
			 0x3286e457, /// 0x3d0
			 0x1d222155, /// 0x3d4
			 0x577f06b3, /// 0x3d8
			 0x5f1b44b3, /// 0x3dc
			 0x09279f5a, /// 0x3e0
			 0x8bf16592, /// 0x3e4
			 0xc2225d7f, /// 0x3e8
			 0x9aa3be70, /// 0x3ec
			 0xdda0be0b, /// 0x3f0
			 0xd27018f3, /// 0x3f4
			 0x3256c3c3, /// 0x3f8
			 0x6368763b, /// 0x3fc
			 0xbdd6f91d, /// 0x400
			 0x9f700391, /// 0x404
			 0x4f71d260, /// 0x408
			 0xf14a7f6e, /// 0x40c
			 0x1f68c6f8, /// 0x410
			 0xa0f00a8e, /// 0x414
			 0xbdc88ac6, /// 0x418
			 0x8c745c12, /// 0x41c
			 0x79276a14, /// 0x420
			 0xe4cae5c0, /// 0x424
			 0xe8789728, /// 0x428
			 0x4780a35e, /// 0x42c
			 0xfdb8b179, /// 0x430
			 0xb2999ac4, /// 0x434
			 0x5f8985d8, /// 0x438
			 0x8c19065a, /// 0x43c
			 0x29936577, /// 0x440
			 0x26d8dc10, /// 0x444
			 0x2b095c0b, /// 0x448
			 0x633b2157, /// 0x44c
			 0x0ff2905e, /// 0x450
			 0x18827bd8, /// 0x454
			 0xb3f49350, /// 0x458
			 0x9b9ba0b9, /// 0x45c
			 0x836e33c6, /// 0x460
			 0x1caea841, /// 0x464
			 0x3a69b746, /// 0x468
			 0x9eb5c73b, /// 0x46c
			 0xbc9144a7, /// 0x470
			 0xb9b99d9e, /// 0x474
			 0x5dddd131, /// 0x478
			 0xa7e07054, /// 0x47c
			 0xc95a3389, /// 0x480
			 0x29ecbcc5, /// 0x484
			 0x9c3035ad, /// 0x488
			 0xb8f021f6, /// 0x48c
			 0xcbe8f50b, /// 0x490
			 0x194e958e, /// 0x494
			 0xc048c148, /// 0x498
			 0x2686b54a, /// 0x49c
			 0x84aac4d5, /// 0x4a0
			 0x9d47a806, /// 0x4a4
			 0x55b4a425, /// 0x4a8
			 0xc94d26d6, /// 0x4ac
			 0xba282d78, /// 0x4b0
			 0xb8dd04b5, /// 0x4b4
			 0x8051deeb, /// 0x4b8
			 0xe328115a, /// 0x4bc
			 0x19a0113a, /// 0x4c0
			 0x708d265b, /// 0x4c4
			 0xa5f36f79, /// 0x4c8
			 0xbfaf05be, /// 0x4cc
			 0x884d5eed, /// 0x4d0
			 0x4b10c1d7, /// 0x4d4
			 0xaa950b48, /// 0x4d8
			 0x98204343, /// 0x4dc
			 0x24be6993, /// 0x4e0
			 0x663a9812, /// 0x4e4
			 0x2c6f29b9, /// 0x4e8
			 0xa73b64a2, /// 0x4ec
			 0x86de3629, /// 0x4f0
			 0xcb6694a8, /// 0x4f4
			 0xa8e38f69, /// 0x4f8
			 0x6d2452e9, /// 0x4fc
			 0xb2cde450, /// 0x500
			 0x5f950e00, /// 0x504
			 0x9ee8bf62, /// 0x508
			 0x1a0fd749, /// 0x50c
			 0xe8e242b5, /// 0x510
			 0x86ddf4ff, /// 0x514
			 0xe8b2971e, /// 0x518
			 0x666727bc, /// 0x51c
			 0x42b857ee, /// 0x520
			 0x0faa3bd6, /// 0x524
			 0x01d7d5dc, /// 0x528
			 0xbc77c0c0, /// 0x52c
			 0x315accc1, /// 0x530
			 0x19aa52cf, /// 0x534
			 0x405f8eaf, /// 0x538
			 0xad72b507, /// 0x53c
			 0x2115c3a8, /// 0x540
			 0xe7d03f7c, /// 0x544
			 0xbf1dee96, /// 0x548
			 0x36375142, /// 0x54c
			 0x03e28ed7, /// 0x550
			 0x7c2785d6, /// 0x554
			 0xc9606deb, /// 0x558
			 0x410ba993, /// 0x55c
			 0xeb93f39f, /// 0x560
			 0x62c5dc23, /// 0x564
			 0xe2838969, /// 0x568
			 0xd1fc21e7, /// 0x56c
			 0x70b3676e, /// 0x570
			 0x974de965, /// 0x574
			 0x746bde78, /// 0x578
			 0x6cd79a41, /// 0x57c
			 0xfe06a4ea, /// 0x580
			 0x43e004c2, /// 0x584
			 0x064266ff, /// 0x588
			 0x31e55928, /// 0x58c
			 0x417083eb, /// 0x590
			 0x86a01e08, /// 0x594
			 0x2f0c0bb0, /// 0x598
			 0xd63e6a70, /// 0x59c
			 0x64b1dca0, /// 0x5a0
			 0xf482f4e0, /// 0x5a4
			 0x4d1f9811, /// 0x5a8
			 0x41b48a99, /// 0x5ac
			 0xc95b5871, /// 0x5b0
			 0x0df4aa27, /// 0x5b4
			 0x315eedd3, /// 0x5b8
			 0xb4029b5f, /// 0x5bc
			 0x2e6eb11e, /// 0x5c0
			 0x206a66b5, /// 0x5c4
			 0xd7e29a36, /// 0x5c8
			 0x24525466, /// 0x5cc
			 0x1596ebac, /// 0x5d0
			 0xcf45bd4a, /// 0x5d4
			 0x91f37b94, /// 0x5d8
			 0x65856b64, /// 0x5dc
			 0xcca9560c, /// 0x5e0
			 0x0aa368ca, /// 0x5e4
			 0xedf952f1, /// 0x5e8
			 0x0c0af19f, /// 0x5ec
			 0x5bd933f4, /// 0x5f0
			 0x5fce3800, /// 0x5f4
			 0xbb75ad3a, /// 0x5f8
			 0x47f897be, /// 0x5fc
			 0x1d387683, /// 0x600
			 0x95649791, /// 0x604
			 0x973a70ef, /// 0x608
			 0x7458d2a5, /// 0x60c
			 0x0159637a, /// 0x610
			 0xea30f0f8, /// 0x614
			 0xc73687e7, /// 0x618
			 0x0ad007d7, /// 0x61c
			 0x6b773e1f, /// 0x620
			 0x1af4df15, /// 0x624
			 0x154cd63e, /// 0x628
			 0x76cc686a, /// 0x62c
			 0x1844c2ea, /// 0x630
			 0x38f10855, /// 0x634
			 0x236271a5, /// 0x638
			 0x4e8c6b82, /// 0x63c
			 0xa7674447, /// 0x640
			 0x9e49484b, /// 0x644
			 0xddd93922, /// 0x648
			 0xedc4797a, /// 0x64c
			 0x3bf98fb2, /// 0x650
			 0xa0fa4a72, /// 0x654
			 0x59baccae, /// 0x658
			 0x11b1b0bd, /// 0x65c
			 0x99e8ae5c, /// 0x660
			 0xd427057d, /// 0x664
			 0x8209f325, /// 0x668
			 0x41844554, /// 0x66c
			 0x90f7d8c3, /// 0x670
			 0xa3656edb, /// 0x674
			 0xc7203271, /// 0x678
			 0xb5814552, /// 0x67c
			 0x193102b7, /// 0x680
			 0x7e198895, /// 0x684
			 0x8c800794, /// 0x688
			 0xcafede60, /// 0x68c
			 0x6bf84289, /// 0x690
			 0xb00bcff1, /// 0x694
			 0x4f3e1411, /// 0x698
			 0x05a017fe, /// 0x69c
			 0x0a9f2fd8, /// 0x6a0
			 0x7da8acb5, /// 0x6a4
			 0x39cf2340, /// 0x6a8
			 0x97c8092d, /// 0x6ac
			 0x3c8f0845, /// 0x6b0
			 0xf754a134, /// 0x6b4
			 0x1bcd3977, /// 0x6b8
			 0xc779a9f4, /// 0x6bc
			 0xebaa223a, /// 0x6c0
			 0x4131e4b5, /// 0x6c4
			 0x5ac60975, /// 0x6c8
			 0xf3091f47, /// 0x6cc
			 0x20af93d6, /// 0x6d0
			 0x5c1b876d, /// 0x6d4
			 0x2d1a36c4, /// 0x6d8
			 0xf6f1f06f, /// 0x6dc
			 0x9a6b7b37, /// 0x6e0
			 0xb95ec00e, /// 0x6e4
			 0xe9c2d73a, /// 0x6e8
			 0xfbafcd34, /// 0x6ec
			 0x16dd280a, /// 0x6f0
			 0x57c0e2e2, /// 0x6f4
			 0xd3c4c428, /// 0x6f8
			 0xa979bcce, /// 0x6fc
			 0x7478a448, /// 0x700
			 0xf75d1f00, /// 0x704
			 0x0f101e9c, /// 0x708
			 0xe5246ea7, /// 0x70c
			 0x7f65f62b, /// 0x710
			 0xeef741fc, /// 0x714
			 0x225989a1, /// 0x718
			 0x2ca18cd4, /// 0x71c
			 0x6a4c1a13, /// 0x720
			 0x4f68a1df, /// 0x724
			 0x0b49ca4b, /// 0x728
			 0x38b758d0, /// 0x72c
			 0xa2aadb1e, /// 0x730
			 0x60021382, /// 0x734
			 0x82b0169b, /// 0x738
			 0xface5ff8, /// 0x73c
			 0xbb41c9c1, /// 0x740
			 0xf8c611db, /// 0x744
			 0x3e23cac5, /// 0x748
			 0xe3d105ce, /// 0x74c
			 0x2a1b019c, /// 0x750
			 0xf470e24b, /// 0x754
			 0xbff25ae0, /// 0x758
			 0x30ab1874, /// 0x75c
			 0x2c42c700, /// 0x760
			 0x61fc5536, /// 0x764
			 0x0c760ce8, /// 0x768
			 0xa915e8a1, /// 0x76c
			 0x3502a88e, /// 0x770
			 0x08c42486, /// 0x774
			 0x5f96bc0d, /// 0x778
			 0xbd0887c6, /// 0x77c
			 0x52f3fd66, /// 0x780
			 0x1ae00dcf, /// 0x784
			 0x6df4d5c1, /// 0x788
			 0xbfb2ca85, /// 0x78c
			 0x9c6b75ba, /// 0x790
			 0xda797535, /// 0x794
			 0xd0ee80ee, /// 0x798
			 0x7f62e9fe, /// 0x79c
			 0x914c24d3, /// 0x7a0
			 0xa8f21d60, /// 0x7a4
			 0x4bfd6888, /// 0x7a8
			 0x062c72dd, /// 0x7ac
			 0xaa242807, /// 0x7b0
			 0x4fab7305, /// 0x7b4
			 0x5cc1fbfe, /// 0x7b8
			 0xe50b5a40, /// 0x7bc
			 0xe46a4607, /// 0x7c0
			 0x182b5d1c, /// 0x7c4
			 0x1a70fef7, /// 0x7c8
			 0xe765a0be, /// 0x7cc
			 0x78d3a336, /// 0x7d0
			 0xb78b94af, /// 0x7d4
			 0xaea663d7, /// 0x7d8
			 0xe2e7e289, /// 0x7dc
			 0x64134056, /// 0x7e0
			 0xa1357ea2, /// 0x7e4
			 0xb06eef78, /// 0x7e8
			 0xa3943f0c, /// 0x7ec
			 0x7326a2df, /// 0x7f0
			 0x70dc0f00, /// 0x7f4
			 0xa8f89440, /// 0x7f8
			 0x22722f89, /// 0x7fc
			 0xd81426fe, /// 0x800
			 0x03a82d62, /// 0x804
			 0x98211669, /// 0x808
			 0x8178b3a1, /// 0x80c
			 0xd9c6febf, /// 0x810
			 0x5417cd37, /// 0x814
			 0x44b14125, /// 0x818
			 0x24cb90ab, /// 0x81c
			 0x32163731, /// 0x820
			 0x249427ee, /// 0x824
			 0xa57ed850, /// 0x828
			 0xf932292a, /// 0x82c
			 0x44ae3ca1, /// 0x830
			 0xe100f17a, /// 0x834
			 0xfe82bac8, /// 0x838
			 0x063982d7, /// 0x83c
			 0xdd2c6459, /// 0x840
			 0x388fc3a4, /// 0x844
			 0x72b1f664, /// 0x848
			 0x478c493f, /// 0x84c
			 0x35751449, /// 0x850
			 0x259ab3d7, /// 0x854
			 0xafae29a0, /// 0x858
			 0xc054830b, /// 0x85c
			 0x23e73e90, /// 0x860
			 0x18425c95, /// 0x864
			 0x72c2595b, /// 0x868
			 0x8a4292b6, /// 0x86c
			 0x64c25490, /// 0x870
			 0x5c5f1517, /// 0x874
			 0x4fd51c15, /// 0x878
			 0xbe870d60, /// 0x87c
			 0x6aa460c3, /// 0x880
			 0x99857022, /// 0x884
			 0x2da834fe, /// 0x888
			 0x920d0147, /// 0x88c
			 0xac4b7f4b, /// 0x890
			 0x55e49f5d, /// 0x894
			 0x590a39ff, /// 0x898
			 0x99c74fbb, /// 0x89c
			 0x26c34176, /// 0x8a0
			 0xe5cc7889, /// 0x8a4
			 0x82e0cf2c, /// 0x8a8
			 0x24058300, /// 0x8ac
			 0x4b6fc5df, /// 0x8b0
			 0xfb443266, /// 0x8b4
			 0x4c47dbdf, /// 0x8b8
			 0x73b3372f, /// 0x8bc
			 0xc3324e72, /// 0x8c0
			 0xed09c431, /// 0x8c4
			 0x128e9b4e, /// 0x8c8
			 0x2c003fa9, /// 0x8cc
			 0xb31ca56a, /// 0x8d0
			 0x95c0e289, /// 0x8d4
			 0x4fb99e23, /// 0x8d8
			 0xb6143b77, /// 0x8dc
			 0x6ca7a6a7, /// 0x8e0
			 0x299ee196, /// 0x8e4
			 0x3a2ee426, /// 0x8e8
			 0xdb204b38, /// 0x8ec
			 0xa35f4b3d, /// 0x8f0
			 0x7200cf2f, /// 0x8f4
			 0x6d54b4da, /// 0x8f8
			 0x5644f50b, /// 0x8fc
			 0x2177a559, /// 0x900
			 0x3ea5e895, /// 0x904
			 0x65baf70c, /// 0x908
			 0x375acdf5, /// 0x90c
			 0xc39fb930, /// 0x910
			 0x4814f0a7, /// 0x914
			 0x4a93434c, /// 0x918
			 0x4ce046d6, /// 0x91c
			 0x213d50e1, /// 0x920
			 0xeebdc8bd, /// 0x924
			 0x5730df49, /// 0x928
			 0xd9c0abb1, /// 0x92c
			 0x76e78364, /// 0x930
			 0x235c99f0, /// 0x934
			 0xd2e1bcdf, /// 0x938
			 0xa5139683, /// 0x93c
			 0x2e7195b4, /// 0x940
			 0x0f64bff4, /// 0x944
			 0x2a2e1c7c, /// 0x948
			 0x367e704e, /// 0x94c
			 0x65cc25b8, /// 0x950
			 0x15e6e2eb, /// 0x954
			 0xdd7d53b4, /// 0x958
			 0xbe725bd0, /// 0x95c
			 0xfab4d4d1, /// 0x960
			 0x1ff3b2e4, /// 0x964
			 0xf49835ec, /// 0x968
			 0xd25df2ee, /// 0x96c
			 0x5a2e434a, /// 0x970
			 0x19e81de9, /// 0x974
			 0xabaac3e8, /// 0x978
			 0x4d364885, /// 0x97c
			 0x0ace6725, /// 0x980
			 0x7393f056, /// 0x984
			 0x34e61968, /// 0x988
			 0x71842e5c, /// 0x98c
			 0xa01feb17, /// 0x990
			 0x8b9a0872, /// 0x994
			 0x5fefaec5, /// 0x998
			 0x7829e321, /// 0x99c
			 0xf7ef0ea6, /// 0x9a0
			 0xb4f4dba9, /// 0x9a4
			 0xd15b0d00, /// 0x9a8
			 0x639a9d70, /// 0x9ac
			 0xecb33929, /// 0x9b0
			 0xe358fc5c, /// 0x9b4
			 0x7cafd84a, /// 0x9b8
			 0x81819fb0, /// 0x9bc
			 0x1f815902, /// 0x9c0
			 0x488bd0b3, /// 0x9c4
			 0x5801c9ec, /// 0x9c8
			 0x860153c3, /// 0x9cc
			 0xfa04e7f6, /// 0x9d0
			 0x09f001ad, /// 0x9d4
			 0xbdc992bc, /// 0x9d8
			 0x2fd35aa3, /// 0x9dc
			 0xc722f532, /// 0x9e0
			 0x2dec485c, /// 0x9e4
			 0xef182aca, /// 0x9e8
			 0xfad1cadd, /// 0x9ec
			 0x6215413e, /// 0x9f0
			 0x94b09a59, /// 0x9f4
			 0xe9b2e646, /// 0x9f8
			 0x28fe126d, /// 0x9fc
			 0xa5d5c0ce, /// 0xa00
			 0x73fa0ffa, /// 0xa04
			 0xa2e7d863, /// 0xa08
			 0x99ee1dcd, /// 0xa0c
			 0x06028e88, /// 0xa10
			 0x39996620, /// 0xa14
			 0xd6495c02, /// 0xa18
			 0xf332c559, /// 0xa1c
			 0xee6dd132, /// 0xa20
			 0x1c41e08f, /// 0xa24
			 0x1f1eb97b, /// 0xa28
			 0x467e4951, /// 0xa2c
			 0x35184441, /// 0xa30
			 0xe882d8c7, /// 0xa34
			 0x6173fe3b, /// 0xa38
			 0x02b85a8b, /// 0xa3c
			 0x4afad58a, /// 0xa40
			 0xc6b49fca, /// 0xa44
			 0x1150e826, /// 0xa48
			 0x83e0dff6, /// 0xa4c
			 0x17936d15, /// 0xa50
			 0x80372c26, /// 0xa54
			 0xd0b4302f, /// 0xa58
			 0x2aed0423, /// 0xa5c
			 0x96a67de5, /// 0xa60
			 0x4a6352d7, /// 0xa64
			 0x87bb6b26, /// 0xa68
			 0x7d9bc077, /// 0xa6c
			 0xf8c4b452, /// 0xa70
			 0x0e52bbc6, /// 0xa74
			 0x61f201af, /// 0xa78
			 0xba92da31, /// 0xa7c
			 0xb71115c1, /// 0xa80
			 0xb192e776, /// 0xa84
			 0xb7706477, /// 0xa88
			 0x14b2579a, /// 0xa8c
			 0x9c363860, /// 0xa90
			 0x456dcf7c, /// 0xa94
			 0x69da9e61, /// 0xa98
			 0xf2c6ace3, /// 0xa9c
			 0x46026dcf, /// 0xaa0
			 0x944f7047, /// 0xaa4
			 0x0f66775c, /// 0xaa8
			 0x71e60c5e, /// 0xaac
			 0x285d3445, /// 0xab0
			 0xcad4a7ae, /// 0xab4
			 0x2dc33187, /// 0xab8
			 0xe7434e3c, /// 0xabc
			 0x9d5da812, /// 0xac0
			 0x15176833, /// 0xac4
			 0x4885769a, /// 0xac8
			 0xe7d76ffa, /// 0xacc
			 0x664d5c43, /// 0xad0
			 0x3bea450e, /// 0xad4
			 0x2a52c3f8, /// 0xad8
			 0x1899ef61, /// 0xadc
			 0x72777723, /// 0xae0
			 0x2abb9363, /// 0xae4
			 0xd9ce7abd, /// 0xae8
			 0x04574afc, /// 0xaec
			 0x61921aec, /// 0xaf0
			 0x40fb69b6, /// 0xaf4
			 0x1c15fdea, /// 0xaf8
			 0x85f1f03a, /// 0xafc
			 0x426e32a3, /// 0xb00
			 0x1df253e1, /// 0xb04
			 0x9ade52cd, /// 0xb08
			 0x7cb9896f, /// 0xb0c
			 0xb0d249af, /// 0xb10
			 0x4abe3531, /// 0xb14
			 0xcaca8df5, /// 0xb18
			 0xedf1dcdd, /// 0xb1c
			 0x9f110e5a, /// 0xb20
			 0xb5176885, /// 0xb24
			 0x0216c0f0, /// 0xb28
			 0xdb7d673a, /// 0xb2c
			 0x382c5a4d, /// 0xb30
			 0x02a843b5, /// 0xb34
			 0x12fe78cf, /// 0xb38
			 0x52e9bc42, /// 0xb3c
			 0xededd02c, /// 0xb40
			 0x99154f4c, /// 0xb44
			 0x0e8f3a13, /// 0xb48
			 0xef147988, /// 0xb4c
			 0xfe29dd75, /// 0xb50
			 0xf9ff98fc, /// 0xb54
			 0x3960ba43, /// 0xb58
			 0x3b47f454, /// 0xb5c
			 0xe56c1081, /// 0xb60
			 0xdf34da76, /// 0xb64
			 0x8aadeee8, /// 0xb68
			 0xdf31c4c6, /// 0xb6c
			 0x0dcf0014, /// 0xb70
			 0x81e6b21e, /// 0xb74
			 0xf3234ffe, /// 0xb78
			 0x3a0e306e, /// 0xb7c
			 0xc6abc61f, /// 0xb80
			 0xa595260e, /// 0xb84
			 0x10e5b467, /// 0xb88
			 0xdd687500, /// 0xb8c
			 0x14332366, /// 0xb90
			 0x689e2e50, /// 0xb94
			 0x31ba2a62, /// 0xb98
			 0x8f10d279, /// 0xb9c
			 0xcafdb6c9, /// 0xba0
			 0x83081ac3, /// 0xba4
			 0x12ad0222, /// 0xba8
			 0x525d7114, /// 0xbac
			 0x3e878aaf, /// 0xbb0
			 0x53d5d168, /// 0xbb4
			 0xd2177b1f, /// 0xbb8
			 0xaf989d9d, /// 0xbbc
			 0x601603b3, /// 0xbc0
			 0xfcc67f54, /// 0xbc4
			 0x2ab7e8dc, /// 0xbc8
			 0xbddcb317, /// 0xbcc
			 0xb7fd3e1a, /// 0xbd0
			 0xe2dc8986, /// 0xbd4
			 0xbbe62ae7, /// 0xbd8
			 0x6a08c11c, /// 0xbdc
			 0x8cc0c03e, /// 0xbe0
			 0xd40a453c, /// 0xbe4
			 0x79b7dab9, /// 0xbe8
			 0x5385835b, /// 0xbec
			 0xde10db96, /// 0xbf0
			 0x78d62070, /// 0xbf4
			 0x6716de36, /// 0xbf8
			 0xf63c141c, /// 0xbfc
			 0x386d9dd7, /// 0xc00
			 0xa6ae966d, /// 0xc04
			 0x39c182c3, /// 0xc08
			 0x226a0704, /// 0xc0c
			 0xf080b357, /// 0xc10
			 0xc1910f16, /// 0xc14
			 0x65cd924c, /// 0xc18
			 0x9e9686e2, /// 0xc1c
			 0x7ad5beda, /// 0xc20
			 0x28a23470, /// 0xc24
			 0xc5858bb5, /// 0xc28
			 0xb30969f7, /// 0xc2c
			 0x8076d8bd, /// 0xc30
			 0xf18358dd, /// 0xc34
			 0x8872444e, /// 0xc38
			 0xb77d91c5, /// 0xc3c
			 0xafaa1019, /// 0xc40
			 0x84a5ed44, /// 0xc44
			 0x0d89bbac, /// 0xc48
			 0x38adbbb8, /// 0xc4c
			 0x988d4aac, /// 0xc50
			 0xee9bf743, /// 0xc54
			 0xcdc2c3b3, /// 0xc58
			 0x78cf1d93, /// 0xc5c
			 0x6f86b8d0, /// 0xc60
			 0xffa27366, /// 0xc64
			 0x714fdc22, /// 0xc68
			 0xa0830b32, /// 0xc6c
			 0x4043797f, /// 0xc70
			 0x60e7a6e7, /// 0xc74
			 0x8c45b695, /// 0xc78
			 0x55a82577, /// 0xc7c
			 0xa3bc0c2c, /// 0xc80
			 0xe1f2c110, /// 0xc84
			 0x94dbdcb8, /// 0xc88
			 0x6c89c6db, /// 0xc8c
			 0x16fd5139, /// 0xc90
			 0x5458fc23, /// 0xc94
			 0x0ccf607f, /// 0xc98
			 0x2b1a410c, /// 0xc9c
			 0x72bd6cae, /// 0xca0
			 0x0d1641f7, /// 0xca4
			 0x7a78144f, /// 0xca8
			 0xbff74479, /// 0xcac
			 0x5f0dd464, /// 0xcb0
			 0xbf774647, /// 0xcb4
			 0x94b64044, /// 0xcb8
			 0x83fa83b7, /// 0xcbc
			 0x467eea75, /// 0xcc0
			 0xf10d8331, /// 0xcc4
			 0xe605a277, /// 0xcc8
			 0xa71e8c4b, /// 0xccc
			 0x224183d0, /// 0xcd0
			 0x3c285aa1, /// 0xcd4
			 0xd9398ed4, /// 0xcd8
			 0x99b9993d, /// 0xcdc
			 0x5d25fa61, /// 0xce0
			 0x3c80a092, /// 0xce4
			 0x6a76cfc5, /// 0xce8
			 0xf8957b63, /// 0xcec
			 0x1ca4b0f0, /// 0xcf0
			 0x24c8ac70, /// 0xcf4
			 0xc3cf47d3, /// 0xcf8
			 0x1539c9a0, /// 0xcfc
			 0x2683147d, /// 0xd00
			 0x235eb861, /// 0xd04
			 0xe0028392, /// 0xd08
			 0x34fca49b, /// 0xd0c
			 0x6b33dc53, /// 0xd10
			 0xa0167d8a, /// 0xd14
			 0x6a52a468, /// 0xd18
			 0xf06ea7d5, /// 0xd1c
			 0x91e228a7, /// 0xd20
			 0x44504a12, /// 0xd24
			 0x88c1653f, /// 0xd28
			 0xaa5b4536, /// 0xd2c
			 0x03e132af, /// 0xd30
			 0x0a58d03b, /// 0xd34
			 0x23466f90, /// 0xd38
			 0xb158b866, /// 0xd3c
			 0xafa935c6, /// 0xd40
			 0x9776132b, /// 0xd44
			 0xd074c194, /// 0xd48
			 0xa2059f52, /// 0xd4c
			 0x51411da1, /// 0xd50
			 0xe3a1de26, /// 0xd54
			 0xf22e5cb4, /// 0xd58
			 0xf29a9464, /// 0xd5c
			 0x5b9031f9, /// 0xd60
			 0xd106f92a, /// 0xd64
			 0x3faf0bc2, /// 0xd68
			 0x97e12e99, /// 0xd6c
			 0xb5dbccc7, /// 0xd70
			 0x0fccfa60, /// 0xd74
			 0xc32a96d0, /// 0xd78
			 0x6663a0d9, /// 0xd7c
			 0xf552e800, /// 0xd80
			 0x993c42b3, /// 0xd84
			 0x62369d84, /// 0xd88
			 0x262e8028, /// 0xd8c
			 0x2534b0e6, /// 0xd90
			 0x7c327647, /// 0xd94
			 0xba4aba82, /// 0xd98
			 0xcf17216e, /// 0xd9c
			 0xf4f84ff7, /// 0xda0
			 0x490882d9, /// 0xda4
			 0x99707592, /// 0xda8
			 0x9d036f3a, /// 0xdac
			 0xa714d26f, /// 0xdb0
			 0xcce80d78, /// 0xdb4
			 0x3f8731aa, /// 0xdb8
			 0x9d1b04e9, /// 0xdbc
			 0xe784b60b, /// 0xdc0
			 0x09496916, /// 0xdc4
			 0xc3cc5d8c, /// 0xdc8
			 0xc1c7dd85, /// 0xdcc
			 0x8a9bcce9, /// 0xdd0
			 0x0452d705, /// 0xdd4
			 0x06b149ec, /// 0xdd8
			 0x3f023627, /// 0xddc
			 0x330af493, /// 0xde0
			 0xcceabd19, /// 0xde4
			 0xa195d154, /// 0xde8
			 0x7d6bc818, /// 0xdec
			 0x910d053c, /// 0xdf0
			 0x8dd1e1c3, /// 0xdf4
			 0xf948fa2c, /// 0xdf8
			 0xb3cce233, /// 0xdfc
			 0x23d16c75, /// 0xe00
			 0x794d3b11, /// 0xe04
			 0x6306e944, /// 0xe08
			 0x3ada90a1, /// 0xe0c
			 0x577fc831, /// 0xe10
			 0xf034eac9, /// 0xe14
			 0x911c4e2d, /// 0xe18
			 0x013cf2a0, /// 0xe1c
			 0xa1c8962a, /// 0xe20
			 0x4c73307b, /// 0xe24
			 0xa7d814f2, /// 0xe28
			 0x8027d19f, /// 0xe2c
			 0xac21f96e, /// 0xe30
			 0x3dfea5bb, /// 0xe34
			 0x26c529c1, /// 0xe38
			 0x49980dcf, /// 0xe3c
			 0x6da4c99a, /// 0xe40
			 0x3db14526, /// 0xe44
			 0xbbe1b7c8, /// 0xe48
			 0x37d89d64, /// 0xe4c
			 0xebeb0bb5, /// 0xe50
			 0x24758f91, /// 0xe54
			 0x759d9689, /// 0xe58
			 0x3d3a7e1f, /// 0xe5c
			 0x82a4f66f, /// 0xe60
			 0x977db8ed, /// 0xe64
			 0x4a6a9537, /// 0xe68
			 0xad92bbcb, /// 0xe6c
			 0x0ed12298, /// 0xe70
			 0xf656e7dc, /// 0xe74
			 0xdb0981cc, /// 0xe78
			 0x763bd10a, /// 0xe7c
			 0x15b6abe9, /// 0xe80
			 0xfd75a71b, /// 0xe84
			 0x47cd874f, /// 0xe88
			 0x2623b7bf, /// 0xe8c
			 0xdb403f4f, /// 0xe90
			 0x1939afce, /// 0xe94
			 0xeee278d0, /// 0xe98
			 0x3ae434ac, /// 0xe9c
			 0x62af390a, /// 0xea0
			 0xa91e67ef, /// 0xea4
			 0xd22632ae, /// 0xea8
			 0x061d822d, /// 0xeac
			 0x2abccabb, /// 0xeb0
			 0x122e9e17, /// 0xeb4
			 0x7c8b99f4, /// 0xeb8
			 0xefc429fa, /// 0xebc
			 0xecdd985f, /// 0xec0
			 0x11cc4e3c, /// 0xec4
			 0xac3533a8, /// 0xec8
			 0xf2fd8e0c, /// 0xecc
			 0x8bedb36c, /// 0xed0
			 0xa935b45d, /// 0xed4
			 0x907282a4, /// 0xed8
			 0xe0d56ad1, /// 0xedc
			 0x0613f4cb, /// 0xee0
			 0x9c043b3e, /// 0xee4
			 0x9b21953e, /// 0xee8
			 0xbd0ae05e, /// 0xeec
			 0xa02aecf2, /// 0xef0
			 0x790540d6, /// 0xef4
			 0x15649ad5, /// 0xef8
			 0x179aea03, /// 0xefc
			 0x00525a97, /// 0xf00
			 0x392fddf0, /// 0xf04
			 0x702592f9, /// 0xf08
			 0xc611ff06, /// 0xf0c
			 0xfbafbe4a, /// 0xf10
			 0x75cf267e, /// 0xf14
			 0xbc54b191, /// 0xf18
			 0x4243d0e4, /// 0xf1c
			 0xd9b2f779, /// 0xf20
			 0x5c110972, /// 0xf24
			 0xebfeff52, /// 0xf28
			 0x9580c2d4, /// 0xf2c
			 0xc0ced688, /// 0xf30
			 0xa4265e4c, /// 0xf34
			 0x745a4ef2, /// 0xf38
			 0xd256c560, /// 0xf3c
			 0xb6b71047, /// 0xf40
			 0xff2d131e, /// 0xf44
			 0x05e37401, /// 0xf48
			 0xc8bb092f, /// 0xf4c
			 0xf136e0e3, /// 0xf50
			 0x83f4a5a4, /// 0xf54
			 0x0f94f230, /// 0xf58
			 0x6367bd95, /// 0xf5c
			 0x430cbf92, /// 0xf60
			 0x4d8ff099, /// 0xf64
			 0x9be6ea8f, /// 0xf68
			 0x4c5df1d5, /// 0xf6c
			 0xfa41c73e, /// 0xf70
			 0x026c01a7, /// 0xf74
			 0x2ae187ea, /// 0xf78
			 0x9c1f2509, /// 0xf7c
			 0x420ca3fd, /// 0xf80
			 0xe688f814, /// 0xf84
			 0x4f3f7205, /// 0xf88
			 0x342e9764, /// 0xf8c
			 0xfdb08833, /// 0xf90
			 0x94cfe1dd, /// 0xf94
			 0xd2e4260a, /// 0xf98
			 0xbe180b73, /// 0xf9c
			 0x3bcc6ad1, /// 0xfa0
			 0x1e02901d, /// 0xfa4
			 0x56b6c465, /// 0xfa8
			 0xbd1d51e4, /// 0xfac
			 0x5baf5b75, /// 0xfb0
			 0x4236239f, /// 0xfb4
			 0x2b476ad4, /// 0xfb8
			 0x51a1c199, /// 0xfbc
			 0x2ed4cf83, /// 0xfc0
			 0x1d932494, /// 0xfc4
			 0x2b297823, /// 0xfc8
			 0xe6a6de06, /// 0xfcc
			 0x20fc3e67, /// 0xfd0
			 0xed75793d, /// 0xfd4
			 0x7389ad24, /// 0xfd8
			 0xa5e5ece1, /// 0xfdc
			 0xbf3cd3da, /// 0xfe0
			 0x20f73f9c, /// 0xfe4
			 0xe855457e, /// 0xfe8
			 0xdc1ce637, /// 0xfec
			 0x7f821103, /// 0xff0
			 0x91b56e42, /// 0xff4
			 0xa3b1e805, /// 0xff8
			 0x06ed3b56 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x077c86fb, /// 0x0
			 0x0233c05e, /// 0x4
			 0x9f8b23d3, /// 0x8
			 0x2b7884f8, /// 0xc
			 0x9e29f2af, /// 0x10
			 0xc188455f, /// 0x14
			 0x952ecd9b, /// 0x18
			 0x3e2bf179, /// 0x1c
			 0xa284ffac, /// 0x20
			 0x9451a469, /// 0x24
			 0x166eaa18, /// 0x28
			 0x2c98e0dc, /// 0x2c
			 0x8fc02890, /// 0x30
			 0x43b16114, /// 0x34
			 0x8ec05f6b, /// 0x38
			 0xd2f3ae68, /// 0x3c
			 0xe8269252, /// 0x40
			 0x95e604b9, /// 0x44
			 0xfd1ce2a7, /// 0x48
			 0x5deb887c, /// 0x4c
			 0x43c64096, /// 0x50
			 0xa41e287c, /// 0x54
			 0xb3fe4308, /// 0x58
			 0xed65bd66, /// 0x5c
			 0x48cb77c9, /// 0x60
			 0x36914199, /// 0x64
			 0x36a9be3f, /// 0x68
			 0x8bd35ce5, /// 0x6c
			 0x2abda267, /// 0x70
			 0x0bb32105, /// 0x74
			 0x4f609056, /// 0x78
			 0x66708703, /// 0x7c
			 0xdd9cf4c8, /// 0x80
			 0x20fd6cd3, /// 0x84
			 0x4ef22000, /// 0x88
			 0x9aecb041, /// 0x8c
			 0x83650903, /// 0x90
			 0x49a2445a, /// 0x94
			 0x70289339, /// 0x98
			 0x8aac8142, /// 0x9c
			 0x1fede7f1, /// 0xa0
			 0x3dff11f1, /// 0xa4
			 0x09769a06, /// 0xa8
			 0xe3f78794, /// 0xac
			 0x4a4a0e75, /// 0xb0
			 0xd1f497f9, /// 0xb4
			 0x3edb5292, /// 0xb8
			 0x93533e86, /// 0xbc
			 0x073245bf, /// 0xc0
			 0x935bbc98, /// 0xc4
			 0x474b3ebf, /// 0xc8
			 0x75552248, /// 0xcc
			 0xf6730e94, /// 0xd0
			 0x8344beae, /// 0xd4
			 0xccd58eae, /// 0xd8
			 0xc9b8945a, /// 0xdc
			 0xcfc41de3, /// 0xe0
			 0x50c0da44, /// 0xe4
			 0xca8a6159, /// 0xe8
			 0xeb558e9a, /// 0xec
			 0x94bea998, /// 0xf0
			 0x28929ccd, /// 0xf4
			 0xb1b82f0f, /// 0xf8
			 0x7751111f, /// 0xfc
			 0x48feaae5, /// 0x100
			 0xa0c51ba9, /// 0x104
			 0xb6756d27, /// 0x108
			 0x1a35fc95, /// 0x10c
			 0x7b2b028a, /// 0x110
			 0x6c677168, /// 0x114
			 0xf7e9eadf, /// 0x118
			 0x01048d77, /// 0x11c
			 0x1e74297d, /// 0x120
			 0x2a2f16bb, /// 0x124
			 0x7afb6e92, /// 0x128
			 0xa924fdec, /// 0x12c
			 0xa7a7a7b6, /// 0x130
			 0x3167d0a5, /// 0x134
			 0xf0062860, /// 0x138
			 0x1ad6d067, /// 0x13c
			 0x0bb3f2f0, /// 0x140
			 0xe3e1cb36, /// 0x144
			 0x912a72fa, /// 0x148
			 0xbcabbbaa, /// 0x14c
			 0xfcf9362a, /// 0x150
			 0x6a105fe4, /// 0x154
			 0x573a575c, /// 0x158
			 0xcaf0cbd2, /// 0x15c
			 0xf84d42d9, /// 0x160
			 0x338093ec, /// 0x164
			 0x7f3a83a8, /// 0x168
			 0xa7c06d2e, /// 0x16c
			 0xea1b1af4, /// 0x170
			 0xb7128744, /// 0x174
			 0xfd33153b, /// 0x178
			 0x7f7d0021, /// 0x17c
			 0xe6184757, /// 0x180
			 0xd40b0a8e, /// 0x184
			 0x6e450eb0, /// 0x188
			 0x1c11da43, /// 0x18c
			 0xd9ad92fc, /// 0x190
			 0x8785a75a, /// 0x194
			 0xee052c67, /// 0x198
			 0x31806755, /// 0x19c
			 0x7221561c, /// 0x1a0
			 0x818ad0ab, /// 0x1a4
			 0xc4edd5b7, /// 0x1a8
			 0x8c0da041, /// 0x1ac
			 0xe39b3b59, /// 0x1b0
			 0xfaf128eb, /// 0x1b4
			 0x40c09fb4, /// 0x1b8
			 0x94c1d96a, /// 0x1bc
			 0x05387888, /// 0x1c0
			 0xafedeae4, /// 0x1c4
			 0x2ca09819, /// 0x1c8
			 0x24b73600, /// 0x1cc
			 0xbf7300ac, /// 0x1d0
			 0xcb189b38, /// 0x1d4
			 0xf33f9dea, /// 0x1d8
			 0x3c8be13d, /// 0x1dc
			 0xa2ee7e7b, /// 0x1e0
			 0x0bd4be94, /// 0x1e4
			 0xe52c2490, /// 0x1e8
			 0x38f8b955, /// 0x1ec
			 0xb9108e93, /// 0x1f0
			 0xc4774836, /// 0x1f4
			 0x80d85cc5, /// 0x1f8
			 0xc7824748, /// 0x1fc
			 0xfc1a071a, /// 0x200
			 0xa8b78e81, /// 0x204
			 0x69ae58c8, /// 0x208
			 0xa293be7b, /// 0x20c
			 0x3744397a, /// 0x210
			 0xea865e4c, /// 0x214
			 0xd1243392, /// 0x218
			 0x2618c76e, /// 0x21c
			 0xf48a5528, /// 0x220
			 0x8cf50e97, /// 0x224
			 0xa743961d, /// 0x228
			 0x3dd76fe7, /// 0x22c
			 0x845c5188, /// 0x230
			 0xa4506dcd, /// 0x234
			 0xafeab4a2, /// 0x238
			 0x865ec7d0, /// 0x23c
			 0x456e7abb, /// 0x240
			 0x2bb45dc7, /// 0x244
			 0xd6cc134c, /// 0x248
			 0x04bac1e4, /// 0x24c
			 0xe680c8c7, /// 0x250
			 0xe8165775, /// 0x254
			 0x90b97e2d, /// 0x258
			 0x2c7e19a6, /// 0x25c
			 0xe52ef8f1, /// 0x260
			 0xdc12acb0, /// 0x264
			 0xa9f8b9c6, /// 0x268
			 0x918613d8, /// 0x26c
			 0x620e8273, /// 0x270
			 0x74e75f89, /// 0x274
			 0xe8c52cef, /// 0x278
			 0xbe0ec1a1, /// 0x27c
			 0x2039af8d, /// 0x280
			 0x4454419b, /// 0x284
			 0x5fa96b0a, /// 0x288
			 0xd2c9faf1, /// 0x28c
			 0x9f2d5891, /// 0x290
			 0xd72faeff, /// 0x294
			 0xa24cc521, /// 0x298
			 0x6ca130a6, /// 0x29c
			 0x768283f0, /// 0x2a0
			 0xb926f3fd, /// 0x2a4
			 0x71489e89, /// 0x2a8
			 0x3dfe62b3, /// 0x2ac
			 0x03c7a92d, /// 0x2b0
			 0x02833700, /// 0x2b4
			 0xf7347abd, /// 0x2b8
			 0xf1bc0c36, /// 0x2bc
			 0x37b0f672, /// 0x2c0
			 0xb13c1005, /// 0x2c4
			 0xd00f0932, /// 0x2c8
			 0x076b3054, /// 0x2cc
			 0x21a2d6cb, /// 0x2d0
			 0x3b14280a, /// 0x2d4
			 0x242a84ab, /// 0x2d8
			 0xea43a1cf, /// 0x2dc
			 0x154e3b22, /// 0x2e0
			 0xd7783c64, /// 0x2e4
			 0xcaf88289, /// 0x2e8
			 0xe12929b4, /// 0x2ec
			 0xcd59806c, /// 0x2f0
			 0x31dd2273, /// 0x2f4
			 0x4bfcc673, /// 0x2f8
			 0xadb2162c, /// 0x2fc
			 0x3c353ce7, /// 0x300
			 0x9259865e, /// 0x304
			 0x4fedd37b, /// 0x308
			 0x569d799d, /// 0x30c
			 0x6dd0e190, /// 0x310
			 0xfe3837fa, /// 0x314
			 0x68ee516d, /// 0x318
			 0x2a5edd89, /// 0x31c
			 0xb54b3ffa, /// 0x320
			 0xf92d0e6f, /// 0x324
			 0xbfe42083, /// 0x328
			 0x50aea9b5, /// 0x32c
			 0x101d0b73, /// 0x330
			 0xa8824f71, /// 0x334
			 0xef7252b9, /// 0x338
			 0x5bf6972c, /// 0x33c
			 0x71c3a85c, /// 0x340
			 0x9b102f52, /// 0x344
			 0x1e095ff9, /// 0x348
			 0x26d03dad, /// 0x34c
			 0x5adcc64b, /// 0x350
			 0x762b9c10, /// 0x354
			 0x95b76eea, /// 0x358
			 0xc3fcf59c, /// 0x35c
			 0x066481bc, /// 0x360
			 0x40c28cce, /// 0x364
			 0x0b85c0fe, /// 0x368
			 0x1c23f194, /// 0x36c
			 0xa5da4043, /// 0x370
			 0x0b34fe83, /// 0x374
			 0x0d0be96c, /// 0x378
			 0x7141c4ee, /// 0x37c
			 0x3d6c3c6f, /// 0x380
			 0x6c7c1e19, /// 0x384
			 0xc597e214, /// 0x388
			 0x4462f340, /// 0x38c
			 0xaef680d9, /// 0x390
			 0xb1a636d7, /// 0x394
			 0x661ab707, /// 0x398
			 0x99d06418, /// 0x39c
			 0xf0b90903, /// 0x3a0
			 0x9a12d066, /// 0x3a4
			 0x018526e0, /// 0x3a8
			 0x931e6f49, /// 0x3ac
			 0x19c76c15, /// 0x3b0
			 0xffe67c67, /// 0x3b4
			 0xaaf4c516, /// 0x3b8
			 0xdcddf6e0, /// 0x3bc
			 0x8cc8c9d7, /// 0x3c0
			 0xb1b054b8, /// 0x3c4
			 0x9bb7eb81, /// 0x3c8
			 0x281fffd1, /// 0x3cc
			 0xf51b129b, /// 0x3d0
			 0x09fd5d8f, /// 0x3d4
			 0x55726d26, /// 0x3d8
			 0x05f444bf, /// 0x3dc
			 0x0ecc0ab3, /// 0x3e0
			 0x7186305f, /// 0x3e4
			 0x6e71ff90, /// 0x3e8
			 0xcdc55bf4, /// 0x3ec
			 0xc198a5d7, /// 0x3f0
			 0xe4b759a8, /// 0x3f4
			 0x64c24d50, /// 0x3f8
			 0xdff63b09, /// 0x3fc
			 0x7a80d230, /// 0x400
			 0x67103727, /// 0x404
			 0xd4e260ee, /// 0x408
			 0xfda4e735, /// 0x40c
			 0x52562a8b, /// 0x410
			 0xe9428c7d, /// 0x414
			 0x10b1e033, /// 0x418
			 0x91194090, /// 0x41c
			 0xd3d7d47d, /// 0x420
			 0x9e699280, /// 0x424
			 0x2747dea9, /// 0x428
			 0x5fb03c36, /// 0x42c
			 0xb360fee4, /// 0x430
			 0x9cac034c, /// 0x434
			 0xae567180, /// 0x438
			 0x705cc39d, /// 0x43c
			 0xc5149bbe, /// 0x440
			 0xc489b8ef, /// 0x444
			 0xdb1a6a1f, /// 0x448
			 0x55a19d22, /// 0x44c
			 0xecdb92d1, /// 0x450
			 0x5ec92932, /// 0x454
			 0x910f95da, /// 0x458
			 0x8cf7b030, /// 0x45c
			 0x43487a09, /// 0x460
			 0x1477186d, /// 0x464
			 0x1c57ca00, /// 0x468
			 0xd47f3a52, /// 0x46c
			 0xed968c3f, /// 0x470
			 0x5415a9b5, /// 0x474
			 0x252d0d3e, /// 0x478
			 0x8290ea32, /// 0x47c
			 0x43a3c1e1, /// 0x480
			 0x74c19827, /// 0x484
			 0x8b13d47b, /// 0x488
			 0x3acba3fd, /// 0x48c
			 0xdcc7406c, /// 0x490
			 0xb42c55bd, /// 0x494
			 0x1079f037, /// 0x498
			 0x22c402c4, /// 0x49c
			 0xd372c9f6, /// 0x4a0
			 0xa8dcc68c, /// 0x4a4
			 0x536d9c51, /// 0x4a8
			 0xb19f2801, /// 0x4ac
			 0x3730d3fc, /// 0x4b0
			 0x8ef59631, /// 0x4b4
			 0x63dcb5fa, /// 0x4b8
			 0xdfdc4be6, /// 0x4bc
			 0x06f6edc9, /// 0x4c0
			 0x22bf9291, /// 0x4c4
			 0xe386fcd1, /// 0x4c8
			 0xfe4d251b, /// 0x4cc
			 0xc74581bb, /// 0x4d0
			 0x70c1dde2, /// 0x4d4
			 0xb0e37241, /// 0x4d8
			 0xf933d518, /// 0x4dc
			 0xfa5e3643, /// 0x4e0
			 0x89556579, /// 0x4e4
			 0x63c2349f, /// 0x4e8
			 0x367ca7ae, /// 0x4ec
			 0xe04c5fc4, /// 0x4f0
			 0xa225df81, /// 0x4f4
			 0x2b7876d9, /// 0x4f8
			 0x7901629d, /// 0x4fc
			 0x44ffed89, /// 0x500
			 0x23df435a, /// 0x504
			 0x4c945603, /// 0x508
			 0x07bc3efa, /// 0x50c
			 0x4ab9a73a, /// 0x510
			 0x35814b02, /// 0x514
			 0x3261bff6, /// 0x518
			 0xdc16faa6, /// 0x51c
			 0xbe211bed, /// 0x520
			 0x172d5117, /// 0x524
			 0x1057eb8d, /// 0x528
			 0xc326db03, /// 0x52c
			 0xd96ccfe6, /// 0x530
			 0x0c433182, /// 0x534
			 0xbd25bfda, /// 0x538
			 0xb2641c83, /// 0x53c
			 0x559f6e33, /// 0x540
			 0xaf5949c4, /// 0x544
			 0x54e9b5bd, /// 0x548
			 0x76232247, /// 0x54c
			 0xbd5f0ed2, /// 0x550
			 0x3e6430dc, /// 0x554
			 0x91ce118a, /// 0x558
			 0x2b124800, /// 0x55c
			 0x568dd7d6, /// 0x560
			 0x5ffe95af, /// 0x564
			 0xa1dabd85, /// 0x568
			 0xa9f9e0c8, /// 0x56c
			 0x05faa593, /// 0x570
			 0xd9896e4d, /// 0x574
			 0x1225467d, /// 0x578
			 0x81603771, /// 0x57c
			 0xfe215aa5, /// 0x580
			 0xce0705f4, /// 0x584
			 0xabbc2129, /// 0x588
			 0xa521ed6c, /// 0x58c
			 0x0196ab36, /// 0x590
			 0x0c8a9bdb, /// 0x594
			 0xc2dbdc52, /// 0x598
			 0x463bfbf0, /// 0x59c
			 0x2006036d, /// 0x5a0
			 0xe3a67cad, /// 0x5a4
			 0xb074ae2f, /// 0x5a8
			 0xb9aa951d, /// 0x5ac
			 0x52082853, /// 0x5b0
			 0x9de59e3c, /// 0x5b4
			 0x607e29cf, /// 0x5b8
			 0x5d989acb, /// 0x5bc
			 0x0b4e689b, /// 0x5c0
			 0x129cc1a4, /// 0x5c4
			 0xe10f70f3, /// 0x5c8
			 0xbd632b19, /// 0x5cc
			 0xbd3da6be, /// 0x5d0
			 0xff93b412, /// 0x5d4
			 0xce388fc4, /// 0x5d8
			 0x45996c68, /// 0x5dc
			 0xad99c901, /// 0x5e0
			 0x31a317a0, /// 0x5e4
			 0x6fbb5ad7, /// 0x5e8
			 0x5b0edc3b, /// 0x5ec
			 0x8179b043, /// 0x5f0
			 0x4f38db94, /// 0x5f4
			 0x9f1e5328, /// 0x5f8
			 0xa502ae7e, /// 0x5fc
			 0x03522d0c, /// 0x600
			 0x3e0a6820, /// 0x604
			 0xc1df12ab, /// 0x608
			 0x77fa238d, /// 0x60c
			 0x28f86644, /// 0x610
			 0x82f0882d, /// 0x614
			 0xcfa7fe91, /// 0x618
			 0x6c487950, /// 0x61c
			 0x811f0687, /// 0x620
			 0x9ae98c41, /// 0x624
			 0x30608839, /// 0x628
			 0xe1577692, /// 0x62c
			 0x4ea93804, /// 0x630
			 0xf9cbbfb0, /// 0x634
			 0x565f37c4, /// 0x638
			 0x12b7cde1, /// 0x63c
			 0x2c43d52c, /// 0x640
			 0x9f94b769, /// 0x644
			 0x64083efe, /// 0x648
			 0x17317c58, /// 0x64c
			 0x13463f4b, /// 0x650
			 0xef3e4b5f, /// 0x654
			 0x413893c5, /// 0x658
			 0xbf1f6154, /// 0x65c
			 0xc05706ce, /// 0x660
			 0x585206bb, /// 0x664
			 0xc476d0d5, /// 0x668
			 0x6ce86c77, /// 0x66c
			 0x2abf6dff, /// 0x670
			 0xb8e5b391, /// 0x674
			 0x0da79dd8, /// 0x678
			 0x52e14e83, /// 0x67c
			 0xc83be4c9, /// 0x680
			 0xf995d04c, /// 0x684
			 0x84937286, /// 0x688
			 0xbbb542db, /// 0x68c
			 0x0791eac9, /// 0x690
			 0xbbe39d72, /// 0x694
			 0x6079cd5b, /// 0x698
			 0xa045d1fb, /// 0x69c
			 0xdec6b45c, /// 0x6a0
			 0xf1836fea, /// 0x6a4
			 0x415242e3, /// 0x6a8
			 0x8fdd477b, /// 0x6ac
			 0x4e4fff38, /// 0x6b0
			 0x6b915a00, /// 0x6b4
			 0xe16e065d, /// 0x6b8
			 0x76daf3ef, /// 0x6bc
			 0x5e4c9183, /// 0x6c0
			 0x093702e3, /// 0x6c4
			 0xda0ef3a5, /// 0x6c8
			 0xbf311956, /// 0x6cc
			 0x9e354106, /// 0x6d0
			 0x84d1a868, /// 0x6d4
			 0x1c6edfff, /// 0x6d8
			 0x1b60b773, /// 0x6dc
			 0xbb0c193b, /// 0x6e0
			 0xd1e0e606, /// 0x6e4
			 0xee78dde3, /// 0x6e8
			 0xeaf34813, /// 0x6ec
			 0x415b50fa, /// 0x6f0
			 0x02ecec16, /// 0x6f4
			 0x3d30f742, /// 0x6f8
			 0x561eeba0, /// 0x6fc
			 0x9e7bef3a, /// 0x700
			 0xf45fd564, /// 0x704
			 0xc815ea9c, /// 0x708
			 0xd166ec97, /// 0x70c
			 0x7cf192dc, /// 0x710
			 0x58028899, /// 0x714
			 0xef44d2cb, /// 0x718
			 0x9f1fca91, /// 0x71c
			 0xad41ae17, /// 0x720
			 0x4c917f08, /// 0x724
			 0x5943e643, /// 0x728
			 0x1b205aee, /// 0x72c
			 0x9a1bba24, /// 0x730
			 0x877964fa, /// 0x734
			 0x51efaf64, /// 0x738
			 0x44a5e4a0, /// 0x73c
			 0x9bd2b394, /// 0x740
			 0xbb6fd650, /// 0x744
			 0xbc042477, /// 0x748
			 0x9f22a23e, /// 0x74c
			 0x16b5258f, /// 0x750
			 0x35444efc, /// 0x754
			 0x45cd632d, /// 0x758
			 0xf3662f42, /// 0x75c
			 0x85ab9ddf, /// 0x760
			 0x66b0f283, /// 0x764
			 0x20afff77, /// 0x768
			 0x9906cd10, /// 0x76c
			 0x71caaa47, /// 0x770
			 0x2d77caa2, /// 0x774
			 0xc978abb8, /// 0x778
			 0xc9cca4ca, /// 0x77c
			 0x41444e3f, /// 0x780
			 0x1968c53c, /// 0x784
			 0xf069e898, /// 0x788
			 0xd508f1a2, /// 0x78c
			 0x35630e6e, /// 0x790
			 0x6d04dbc3, /// 0x794
			 0x9fe614cb, /// 0x798
			 0x78af9369, /// 0x79c
			 0x0a7e32ee, /// 0x7a0
			 0xdd1b2ce9, /// 0x7a4
			 0x3fb637cf, /// 0x7a8
			 0xde3c3ba9, /// 0x7ac
			 0x8db6d94d, /// 0x7b0
			 0xc7ee3461, /// 0x7b4
			 0x6b192975, /// 0x7b8
			 0xa1df14f5, /// 0x7bc
			 0x1d4e5743, /// 0x7c0
			 0xf169ab4f, /// 0x7c4
			 0x5b49f0e5, /// 0x7c8
			 0xf8c21bed, /// 0x7cc
			 0x6504395a, /// 0x7d0
			 0xa3495ead, /// 0x7d4
			 0x15f334e8, /// 0x7d8
			 0x4e1f8047, /// 0x7dc
			 0xf5364d33, /// 0x7e0
			 0xe92b8825, /// 0x7e4
			 0x0b6be9d8, /// 0x7e8
			 0x0bf8cdde, /// 0x7ec
			 0x31c878af, /// 0x7f0
			 0x1780a599, /// 0x7f4
			 0x9e6e9d90, /// 0x7f8
			 0x923113cb, /// 0x7fc
			 0xddafe008, /// 0x800
			 0x52f7df93, /// 0x804
			 0x4b92a2ba, /// 0x808
			 0xfdb8f3cc, /// 0x80c
			 0xdc716ab5, /// 0x810
			 0x57d16087, /// 0x814
			 0xe7839433, /// 0x818
			 0xf5336c9b, /// 0x81c
			 0x55ed5931, /// 0x820
			 0xec25b3c2, /// 0x824
			 0xbc27bcd2, /// 0x828
			 0x715fe81b, /// 0x82c
			 0x67a3026f, /// 0x830
			 0x111eb9e2, /// 0x834
			 0x908f56b8, /// 0x838
			 0x42ece21e, /// 0x83c
			 0xabf423d5, /// 0x840
			 0x250d7620, /// 0x844
			 0x0ea9a0bd, /// 0x848
			 0x3469d46d, /// 0x84c
			 0x41a5b0e3, /// 0x850
			 0xa724d1e7, /// 0x854
			 0x1cf00a73, /// 0x858
			 0x36b113b1, /// 0x85c
			 0x4efb0f2f, /// 0x860
			 0x7c1f7457, /// 0x864
			 0x648c723a, /// 0x868
			 0x5410438e, /// 0x86c
			 0x4be2fc2f, /// 0x870
			 0x2c00fe16, /// 0x874
			 0x3044e167, /// 0x878
			 0xf2f62754, /// 0x87c
			 0xc4ed9bd0, /// 0x880
			 0x824a7ff9, /// 0x884
			 0x5b7e338a, /// 0x888
			 0x49179cfd, /// 0x88c
			 0x9fb87466, /// 0x890
			 0x280d3116, /// 0x894
			 0xb8aefe39, /// 0x898
			 0x1d458a9d, /// 0x89c
			 0x3eb20622, /// 0x8a0
			 0x6c56baef, /// 0x8a4
			 0x21ed0737, /// 0x8a8
			 0x2395b202, /// 0x8ac
			 0xd3f6d6cf, /// 0x8b0
			 0xdb2832f5, /// 0x8b4
			 0xb425a96d, /// 0x8b8
			 0xa49eb1ab, /// 0x8bc
			 0xde0a9263, /// 0x8c0
			 0x400f2f5b, /// 0x8c4
			 0x0a1acf6f, /// 0x8c8
			 0xa686d055, /// 0x8cc
			 0x9c315b30, /// 0x8d0
			 0x382dd764, /// 0x8d4
			 0x4ad532c6, /// 0x8d8
			 0x631d49b2, /// 0x8dc
			 0xc915f272, /// 0x8e0
			 0xeae4a8ba, /// 0x8e4
			 0x041c7639, /// 0x8e8
			 0x26a1b75a, /// 0x8ec
			 0x4faeeb40, /// 0x8f0
			 0xcf70e936, /// 0x8f4
			 0xd3a9a249, /// 0x8f8
			 0x7cc48c82, /// 0x8fc
			 0x637ca3c5, /// 0x900
			 0x0e0495d2, /// 0x904
			 0x2e11b28a, /// 0x908
			 0xfed9955b, /// 0x90c
			 0xe2be7b48, /// 0x910
			 0xdf6de2e1, /// 0x914
			 0x0cb536ce, /// 0x918
			 0x9e9fa93b, /// 0x91c
			 0x0ef30e26, /// 0x920
			 0xecf100af, /// 0x924
			 0xa2e9dc80, /// 0x928
			 0x8ac58a7d, /// 0x92c
			 0x0badca4d, /// 0x930
			 0x873d4966, /// 0x934
			 0xe6e1613b, /// 0x938
			 0x4a86335f, /// 0x93c
			 0x4febc133, /// 0x940
			 0x84df450c, /// 0x944
			 0x9d5e271b, /// 0x948
			 0x7d38a7fc, /// 0x94c
			 0x23e3317b, /// 0x950
			 0xcf9942b4, /// 0x954
			 0x04ed5bb1, /// 0x958
			 0x2c34220d, /// 0x95c
			 0x85208d45, /// 0x960
			 0x06fcdcaa, /// 0x964
			 0x1a1f36d0, /// 0x968
			 0xcc048862, /// 0x96c
			 0xb9603c33, /// 0x970
			 0x2372fcb6, /// 0x974
			 0x1f10ea11, /// 0x978
			 0x1337ea80, /// 0x97c
			 0xeb9fb210, /// 0x980
			 0x5f71ddac, /// 0x984
			 0xf1aacef4, /// 0x988
			 0x2a2c4204, /// 0x98c
			 0xf7ac830b, /// 0x990
			 0x8bee06eb, /// 0x994
			 0x4487a2c8, /// 0x998
			 0xace7f56d, /// 0x99c
			 0x591517a3, /// 0x9a0
			 0x6e17f40e, /// 0x9a4
			 0xdf1c1e4b, /// 0x9a8
			 0x838bd3b0, /// 0x9ac
			 0xab943fd5, /// 0x9b0
			 0xa979464f, /// 0x9b4
			 0x90b41e4e, /// 0x9b8
			 0xf0bf2e06, /// 0x9bc
			 0xbdbfc23e, /// 0x9c0
			 0x2b02787b, /// 0x9c4
			 0x211ab8b8, /// 0x9c8
			 0x949190f8, /// 0x9cc
			 0x83aaba3f, /// 0x9d0
			 0x09750dc3, /// 0x9d4
			 0x9c0e9062, /// 0x9d8
			 0xebab1f04, /// 0x9dc
			 0x44bb3e67, /// 0x9e0
			 0xdf491d51, /// 0x9e4
			 0x4ce5813b, /// 0x9e8
			 0x709bd056, /// 0x9ec
			 0xdbab0456, /// 0x9f0
			 0xe58fe3f6, /// 0x9f4
			 0xf8528398, /// 0x9f8
			 0x5b9807cc, /// 0x9fc
			 0xa9a144d1, /// 0xa00
			 0x8a5cbbb6, /// 0xa04
			 0xb2aa26b5, /// 0xa08
			 0xd49349eb, /// 0xa0c
			 0xeb9a7d04, /// 0xa10
			 0x966e8247, /// 0xa14
			 0xf68b6120, /// 0xa18
			 0xb8e96ae1, /// 0xa1c
			 0x26fe428a, /// 0xa20
			 0xa46f795a, /// 0xa24
			 0xe29690d5, /// 0xa28
			 0x162976df, /// 0xa2c
			 0xa9f8e46c, /// 0xa30
			 0x1ffc1394, /// 0xa34
			 0x842dc62a, /// 0xa38
			 0xe3ae575b, /// 0xa3c
			 0x554ac51b, /// 0xa40
			 0x9eedd500, /// 0xa44
			 0x10edbf1c, /// 0xa48
			 0x49fe8f9a, /// 0xa4c
			 0x67929253, /// 0xa50
			 0x307f6711, /// 0xa54
			 0xe9e156ac, /// 0xa58
			 0xe604cfa3, /// 0xa5c
			 0xf071a24a, /// 0xa60
			 0x85e2a18f, /// 0xa64
			 0x3f9dbfa2, /// 0xa68
			 0xcd067355, /// 0xa6c
			 0xdf45830c, /// 0xa70
			 0xef3d30e5, /// 0xa74
			 0xbcc14729, /// 0xa78
			 0xf2756524, /// 0xa7c
			 0x1cc21511, /// 0xa80
			 0x5e1c6367, /// 0xa84
			 0x17015bcb, /// 0xa88
			 0xdf6919fb, /// 0xa8c
			 0xcf2228d8, /// 0xa90
			 0x8a94440b, /// 0xa94
			 0x2ac62de7, /// 0xa98
			 0x007751cc, /// 0xa9c
			 0x8556e51b, /// 0xaa0
			 0x6fbd962f, /// 0xaa4
			 0x183a983d, /// 0xaa8
			 0x435cec75, /// 0xaac
			 0x2b9befad, /// 0xab0
			 0x5bd80908, /// 0xab4
			 0xb1789430, /// 0xab8
			 0xf8658b41, /// 0xabc
			 0xec86b3e5, /// 0xac0
			 0x44ea8d4f, /// 0xac4
			 0x33c1996d, /// 0xac8
			 0x46422539, /// 0xacc
			 0xcded319d, /// 0xad0
			 0x0c2184d3, /// 0xad4
			 0xda84ad11, /// 0xad8
			 0x10edeb75, /// 0xadc
			 0xcac5bff4, /// 0xae0
			 0x70abd8f7, /// 0xae4
			 0x975365aa, /// 0xae8
			 0x0e6ebb55, /// 0xaec
			 0xf490e9f1, /// 0xaf0
			 0x1d93c9b2, /// 0xaf4
			 0xb5aff928, /// 0xaf8
			 0x622e3077, /// 0xafc
			 0x118d191e, /// 0xb00
			 0xa7b61d20, /// 0xb04
			 0x90f1407d, /// 0xb08
			 0x104f7d08, /// 0xb0c
			 0xa12b388e, /// 0xb10
			 0x26d2c64b, /// 0xb14
			 0xef8bea25, /// 0xb18
			 0xfc01ac7b, /// 0xb1c
			 0xa2b1ab94, /// 0xb20
			 0x9d1fa5cd, /// 0xb24
			 0xfea4fb35, /// 0xb28
			 0xa3964832, /// 0xb2c
			 0x088211f7, /// 0xb30
			 0x14c10f14, /// 0xb34
			 0xb66b3d3c, /// 0xb38
			 0xb28a085c, /// 0xb3c
			 0xc2fc2dcc, /// 0xb40
			 0x28cff95b, /// 0xb44
			 0x45de0a42, /// 0xb48
			 0x016987f5, /// 0xb4c
			 0x9594ad1e, /// 0xb50
			 0xb42ed7c5, /// 0xb54
			 0x77a84d69, /// 0xb58
			 0x411301e3, /// 0xb5c
			 0x1e5bcda6, /// 0xb60
			 0x4b67755f, /// 0xb64
			 0xb8b9f332, /// 0xb68
			 0x3d90bd66, /// 0xb6c
			 0x3540da01, /// 0xb70
			 0x8ef1dcc7, /// 0xb74
			 0x176cc290, /// 0xb78
			 0xfdf7dd18, /// 0xb7c
			 0x808c4c2b, /// 0xb80
			 0xf50a5fad, /// 0xb84
			 0x5c2d8af8, /// 0xb88
			 0x26a816fe, /// 0xb8c
			 0xdf4c6f3c, /// 0xb90
			 0x26255117, /// 0xb94
			 0xc4411157, /// 0xb98
			 0xf662d2f9, /// 0xb9c
			 0x4fa7cb45, /// 0xba0
			 0x689e50f2, /// 0xba4
			 0x547b5404, /// 0xba8
			 0x23c7fccf, /// 0xbac
			 0x3821afd2, /// 0xbb0
			 0x8656d9a8, /// 0xbb4
			 0x4e3ee54a, /// 0xbb8
			 0x22a0f7a0, /// 0xbbc
			 0x0f96bb34, /// 0xbc0
			 0x226aa4ac, /// 0xbc4
			 0xc4be5e69, /// 0xbc8
			 0x80104ba8, /// 0xbcc
			 0x1d490690, /// 0xbd0
			 0x4695d329, /// 0xbd4
			 0xe6149e45, /// 0xbd8
			 0x2fb31b65, /// 0xbdc
			 0x69060732, /// 0xbe0
			 0x140cedee, /// 0xbe4
			 0xb3273c86, /// 0xbe8
			 0xe17d9b55, /// 0xbec
			 0xc7640cb6, /// 0xbf0
			 0x14d30209, /// 0xbf4
			 0xbb10f898, /// 0xbf8
			 0x87ccb058, /// 0xbfc
			 0xc8884323, /// 0xc00
			 0x8a79b270, /// 0xc04
			 0x78b66f19, /// 0xc08
			 0xb86be72f, /// 0xc0c
			 0xaaa41718, /// 0xc10
			 0xd2f13cac, /// 0xc14
			 0xdd3db3bc, /// 0xc18
			 0x6337def6, /// 0xc1c
			 0xfe2fd0cd, /// 0xc20
			 0xc52e63d8, /// 0xc24
			 0xc4f1a24d, /// 0xc28
			 0x1af831c3, /// 0xc2c
			 0xba55ab45, /// 0xc30
			 0x2350e23b, /// 0xc34
			 0x625032c6, /// 0xc38
			 0xf1d82bf7, /// 0xc3c
			 0x719a26b8, /// 0xc40
			 0xf9ea93d3, /// 0xc44
			 0xc8420d97, /// 0xc48
			 0xd0f8ed30, /// 0xc4c
			 0xe2819f45, /// 0xc50
			 0x87f5b52e, /// 0xc54
			 0x2e97ccb8, /// 0xc58
			 0x0bd86c59, /// 0xc5c
			 0x9ae3372d, /// 0xc60
			 0xc05239d0, /// 0xc64
			 0x752e93a9, /// 0xc68
			 0x7e12c199, /// 0xc6c
			 0x03661b7a, /// 0xc70
			 0x762e65d4, /// 0xc74
			 0x098c0926, /// 0xc78
			 0xe6f1188e, /// 0xc7c
			 0x3fb3b613, /// 0xc80
			 0x6e4382b0, /// 0xc84
			 0xdb0ee5aa, /// 0xc88
			 0xdb33232a, /// 0xc8c
			 0x55becf59, /// 0xc90
			 0xaa3f0cc6, /// 0xc94
			 0x7807bb06, /// 0xc98
			 0x08e5ea5c, /// 0xc9c
			 0x0ecc5749, /// 0xca0
			 0x020818ed, /// 0xca4
			 0xa13735e1, /// 0xca8
			 0x42c2e26c, /// 0xcac
			 0x1b2c34bb, /// 0xcb0
			 0x921b0017, /// 0xcb4
			 0x7980ef62, /// 0xcb8
			 0x79688a64, /// 0xcbc
			 0x87252efb, /// 0xcc0
			 0x17e82a67, /// 0xcc4
			 0x4723d920, /// 0xcc8
			 0x6250db60, /// 0xccc
			 0xd0c58d17, /// 0xcd0
			 0x06eafb2f, /// 0xcd4
			 0xd20dbd8c, /// 0xcd8
			 0x3ed45c36, /// 0xcdc
			 0xdbfab51c, /// 0xce0
			 0xdaa4e627, /// 0xce4
			 0xdcfa091d, /// 0xce8
			 0x95f6d625, /// 0xcec
			 0xe807b284, /// 0xcf0
			 0x83fdd342, /// 0xcf4
			 0x89d5fcd7, /// 0xcf8
			 0x1335bc90, /// 0xcfc
			 0x462a391c, /// 0xd00
			 0x5f9b3e42, /// 0xd04
			 0xf4f68cad, /// 0xd08
			 0xbb6a0a8c, /// 0xd0c
			 0x16b81482, /// 0xd10
			 0x0fd6a64a, /// 0xd14
			 0xd824c557, /// 0xd18
			 0x35164bad, /// 0xd1c
			 0xa2571bb1, /// 0xd20
			 0xafaae450, /// 0xd24
			 0xc66edfd5, /// 0xd28
			 0x409dece8, /// 0xd2c
			 0xc5bb377e, /// 0xd30
			 0xd816c4e4, /// 0xd34
			 0x4e377c95, /// 0xd38
			 0x040ac5e2, /// 0xd3c
			 0xbd9dc9b2, /// 0xd40
			 0x510a7acf, /// 0xd44
			 0x52fec789, /// 0xd48
			 0xe7607c21, /// 0xd4c
			 0x42c0a804, /// 0xd50
			 0x902a0f5a, /// 0xd54
			 0x7c6006c9, /// 0xd58
			 0x9d490af0, /// 0xd5c
			 0x74f4fbe6, /// 0xd60
			 0xc7d693a9, /// 0xd64
			 0x554c701d, /// 0xd68
			 0x79e772c5, /// 0xd6c
			 0xd1448662, /// 0xd70
			 0xe72f3225, /// 0xd74
			 0x35662a2d, /// 0xd78
			 0x5585e5d7, /// 0xd7c
			 0x1d22f22b, /// 0xd80
			 0x5b06283a, /// 0xd84
			 0x5bff9619, /// 0xd88
			 0x3702053c, /// 0xd8c
			 0xe5ed840c, /// 0xd90
			 0x7744583b, /// 0xd94
			 0xec1cb583, /// 0xd98
			 0x201bc0e6, /// 0xd9c
			 0xbe3fb5b8, /// 0xda0
			 0x1f406c6a, /// 0xda4
			 0x03d0e46f, /// 0xda8
			 0x042804ba, /// 0xdac
			 0xbca6d16f, /// 0xdb0
			 0x10471315, /// 0xdb4
			 0xe805614d, /// 0xdb8
			 0xac1c72ee, /// 0xdbc
			 0x4da81b34, /// 0xdc0
			 0xc3258b54, /// 0xdc4
			 0x8dca9c9a, /// 0xdc8
			 0x79bb67c3, /// 0xdcc
			 0x7844bc81, /// 0xdd0
			 0xbe650b1e, /// 0xdd4
			 0xc9b03fc6, /// 0xdd8
			 0x50e1cc9f, /// 0xddc
			 0xc6f225a1, /// 0xde0
			 0xee58abff, /// 0xde4
			 0x223e2040, /// 0xde8
			 0x1f593e80, /// 0xdec
			 0xaf97d779, /// 0xdf0
			 0x21b947e1, /// 0xdf4
			 0x2d84e7a8, /// 0xdf8
			 0xb4aa1444, /// 0xdfc
			 0x9e082d92, /// 0xe00
			 0x1e7f0a23, /// 0xe04
			 0x0c880e60, /// 0xe08
			 0xea5ce03e, /// 0xe0c
			 0xc49e3557, /// 0xe10
			 0x4b9c3860, /// 0xe14
			 0x19d0f299, /// 0xe18
			 0xf8e46baa, /// 0xe1c
			 0xe20aabf6, /// 0xe20
			 0x46dba9cb, /// 0xe24
			 0x891b39bb, /// 0xe28
			 0xf630bd31, /// 0xe2c
			 0x5f83f540, /// 0xe30
			 0x3f628613, /// 0xe34
			 0xdb8fd474, /// 0xe38
			 0xc892f7ed, /// 0xe3c
			 0xac7721ac, /// 0xe40
			 0xb9d3a0b5, /// 0xe44
			 0x7ddadb33, /// 0xe48
			 0x5aaa62c5, /// 0xe4c
			 0x352a478b, /// 0xe50
			 0xb08c106a, /// 0xe54
			 0x8bec7707, /// 0xe58
			 0xb3e640ae, /// 0xe5c
			 0x62b7c954, /// 0xe60
			 0xdae727ba, /// 0xe64
			 0x8e001e2b, /// 0xe68
			 0xd9710b78, /// 0xe6c
			 0x4084b838, /// 0xe70
			 0xee0d16f6, /// 0xe74
			 0x4739f67b, /// 0xe78
			 0xcc47eea6, /// 0xe7c
			 0xca575a6b, /// 0xe80
			 0x503d5402, /// 0xe84
			 0xb7eec728, /// 0xe88
			 0xb55de452, /// 0xe8c
			 0xf9575a10, /// 0xe90
			 0xc9c9e3d5, /// 0xe94
			 0x0b92c170, /// 0xe98
			 0xc8bfa745, /// 0xe9c
			 0x5631ded1, /// 0xea0
			 0xb72e3d4a, /// 0xea4
			 0x7cbe9cd5, /// 0xea8
			 0x5c3ae5f6, /// 0xeac
			 0x6ab3d043, /// 0xeb0
			 0xa08b0d7c, /// 0xeb4
			 0x99330b29, /// 0xeb8
			 0x24e97eb4, /// 0xebc
			 0x995edb53, /// 0xec0
			 0xc7471859, /// 0xec4
			 0xe362a3b9, /// 0xec8
			 0x220a5896, /// 0xecc
			 0x12fe9a2e, /// 0xed0
			 0x756923e6, /// 0xed4
			 0xabe0dc05, /// 0xed8
			 0xb534da91, /// 0xedc
			 0x0103ea5d, /// 0xee0
			 0x919ad34e, /// 0xee4
			 0x01f603d2, /// 0xee8
			 0xef1cd3b5, /// 0xeec
			 0x5226450a, /// 0xef0
			 0xf88a66c7, /// 0xef4
			 0x69684933, /// 0xef8
			 0x8cc60f28, /// 0xefc
			 0xc778b689, /// 0xf00
			 0xffbd010d, /// 0xf04
			 0xa4d9634c, /// 0xf08
			 0xcb75888f, /// 0xf0c
			 0xdb3bc33a, /// 0xf10
			 0x7cef29e6, /// 0xf14
			 0x532d3765, /// 0xf18
			 0x928a8fbc, /// 0xf1c
			 0x47dc794d, /// 0xf20
			 0xa921525a, /// 0xf24
			 0x1ee73d23, /// 0xf28
			 0xa3c4a772, /// 0xf2c
			 0x1b6c8381, /// 0xf30
			 0x02beb5aa, /// 0xf34
			 0x4e45e419, /// 0xf38
			 0x613a505d, /// 0xf3c
			 0x6c8a44dc, /// 0xf40
			 0x65e63ed9, /// 0xf44
			 0x8ff47887, /// 0xf48
			 0x471b29fd, /// 0xf4c
			 0x2f4d8741, /// 0xf50
			 0x44e27925, /// 0xf54
			 0x03070756, /// 0xf58
			 0x99243858, /// 0xf5c
			 0x0d0f5893, /// 0xf60
			 0x94369194, /// 0xf64
			 0xf27142c1, /// 0xf68
			 0x41b6b014, /// 0xf6c
			 0x7081ec8b, /// 0xf70
			 0xe0ce8b97, /// 0xf74
			 0x1f9b9573, /// 0xf78
			 0xa44ce1d6, /// 0xf7c
			 0x0370e3bb, /// 0xf80
			 0x1a46a76c, /// 0xf84
			 0xe04df5fd, /// 0xf88
			 0x735abb56, /// 0xf8c
			 0x49af306e, /// 0xf90
			 0xccc84043, /// 0xf94
			 0x19e27504, /// 0xf98
			 0xf3d13de0, /// 0xf9c
			 0xea4401fc, /// 0xfa0
			 0x252888a9, /// 0xfa4
			 0x0a5f0485, /// 0xfa8
			 0x4260ec91, /// 0xfac
			 0x7e2c604c, /// 0xfb0
			 0x4e0b81e4, /// 0xfb4
			 0x038b628f, /// 0xfb8
			 0xf257b6d7, /// 0xfbc
			 0xa4620947, /// 0xfc0
			 0x1ca325fa, /// 0xfc4
			 0xbdb9043e, /// 0xfc8
			 0x6979a841, /// 0xfcc
			 0x72cb2ee5, /// 0xfd0
			 0x98831779, /// 0xfd4
			 0x18a18fc1, /// 0xfd8
			 0xcbf8ada0, /// 0xfdc
			 0x57f5e488, /// 0xfe0
			 0x18bdb046, /// 0xfe4
			 0xb47855d2, /// 0xfe8
			 0xc40ed66c, /// 0xfec
			 0x243f5b21, /// 0xff0
			 0xedf26cc5, /// 0xff4
			 0xd42a2362, /// 0xff8
			 0x10c957bb /// last
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
			 0x0000061c,
			 0x000004b0,
			 0x000007cc,
			 0x00000280,
			 0x000002a4,
			 0x00000480,
			 0x0000027c,
			 0x0000003c,

			 /// vpu register f2
			 0x41800000,
			 0x40e00000,
			 0x41b00000,
			 0x41d00000,
			 0x40400000,
			 0x41f00000,
			 0x41700000,
			 0x41900000,

			 /// vpu register f3
			 0x40800000,
			 0x41100000,
			 0x41100000,
			 0x41100000,
			 0x41e80000,
			 0x40a00000,
			 0x41d00000,
			 0x40c00000,

			 /// vpu register f4
			 0x40e00000,
			 0x40e00000,
			 0x41000000,
			 0x41100000,
			 0x41500000,
			 0x41300000,
			 0x41100000,
			 0x40800000,

			 /// vpu register f5
			 0x41880000,
			 0x40000000,
			 0x41880000,
			 0x41500000,
			 0x41700000,
			 0x41e00000,
			 0x41b00000,
			 0x41880000,

			 /// vpu register f6
			 0x41200000,
			 0x40800000,
			 0x41a80000,
			 0x40000000,
			 0x3f800000,
			 0x41700000,
			 0x40800000,
			 0x41c00000,

			 /// vpu register f7
			 0x41a00000,
			 0x40e00000,
			 0x41a80000,
			 0x40000000,
			 0x41900000,
			 0x41700000,
			 0x41e00000,
			 0x41e80000,

			 /// vpu register f8
			 0x40e00000,
			 0x41880000,
			 0x41000000,
			 0x40a00000,
			 0x41e00000,
			 0x41980000,
			 0x40a00000,
			 0x41100000,

			 /// vpu register f9
			 0x41a80000,
			 0x41b00000,
			 0x40000000,
			 0x41d00000,
			 0x41600000,
			 0x41700000,
			 0x41600000,
			 0x41b00000,

			 /// vpu register f10
			 0x40000000,
			 0x40000000,
			 0x41d00000,
			 0x40e00000,
			 0x41d00000,
			 0x41980000,
			 0x40400000,
			 0x41000000,

			 /// vpu register f11
			 0x41880000,
			 0x42000000,
			 0x40400000,
			 0x41c00000,
			 0x41a80000,
			 0x41300000,
			 0x41c80000,
			 0x40a00000,

			 /// vpu register f12
			 0x41980000,
			 0x41900000,
			 0x41300000,
			 0x40800000,
			 0x41b00000,
			 0x41200000,
			 0x41880000,
			 0x41c80000,

			 /// vpu register f13
			 0x40c00000,
			 0x41d80000,
			 0x41d00000,
			 0x41d00000,
			 0x41600000,
			 0x41c80000,
			 0x41300000,
			 0x41b80000,

			 /// vpu register f14
			 0x40000000,
			 0x41a80000,
			 0x42000000,
			 0x41c00000,
			 0x41b00000,
			 0x41a80000,
			 0x41e80000,
			 0x41500000,

			 /// vpu register f15
			 0x41700000,
			 0x41a80000,
			 0x40a00000,
			 0x41b00000,
			 0x41700000,
			 0x41900000,
			 0x40c00000,
			 0x41f00000,

			 /// vpu register f16
			 0x41200000,
			 0x42000000,
			 0x41000000,
			 0x41900000,
			 0x41d80000,
			 0x41e00000,
			 0x41d80000,
			 0x41b80000,

			 /// vpu register f17
			 0x41600000,
			 0x41f00000,
			 0x40800000,
			 0x41100000,
			 0x40000000,
			 0x41100000,
			 0x41880000,
			 0x40e00000,

			 /// vpu register f18
			 0x41980000,
			 0x41c80000,
			 0x41c80000,
			 0x40800000,
			 0x41f80000,
			 0x41500000,
			 0x41a80000,
			 0x41c00000,

			 /// vpu register f19
			 0x41880000,
			 0x40c00000,
			 0x41b00000,
			 0x41b80000,
			 0x41f80000,
			 0x40c00000,
			 0x40000000,
			 0x40c00000,

			 /// vpu register f20
			 0x42000000,
			 0x41200000,
			 0x40a00000,
			 0x41f00000,
			 0x40a00000,
			 0x41e00000,
			 0x41600000,
			 0x41500000,

			 /// vpu register f21
			 0x40e00000,
			 0x41a00000,
			 0x41880000,
			 0x41e00000,
			 0x40400000,
			 0x41d00000,
			 0x41e00000,
			 0x41f00000,

			 /// vpu register f22
			 0x41300000,
			 0x40e00000,
			 0x41200000,
			 0x41400000,
			 0x41800000,
			 0x42000000,
			 0x41000000,
			 0x40400000,

			 /// vpu register f23
			 0x3f800000,
			 0x41500000,
			 0x41c80000,
			 0x41900000,
			 0x41b80000,
			 0x41c00000,
			 0x41b00000,
			 0x41200000,

			 /// vpu register f24
			 0x40a00000,
			 0x41880000,
			 0x41f00000,
			 0x41d00000,
			 0x41d00000,
			 0x42000000,
			 0x41000000,
			 0x41100000,

			 /// vpu register f25
			 0x40a00000,
			 0x41300000,
			 0x41e00000,
			 0x41d00000,
			 0x41900000,
			 0x41f80000,
			 0x40400000,
			 0x41900000,

			 /// vpu register f26
			 0x40400000,
			 0x41c80000,
			 0x41f80000,
			 0x41880000,
			 0x41a80000,
			 0x41400000,
			 0x41700000,
			 0x41f80000,

			 /// vpu register f27
			 0x40c00000,
			 0x40c00000,
			 0x40c00000,
			 0x41500000,
			 0x40000000,
			 0x41800000,
			 0x41e00000,
			 0x40e00000,

			 /// vpu register f28
			 0x41d80000,
			 0x41300000,
			 0x41e80000,
			 0x41200000,
			 0x41200000,
			 0x40a00000,
			 0x41d80000,
			 0x41c80000,

			 /// vpu register f29
			 0x41b80000,
			 0x41a00000,
			 0x41a80000,
			 0x41a00000,
			 0x41000000,
			 0x41a80000,
			 0x40c00000,
			 0x41e80000,

			 /// vpu register f30
			 0x40800000,
			 0x41300000,
			 0x41100000,
			 0x41100000,
			 0x41b00000,
			 0x41a00000,
			 0x41100000,
			 0x41100000,

			 /// vpu register f31
			 0x41300000,
			 0x41c00000,
			 0x40c00000,
			 0x40000000,
			 0x42000000,
			 0x41a80000,
			 0x41700000,
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
		"mov.m.x m7, x12, 124\n"                              ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m5, x12, 74\n"                               ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m2, x11, 96\n"                               ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m0, x12, 121\n"                              ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m0, x15, 119\n"                              ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m7, x12, 87\n"                               ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m6, x14, 85\n"                               ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m4, x14, 103\n"                              ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m0, x12, 42\n"                               ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m7, x13, 36\n"                               ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m5, x12, 63\n"                               ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m3, x13, 106\n"                              ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m5, x14, 27\n"                               ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m1, x14, 121\n"                              ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m7, x12, 48\n"                               ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m1, x11, 36\n"                               ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m6, x15, 93\n"                               ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m1, x12, 37\n"                               ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m1, x11, 56\n"                               ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m2, x11, 25\n"                               ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m3, x13, 70\n"                               ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m5, x14, 8\n"                                ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m5, x13, 5\n"                                ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m1, x11, 84\n"                               ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m3, x14, 79\n"                               ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m4, x12, 83\n"                               ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m7, x11, 82\n"                               ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m1, x13, 37\n"                               ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m1, x14, 1\n"                                ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m7, x15, 14\n"                               ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m6, x13, 13\n"                               ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m7, x15, 1\n"                                ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m0, x11, 23\n"                               ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m2, x13, 67\n"                               ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m2, x11, 104\n"                              ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m4, x11, 13\n"                               ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m1, x13, 18\n"                               ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m6, x12, 1\n"                                ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m4, x12, 92\n"                               ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m3, x11, 58\n"                               ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m2, x15, 82\n"                               ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m2, x13, 50\n"                               ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m1, x12, 44\n"                               ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m1, x14, 100\n"                              ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m6, x12, 102\n"                              ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m3, x11, 43\n"                               ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m3, x14, 15\n"                               ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m2, x12, 104\n"                              ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m3, x12, 111\n"                              ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m5, x12, 81\n"                               ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m4, x11, 109\n"                              ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m5, x14, 108\n"                              ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m7, x14, 113\n"                              ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m4, x12, 34\n"                               ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m3, x11, 102\n"                              ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m3, x11, 1\n"                                ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m5, x14, 27\n"                               ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m7, x12, 12\n"                               ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m7, x14, 92\n"                               ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m3, x12, 73\n"                               ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m3, x14, 14\n"                               ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m4, x11, 24\n"                               ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m5, x12, 70\n"                               ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m2, x12, 37\n"                               ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m1, x12, 56\n"                               ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m5, x11, 64\n"                               ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m2, x12, 93\n"                               ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m5, x13, 3\n"                                ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m1, x12, 34\n"                               ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m7, x11, 86\n"                               ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m1, x15, 1\n"                                ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m7, x11, 64\n"                               ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m7, x15, 97\n"                               ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m2, x12, 76\n"                               ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m2, x11, 33\n"                               ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m6, x13, 57\n"                               ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m7, x13, 83\n"                               ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m7, x14, 2\n"                                ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m7, x13, 84\n"                               ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m2, x11, 20\n"                               ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m1, x14, 80\n"                               ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m7, x11, 33\n"                               ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m2, x14, 22\n"                               ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m0, x12, 124\n"                              ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m3, x14, 125\n"                              ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m6, x15, 115\n"                              ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m3, x15, 116\n"                              ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m5, x13, 31\n"                               ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m4, x12, 103\n"                              ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m5, x11, 96\n"                               ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m2, x11, 122\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m1, x11, 26\n"                               ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m7, x13, 123\n"                              ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m6, x15, 35\n"                               ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m3, x14, 13\n"                               ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m1, x14, 16\n"                               ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m0, x15, 34\n"                               ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m5, x13, 36\n"                               ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mov.m.x m2, x14, 88\n"                               ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
