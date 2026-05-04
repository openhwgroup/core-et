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
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0000c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00010
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00018
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0001c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00020
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00024
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00028
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0002c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00030
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00034
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00038
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0003c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00040
			 0xff800000,                                                  // -inf                                        /// 00044
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00048
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0004c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00050
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00058
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00064
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00068
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00074
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00078
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00084
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00088
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0008c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00090
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00094
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00098
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0009c
			 0x0e000003,                                                  // Trailing 1s:                                /// 000a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000a8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000ac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000b0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000b4
			 0x7f800000,                                                  // inf                                         /// 000b8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000bc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000c8
			 0x0c400000,                                                  // Leading 1s:                                 /// 000cc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000d4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000d8
			 0x80000000,                                                  // -zero                                       /// 000dc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000e0
			 0x55555555,                                                  // 4 random values                             /// 000e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000ec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000f0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000f4
			 0x0e000001,                                                  // Trailing 1s:                                /// 000f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000fc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00104
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00110
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00114
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00118
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0011c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00120
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00124
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00130
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00138
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0013c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00140
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00144
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00148
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00150
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00154
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0015c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00160
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00164
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00168
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0016c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00170
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00174
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00178
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0017c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00180
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00184
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00188
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0018c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00190
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00194
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00198
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0019c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001a0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001a4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001a8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001ac
			 0xff800000,                                                  // -inf                                        /// 001b0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001b8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001bc
			 0x0e000001,                                                  // Trailing 1s:                                /// 001c0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001c4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001c8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001d0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001d4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001d8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x33333333,                                                  // 4 random values                             /// 001e4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001f0
			 0x33333333,                                                  // 4 random values                             /// 001f4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00208
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0020c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00214
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00218
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0021c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00224
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00228
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00230
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00234
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00238
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0023c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00240
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00248
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0024c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00250
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00254
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00258
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0025c
			 0x4b000000,                                                  // 8388608.0                                   /// 00260
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00264
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0026c
			 0x7fc00001,                                                  // signaling NaN                               /// 00270
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00278
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0027c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00280
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00284
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00290
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00298
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0029c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002a0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002a4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002ac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002b0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002b4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002b8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002bc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002c0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002c4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002c8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002cc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002d4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002d8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002dc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002e0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002e8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002ec
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002f0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002fc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00304
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00308
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0030c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00310
			 0x00011111,                                                  // 9.7958E-41                                  /// 00314
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00318
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0031c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00320
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00328
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0032c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00330
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00338
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0033c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00340
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00344
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00348
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0034c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00350
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00354
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00358
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0035c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00364
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00368
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0036c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00370
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00374
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0037c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00380
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00388
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0038c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00390
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00398
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0xcb000000,                                                  // -8388608.0                                  /// 003a0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003a4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003a8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003b0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003b8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003bc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003c4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003c8
			 0x55555555,                                                  // 4 random values                             /// 003cc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003d4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003d8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003dc
			 0x00000000,                                                  // zero                                        /// 003e0
			 0x4b000000,                                                  // 8388608.0                                   /// 003e4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003f4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003f8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00400
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00404
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0040c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00410
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00414
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00418
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00424
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00428
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0042c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00430
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00434
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00438
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00444
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00448
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0044c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00458
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00464
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0046c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00470
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00474
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00478
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0047c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00484
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00488
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0048c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00490
			 0x4b000000,                                                  // 8388608.0                                   /// 00494
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00498
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0049c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004a0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004a8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004b0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004b4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x0e000003,                                                  // Trailing 1s:                                /// 004bc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004c0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004c4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004c8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004cc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004d0
			 0xffc00001,                                                  // -signaling NaN                              /// 004d4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004d8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004e4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004e8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004f0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004f4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004f8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004fc
			 0x55555555,                                                  // 4 random values                             /// 00500
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00508
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0050c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00510
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00514
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00518
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0051c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00520
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00524
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00528
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0052c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00534
			 0x0c780000,                                                  // Leading 1s:                                 /// 00538
			 0x00000000,                                                  // zero                                        /// 0053c
			 0x4b000000,                                                  // 8388608.0                                   /// 00540
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00548
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0054c
			 0xbf028f5c,                                                  // -0.51                                       /// 00550
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00558
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0055c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00560
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00564
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00568
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0056c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00570
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00574
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00578
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00580
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00588
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0058c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00590
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00594
			 0x4b000000,                                                  // 8388608.0                                   /// 00598
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0059c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005a0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005a4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005b0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005b4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005b8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005c0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 005c8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005d4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005d8
			 0xbf028f5c,                                                  // -0.51                                       /// 005dc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005e0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005ec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005f0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005f8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00600
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00604
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0060c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00610
			 0x00000000,                                                  // zero                                        /// 00614
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00618
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0061c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00620
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00624
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00628
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0062c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00630
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00634
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00640
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00644
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00648
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0064c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00650
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00654
			 0x80000000,                                                  // -zero                                       /// 00658
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00660
			 0xffc00001,                                                  // -signaling NaN                              /// 00664
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00668
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0066c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00670
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00674
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00678
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x7f800000,                                                  // inf                                         /// 00684
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00688
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0068c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00690
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00694
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00698
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0069c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006a0
			 0x33333333,                                                  // 4 random values                             /// 006a4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006b0
			 0x0c400000,                                                  // Leading 1s:                                 /// 006b4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x00000000,                                                  // zero                                        /// 006bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 006c0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006d0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006d4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006d8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006dc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006e0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006e8
			 0xff800000,                                                  // -inf                                        /// 006ec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x33333333,                                                  // 4 random values                             /// 00704
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00708
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0070c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00710
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00714
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00718
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0071c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00720
			 0xbf028f5c,                                                  // -0.51                                       /// 00724
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00728
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0072c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00730
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00734
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00738
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0073c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00744
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00748
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0074c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0075c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00764
			 0x3f028f5c,                                                  // 0.51                                        /// 00768
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0076c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00774
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00778
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0077c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00780
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00784
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0078c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00794
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00798
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0079c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007a0
			 0x7fc00001,                                                  // signaling NaN                               /// 007a4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007a8
			 0xffc00001,                                                  // -signaling NaN                              /// 007ac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007b0
			 0x4b000000,                                                  // 8388608.0                                   /// 007b4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007b8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007bc
			 0x7fc00001,                                                  // signaling NaN                               /// 007c0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007c4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007c8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007cc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007d4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007dc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007e0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0xffc00001,                                                  // -signaling NaN                              /// 007ec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007f4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007fc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00800
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00808
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0080c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00810
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00814
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x00011111,                                                  // 9.7958E-41                                  /// 0081c
			 0x00000000,                                                  // zero                                        /// 00820
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00824
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0082c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00834
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0083c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00840
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00844
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00848
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0084c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00850
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00854
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00858
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0085c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x0c700000,                                                  // Leading 1s:                                 /// 00864
			 0x3f028f5c,                                                  // 0.51                                        /// 00868
			 0x0e000007,                                                  // Trailing 1s:                                /// 0086c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00870
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00874
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00878
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0087c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00884
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00894
			 0x0c600000,                                                  // Leading 1s:                                 /// 00898
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008a0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008a4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008a8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008ac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008b0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008b4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008b8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008bc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008c0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008c4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008c8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008d4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008d8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008e0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008e8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008ec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008f4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008f8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008fc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0xcb000000,                                                  // -8388608.0                                  /// 00904
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0090c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00914
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0091c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00920
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00924
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00928
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0092c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00930
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00934
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00938
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0093c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00940
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00944
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00948
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0094c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00958
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0095c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00964
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00968
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x7f800000,                                                  // inf                                         /// 00970
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00974
			 0x80011111,                                                  // -9.7958E-41                                 /// 00978
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00980
			 0x4b000000,                                                  // 8388608.0                                   /// 00984
			 0x0e000007,                                                  // Trailing 1s:                                /// 00988
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00990
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00998
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0099c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009a4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009a8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009ac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x00011111,                                                  // 9.7958E-41                                  /// 009b4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009bc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009c0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009c8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009cc
			 0x33333333,                                                  // 4 random values                             /// 009d0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 009d8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009e0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009e4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009e8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009f4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009fc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a00
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a0c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a10
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a14
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a18
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a1c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a20
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a24
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a28
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a2c
			 0x80000000,                                                  // -zero                                       /// 00a30
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a34
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a38
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a40
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a44
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a4c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a50
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a54
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a5c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a64
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a68
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a6c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a70
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a74
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a7c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a80
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a8c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a94
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a9c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00aa0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00aac
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ab8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ac4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ac8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00acc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ad0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ad4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ad8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00adc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ae4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00aec
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00af0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00af8
			 0x00000000,                                                  // zero                                        /// 00afc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b00
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b10
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b14
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b18
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b1c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b20
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b24
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b2c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b30
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b34
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b3c
			 0xcb000000,                                                  // -8388608.0                                  /// 00b40
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b48
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b54
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b58
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b5c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b60
			 0xcb000000,                                                  // -8388608.0                                  /// 00b64
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b68
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b6c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b70
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b78
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b88
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b8c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b98
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b9c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ba0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ba4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bb0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bbc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bc0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bc4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bc8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bcc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bd4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bd8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bdc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00be4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00be8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bf4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bfc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c04
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c08
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c0c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c14
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c20
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c24
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c28
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c2c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c30
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0xcb000000,                                                  // -8388608.0                                  /// 00c38
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c48
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c4c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c50
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c54
			 0x33333333,                                                  // 4 random values                             /// 00c58
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c60
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c64
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c68
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c70
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c74
			 0x7fc00001,                                                  // signaling NaN                               /// 00c78
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c7c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c84
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c88
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c8c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c90
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c94
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ca0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cb0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cb4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cb8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cbc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cc0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cc4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cc8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ccc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cd0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cd4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cd8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cdc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ce0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ce4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cf0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cf8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cfc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d00
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d04
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d08
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d0c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d10
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d1c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d20
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d24
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d28
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d30
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d38
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d40
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d44
			 0x7f800000,                                                  // inf                                         /// 00d48
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d4c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d58
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d5c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d68
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d6c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d74
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d80
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d84
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d88
			 0xcb000000,                                                  // -8388608.0                                  /// 00d8c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d90
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d94
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d98
			 0x7fc00001,                                                  // signaling NaN                               /// 00d9c
			 0xffc00001,                                                  // -signaling NaN                              /// 00da0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00da4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00da8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00db0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00db4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00db8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dbc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00dc0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00dc4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00dc8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dcc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00dd0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00dd8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ddc
			 0x55555555,                                                  // 4 random values                             /// 00de0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00df0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00dfc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e00
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e08
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e10
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e14
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e18
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e1c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e20
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e24
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e28
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e30
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e34
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e38
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e3c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e40
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e44
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e48
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e50
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e54
			 0xcb000000,                                                  // -8388608.0                                  /// 00e58
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e5c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e64
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e68
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e74
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e78
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e7c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e80
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e84
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e88
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e8c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e90
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e98
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ea0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ea4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00eac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00eb0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ec4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ec8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ecc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ed0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ed4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ed8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00edc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ee0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ee4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ee8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00eec
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ef0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ef4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f00
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f04
			 0x33333333,                                                  // 4 random values                             /// 00f08
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f0c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f10
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f14
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f1c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f20
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f24
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f28
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f2c
			 0xffc00001,                                                  // -signaling NaN                              /// 00f30
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f34
			 0x7f800000,                                                  // inf                                         /// 00f38
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f3c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f40
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f48
			 0xff800000,                                                  // -inf                                        /// 00f4c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f5c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f64
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f68
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f6c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f70
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f74
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f7c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f80
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f88
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f90
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f94
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x7fc00001,                                                  // signaling NaN                               /// 00f9c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fa0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fa8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fb0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fb4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fbc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fc0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fc4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00fcc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fd4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x33333333,                                                  // 4 random values                             /// 00fdc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00fe0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fe4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fe8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ff0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ff8
			 0x0d000ff0                                                  // Set of 1s                                   /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x55555555,                                                  // 4 random values                             /// 00000
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00004
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00008
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0000c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00018
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0001c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00020
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00024
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00030
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00034
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00038
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0003c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0004c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00050
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00054
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00058
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0005c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00074
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0007c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00084
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0008c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00090
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00094
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00098
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000a0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000a8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000ac
			 0x7f800000,                                                  // inf                                         /// 000b0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000b8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000c0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000c4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000c8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000cc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x80000000,                                                  // -zero                                       /// 000d4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x00000000,                                                  // zero                                        /// 000e0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000e4
			 0x33333333,                                                  // 4 random values                             /// 000e8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000f4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x00011111,                                                  // 9.7958E-41                                  /// 000fc
			 0xcb000000,                                                  // -8388608.0                                  /// 00100
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00104
			 0xff800000,                                                  // -inf                                        /// 00108
			 0x7f800000,                                                  // inf                                         /// 0010c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00114
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00118
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0011c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00120
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00128
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0012c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00134
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00138
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0013c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00140
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00148
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0014c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00150
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00154
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00158
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00160
			 0x0e000001,                                                  // Trailing 1s:                                /// 00164
			 0x0c780000,                                                  // Leading 1s:                                 /// 00168
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0016c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00170
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00174
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00178
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00180
			 0x0e000003,                                                  // Trailing 1s:                                /// 00184
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00188
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0018c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00194
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00198
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001a0
			 0x55555555,                                                  // 4 random values                             /// 001a4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001b8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001bc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001c0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001c4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001c8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001d0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001d4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001d8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001e0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001e4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001f0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001f4
			 0x0c400000,                                                  // Leading 1s:                                 /// 001f8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001fc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00200
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00204
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00208
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0020c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00210
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00214
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0021c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00220
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00224
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00228
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0022c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00230
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00234
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00238
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00240
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00248
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0024c
			 0x33333333,                                                  // 4 random values                             /// 00250
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00254
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0025c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00260
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00264
			 0x0c700000,                                                  // Leading 1s:                                 /// 00268
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0026c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00270
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00274
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00278
			 0x7fc00001,                                                  // signaling NaN                               /// 0027c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0028c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00290
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00294
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00298
			 0x0c600000,                                                  // Leading 1s:                                 /// 0029c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002a4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002a8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002ac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002b0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002b4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002c0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002c4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002c8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002d0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002d4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002d8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002dc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002e0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002e8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002ec
			 0xcb000000,                                                  // -8388608.0                                  /// 002f0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002f8
			 0x33333333,                                                  // 4 random values                             /// 002fc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00300
			 0x80011111,                                                  // -9.7958E-41                                 /// 00304
			 0x00011111,                                                  // 9.7958E-41                                  /// 00308
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0030c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00310
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00314
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00318
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00320
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0032c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00330
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00338
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0033c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00340
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0034c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00354
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00358
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0035c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00360
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00364
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00368
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0036c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0xff800000,                                                  // -inf                                        /// 00374
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00378
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0037c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00380
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00384
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00388
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0038c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00394
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00398
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0039c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003a0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003a4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003a8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003ac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003b0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003b4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003b8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003c4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003c8
			 0x33333333,                                                  // 4 random values                             /// 003cc
			 0x7f800000,                                                  // inf                                         /// 003d0
			 0xff800000,                                                  // -inf                                        /// 003d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 003d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003dc
			 0xffc00001,                                                  // -signaling NaN                              /// 003e0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003e8
			 0xffc00001,                                                  // -signaling NaN                              /// 003ec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003f0
			 0x00011111,                                                  // 9.7958E-41                                  /// 003f4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003f8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00400
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00404
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00408
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00410
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00414
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00418
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0041c
			 0x80000000,                                                  // -zero                                       /// 00420
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00428
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0042c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00430
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00434
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0xbf028f5c,                                                  // -0.51                                       /// 00440
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x0c600000,                                                  // Leading 1s:                                 /// 0044c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0045c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00464
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0046c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00470
			 0x0c600000,                                                  // Leading 1s:                                 /// 00474
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0047c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00480
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00484
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00488
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0048c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00490
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00494
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0049c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004a4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004a8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004b0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004b8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004bc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004c0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004c4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004c8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004cc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004d4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004d8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004dc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004e0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004e8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004f0
			 0x0c700000,                                                  // Leading 1s:                                 /// 004f4
			 0x3f028f5c,                                                  // 0.51                                        /// 004f8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0050c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00510
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00514
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00518
			 0xffc00001,                                                  // -signaling NaN                              /// 0051c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00520
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00524
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00528
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0052c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00530
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00534
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00538
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0053c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00544
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00548
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0054c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00550
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00554
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00558
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0055c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00560
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00564
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0056c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00570
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00578
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0057c
			 0xff800000,                                                  // -inf                                        /// 00580
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00584
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00588
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0058c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00590
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00594
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00598
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0059c
			 0xcb000000,                                                  // -8388608.0                                  /// 005a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005a4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005a8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005ac
			 0x80011111,                                                  // -9.7958E-41                                 /// 005b0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x00000000,                                                  // zero                                        /// 005c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005c8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005d0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005d4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005d8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005dc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005e0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005e8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005ec
			 0x80011111,                                                  // -9.7958E-41                                 /// 005f0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005f4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005fc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00600
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0060c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00614
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00618
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0061c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00620
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00624
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00628
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0062c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00630
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00634
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00638
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0063c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00640
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00644
			 0xbf028f5c,                                                  // -0.51                                       /// 00648
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0064c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00650
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00654
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00658
			 0xffc00001,                                                  // -signaling NaN                              /// 0065c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00660
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00664
			 0x00011111,                                                  // 9.7958E-41                                  /// 00668
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00678
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00680
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00684
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00688
			 0x7f800000,                                                  // inf                                         /// 0068c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00690
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00694
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00698
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006a0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006a4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006a8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006b0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006b4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006b8
			 0x0e000001,                                                  // Trailing 1s:                                /// 006bc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006c0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006c8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006d0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006dc
			 0xffc00001,                                                  // -signaling NaN                              /// 006e0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006e4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006e8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006ec
			 0x0c780000,                                                  // Leading 1s:                                 /// 006f0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006f4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00704
			 0x80000000,                                                  // -zero                                       /// 00708
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0070c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00710
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00718
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0071c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00720
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00728
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00734
			 0x7fc00001,                                                  // signaling NaN                               /// 00738
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00744
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00748
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00750
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00754
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00758
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0075c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00764
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00768
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0076c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00770
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00774
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00778
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0077c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00780
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00784
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00788
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0078c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00798
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0079c
			 0x0e000001,                                                  // Trailing 1s:                                /// 007a0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0x0e000001,                                                  // Trailing 1s:                                /// 007ac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007b0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007b4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007b8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007bc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007cc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007d0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007d4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007dc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007e0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007e4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007e8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007ec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007f0
			 0x0c780000,                                                  // Leading 1s:                                 /// 007f4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007f8
			 0x0c400000,                                                  // Leading 1s:                                 /// 007fc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00800
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00804
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0080c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00810
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00814
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00818
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0081c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0082c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00834
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0083c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00840
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00844
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00848
			 0x0c600000,                                                  // Leading 1s:                                 /// 0084c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00854
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0085c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00864
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00868
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0086c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00870
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00874
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00878
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00880
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00888
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0088c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00890
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00894
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00898
			 0x4b000000,                                                  // 8388608.0                                   /// 0089c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008a0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008a8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008ac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008b0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008b8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008bc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008c0
			 0xff800000,                                                  // -inf                                        /// 008c4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008cc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008d4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008d8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008e4
			 0xcb000000,                                                  // -8388608.0                                  /// 008e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008ec
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008f4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008f8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008fc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00900
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00908
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0090c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00914
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0091c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00920
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00928
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0092c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00930
			 0x0e000003,                                                  // Trailing 1s:                                /// 00934
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00938
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0093c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00944
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0094c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00950
			 0xcb000000,                                                  // -8388608.0                                  /// 00954
			 0x55555555,                                                  // 4 random values                             /// 00958
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0095c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00960
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00964
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0096c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00970
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00978
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00980
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00984
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0098c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00990
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00994
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00998
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009a4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009a8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009ac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009b0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 009b8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009bc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009c4
			 0x33333333,                                                  // 4 random values                             /// 009c8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009d0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009e0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009ec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009f0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009f4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a00
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a04
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a08
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a0c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a10
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a1c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a20
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a24
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a28
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a34
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a38
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a3c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a40
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a44
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a4c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a50
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a54
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a58
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a5c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a6c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a74
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a7c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a80
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a84
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a8c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a90
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a94
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00aa0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00aa8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00aac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ab0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ab4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ab8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00abc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ac0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ac4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0xbf028f5c,                                                  // -0.51                                       /// 00acc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ad0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ad4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ad8
			 0x80000000,                                                  // -zero                                       /// 00adc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ae0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ae4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00aec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00af0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00af4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00af8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b00
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b08
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b0c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b10
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b14
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b18
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b20
			 0xbf028f5c,                                                  // -0.51                                       /// 00b24
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b28
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b2c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b30
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b34
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b38
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b3c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b40
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b44
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b48
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b4c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b50
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b54
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b58
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b5c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b60
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b64
			 0x4b000000,                                                  // 8388608.0                                   /// 00b68
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b70
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b74
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b80
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b84
			 0xffc00001,                                                  // -signaling NaN                              /// 00b88
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b8c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b90
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b94
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b98
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b9c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ba0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ba8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bb0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00bc0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x55555555,                                                  // 4 random values                             /// 00bc8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bcc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bd0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bd4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bd8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bdc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00be0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00be4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00be8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bf0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00bf4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bfc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c00
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c04
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c08
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c0c
			 0x7fc00001,                                                  // signaling NaN                               /// 00c10
			 0x7f800000,                                                  // inf                                         /// 00c14
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c18
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c1c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c20
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c28
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c30
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c38
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c3c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c44
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c48
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c4c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c50
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c54
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c5c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c60
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c64
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c68
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c6c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c70
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c74
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c78
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c7c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c80
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c84
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c8c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c90
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c94
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x00000000,                                                  // zero                                        /// 00c9c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ca0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ca4
			 0x55555555,                                                  // 4 random values                             /// 00ca8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cb0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cb4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cb8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00cbc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cc0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cc4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ccc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cd4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cd8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ce0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0xbf028f5c,                                                  // -0.51                                       /// 00ce8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cf4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cf8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d00
			 0x00000000,                                                  // zero                                        /// 00d04
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d0c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d18
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d1c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d20
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d28
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d2c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d30
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d38
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d3c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d40
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d44
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d48
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d50
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d54
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d5c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d60
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d6c
			 0x4b000000,                                                  // 8388608.0                                   /// 00d70
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d7c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d80
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d84
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d88
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d8c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d90
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d94
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00da0
			 0xffc00001,                                                  // -signaling NaN                              /// 00da4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00dac
			 0x0c600000,                                                  // Leading 1s:                                 /// 00db0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00db4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00db8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00dbc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dc0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00dc4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00dc8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00dcc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00dd8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00de0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00de4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00de8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00df4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00df8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00dfc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e00
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e08
			 0x00000000,                                                  // zero                                        /// 00e0c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e10
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e14
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e18
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e20
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e24
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e2c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e30
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e38
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e3c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e44
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e48
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e4c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e50
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e58
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x7fc00001,                                                  // signaling NaN                               /// 00e60
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e64
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e68
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e6c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e70
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e74
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e78
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e80
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e88
			 0xffc00001,                                                  // -signaling NaN                              /// 00e8c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e90
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e94
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e98
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ea0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ea4
			 0xffc00001,                                                  // -signaling NaN                              /// 00ea8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0xff800000,                                                  // -inf                                        /// 00eb0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00eb8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ecc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ed4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ed8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x33333333,                                                  // 4 random values                             /// 00ee0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ee8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00eec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ef4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ef8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00efc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f00
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f04
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f0c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f10
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f14
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f18
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f1c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f20
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f24
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f28
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f34
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f38
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f3c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f40
			 0x55555555,                                                  // 4 random values                             /// 00f44
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f54
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f58
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f5c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f64
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f68
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f6c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f70
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f74
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f7c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f80
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f84
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f90
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f94
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f98
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f9c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fa0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fa4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x00000000,                                                  // zero                                        /// 00fb0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fb8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fc0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fc4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fcc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fd4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fd8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fdc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fe0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fe8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fec
			 0x7fc00001,                                                  // signaling NaN                               /// 00ff0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ff4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ff8
			 0x0e0001ff                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0xc2a507c1, /// 0x0
			 0xf747a443, /// 0x4
			 0xec78fc42, /// 0x8
			 0x053e465d, /// 0xc
			 0x2991344f, /// 0x10
			 0xf71e4e20, /// 0x14
			 0x4d540824, /// 0x18
			 0x0f7ddd9f, /// 0x1c
			 0x9efebc85, /// 0x20
			 0x2aeba7f3, /// 0x24
			 0x2d2c118b, /// 0x28
			 0xddc27712, /// 0x2c
			 0x99ca9653, /// 0x30
			 0x08f6dd4b, /// 0x34
			 0xb69c77c1, /// 0x38
			 0x52928b38, /// 0x3c
			 0xd8d3988d, /// 0x40
			 0xf8f102d5, /// 0x44
			 0x1acf147e, /// 0x48
			 0x17393f1f, /// 0x4c
			 0x7ea17419, /// 0x50
			 0xd83dcd46, /// 0x54
			 0xa41e88ab, /// 0x58
			 0x4b1cf410, /// 0x5c
			 0x96b34bc4, /// 0x60
			 0x39421746, /// 0x64
			 0x74533979, /// 0x68
			 0xdd9ec56b, /// 0x6c
			 0xa0f297f4, /// 0x70
			 0xa5cc6e2c, /// 0x74
			 0xed595c34, /// 0x78
			 0x34a341ef, /// 0x7c
			 0x0d608d2c, /// 0x80
			 0xdc252821, /// 0x84
			 0xfc02ea77, /// 0x88
			 0xc8fb1125, /// 0x8c
			 0x1a92614d, /// 0x90
			 0xfc8c05ee, /// 0x94
			 0x48149a6e, /// 0x98
			 0x5373e126, /// 0x9c
			 0x44362c7f, /// 0xa0
			 0x0b7a1ad9, /// 0xa4
			 0x66add849, /// 0xa8
			 0xeaf1c660, /// 0xac
			 0xde0a6ad5, /// 0xb0
			 0x72306e6f, /// 0xb4
			 0x8eb767d8, /// 0xb8
			 0xe279a6f1, /// 0xbc
			 0x41edd574, /// 0xc0
			 0xe551b9c4, /// 0xc4
			 0x00d79cc4, /// 0xc8
			 0x9095724c, /// 0xcc
			 0xd288a420, /// 0xd0
			 0x42e24998, /// 0xd4
			 0xa49a6e08, /// 0xd8
			 0x7a26ad24, /// 0xdc
			 0xfc472402, /// 0xe0
			 0x7bd8a954, /// 0xe4
			 0xdbba170a, /// 0xe8
			 0xc8792284, /// 0xec
			 0xb265e7f9, /// 0xf0
			 0x4ab11658, /// 0xf4
			 0x168f5a08, /// 0xf8
			 0xf60ec943, /// 0xfc
			 0x53b13060, /// 0x100
			 0xb139ecf3, /// 0x104
			 0x9fd6843c, /// 0x108
			 0x2d5a078e, /// 0x10c
			 0xbfebffa5, /// 0x110
			 0x9ab93d64, /// 0x114
			 0x42b1c20e, /// 0x118
			 0x2239820b, /// 0x11c
			 0xe4b583ba, /// 0x120
			 0xe816c2f5, /// 0x124
			 0x0ffac7b0, /// 0x128
			 0x1874c731, /// 0x12c
			 0x97a00a6f, /// 0x130
			 0x6869acc2, /// 0x134
			 0x1a101128, /// 0x138
			 0x4bf3d7d9, /// 0x13c
			 0x74cbfe9a, /// 0x140
			 0x6f31bff3, /// 0x144
			 0x93c6c734, /// 0x148
			 0x56e51e5d, /// 0x14c
			 0x747f96f8, /// 0x150
			 0x6abe8ebd, /// 0x154
			 0x87cd88d1, /// 0x158
			 0x4ccbe766, /// 0x15c
			 0x4d292f8e, /// 0x160
			 0xb0d8fa29, /// 0x164
			 0xf1d8900c, /// 0x168
			 0xe4866466, /// 0x16c
			 0x378a5879, /// 0x170
			 0x25e41d2b, /// 0x174
			 0xd152d799, /// 0x178
			 0x04d179ae, /// 0x17c
			 0xc57d37ab, /// 0x180
			 0x568216b3, /// 0x184
			 0x98b6700e, /// 0x188
			 0x0fdc27fd, /// 0x18c
			 0x0c4cbff6, /// 0x190
			 0xa52084a5, /// 0x194
			 0x85ac93b3, /// 0x198
			 0x4ea9d996, /// 0x19c
			 0x68fe6b79, /// 0x1a0
			 0x57bd028f, /// 0x1a4
			 0xad3b28f2, /// 0x1a8
			 0x360dcbc8, /// 0x1ac
			 0x88d9eb9d, /// 0x1b0
			 0x3f49a676, /// 0x1b4
			 0x1001c93a, /// 0x1b8
			 0xc956e5d3, /// 0x1bc
			 0xaaeba56d, /// 0x1c0
			 0x462a1cce, /// 0x1c4
			 0x37edf364, /// 0x1c8
			 0xcb848666, /// 0x1cc
			 0x2360a44c, /// 0x1d0
			 0x1f1b1c90, /// 0x1d4
			 0x31c1d8bd, /// 0x1d8
			 0x0a3f1809, /// 0x1dc
			 0xfec01e54, /// 0x1e0
			 0x9e0c1980, /// 0x1e4
			 0xa767afb5, /// 0x1e8
			 0x51952b1b, /// 0x1ec
			 0x55afcbf9, /// 0x1f0
			 0x4b24b8b2, /// 0x1f4
			 0xae5c599b, /// 0x1f8
			 0x7088ccca, /// 0x1fc
			 0x60438784, /// 0x200
			 0x24b6631d, /// 0x204
			 0x1c07c539, /// 0x208
			 0xa9cbb967, /// 0x20c
			 0x999c4506, /// 0x210
			 0x64d4778b, /// 0x214
			 0x2a548643, /// 0x218
			 0x6587059b, /// 0x21c
			 0x5e5c4a5c, /// 0x220
			 0x5cca2c6c, /// 0x224
			 0x157e9ac3, /// 0x228
			 0x5091ddaa, /// 0x22c
			 0x040dcd15, /// 0x230
			 0xb8fef87f, /// 0x234
			 0xccf8ba47, /// 0x238
			 0x0bc9b098, /// 0x23c
			 0x718531db, /// 0x240
			 0xaa8be94d, /// 0x244
			 0x1585f64a, /// 0x248
			 0xd88f8d7f, /// 0x24c
			 0xd41ccd65, /// 0x250
			 0xb7b7c840, /// 0x254
			 0x1e536876, /// 0x258
			 0x16fa28f2, /// 0x25c
			 0xe071cc38, /// 0x260
			 0xeab283bb, /// 0x264
			 0xff62be49, /// 0x268
			 0x92994d8e, /// 0x26c
			 0xbf8f881a, /// 0x270
			 0x1c357682, /// 0x274
			 0x5fa027d4, /// 0x278
			 0xd0050c1e, /// 0x27c
			 0xe5162c72, /// 0x280
			 0x36b72bda, /// 0x284
			 0xbab53677, /// 0x288
			 0xc379f4d5, /// 0x28c
			 0xbcfd1ce7, /// 0x290
			 0x092cc604, /// 0x294
			 0x380fd942, /// 0x298
			 0x138317a8, /// 0x29c
			 0x8cfc0e87, /// 0x2a0
			 0x294b9d4e, /// 0x2a4
			 0x132531b2, /// 0x2a8
			 0x076b146e, /// 0x2ac
			 0x745c9ee7, /// 0x2b0
			 0xd171b598, /// 0x2b4
			 0xa832ca29, /// 0x2b8
			 0x857a985d, /// 0x2bc
			 0x339557da, /// 0x2c0
			 0xa52bc8d5, /// 0x2c4
			 0xe1501e03, /// 0x2c8
			 0xfb1074cb, /// 0x2cc
			 0x18eb5a51, /// 0x2d0
			 0x617cabf0, /// 0x2d4
			 0xcce5086c, /// 0x2d8
			 0xdad7880a, /// 0x2dc
			 0x556b22b2, /// 0x2e0
			 0x19894e30, /// 0x2e4
			 0xabda1a3c, /// 0x2e8
			 0xf98fac2c, /// 0x2ec
			 0x1e4b248a, /// 0x2f0
			 0x884ebc28, /// 0x2f4
			 0x1922d7d1, /// 0x2f8
			 0xe2c837d9, /// 0x2fc
			 0xb0cbdd4d, /// 0x300
			 0x779b4824, /// 0x304
			 0x8f19f587, /// 0x308
			 0xac13ad51, /// 0x30c
			 0x85c165a8, /// 0x310
			 0x1c1c1fd4, /// 0x314
			 0x9c0904bc, /// 0x318
			 0x1807a0ad, /// 0x31c
			 0xb5250f75, /// 0x320
			 0x7a6d973d, /// 0x324
			 0x623c194a, /// 0x328
			 0x2ca01bf1, /// 0x32c
			 0xa4a2c129, /// 0x330
			 0xdaed1f7b, /// 0x334
			 0x4b030770, /// 0x338
			 0x2d6490a0, /// 0x33c
			 0xc2b0a5f0, /// 0x340
			 0x438cab93, /// 0x344
			 0x4f8ad0dc, /// 0x348
			 0xffffeee6, /// 0x34c
			 0xd82c24df, /// 0x350
			 0x96e63903, /// 0x354
			 0x3a0bca63, /// 0x358
			 0xf8962855, /// 0x35c
			 0xf176f842, /// 0x360
			 0x9e716709, /// 0x364
			 0xad947129, /// 0x368
			 0x2ee2fb56, /// 0x36c
			 0xe60d8d92, /// 0x370
			 0xe921cf80, /// 0x374
			 0x7a08ed50, /// 0x378
			 0x0c0fd869, /// 0x37c
			 0xa84d8644, /// 0x380
			 0x5b60f250, /// 0x384
			 0x0f129418, /// 0x388
			 0x6d41aff6, /// 0x38c
			 0x85f2b33a, /// 0x390
			 0xac413159, /// 0x394
			 0x82b236ae, /// 0x398
			 0xa2335373, /// 0x39c
			 0xa6584a40, /// 0x3a0
			 0x99c596d6, /// 0x3a4
			 0xae592249, /// 0x3a8
			 0x22505d28, /// 0x3ac
			 0xc657d5cc, /// 0x3b0
			 0x8f48159f, /// 0x3b4
			 0x0e864710, /// 0x3b8
			 0xc264f985, /// 0x3bc
			 0xbdf1327c, /// 0x3c0
			 0x458a7374, /// 0x3c4
			 0xdf6757d3, /// 0x3c8
			 0xc2f5d833, /// 0x3cc
			 0x6f3de021, /// 0x3d0
			 0xcf56a68d, /// 0x3d4
			 0x8912e67e, /// 0x3d8
			 0x5c3739dd, /// 0x3dc
			 0xf58fcc1e, /// 0x3e0
			 0xe23c4ec4, /// 0x3e4
			 0xa1cced9e, /// 0x3e8
			 0xc4d1ac77, /// 0x3ec
			 0xe23fe760, /// 0x3f0
			 0x7fb44d64, /// 0x3f4
			 0xacceec41, /// 0x3f8
			 0x613a5871, /// 0x3fc
			 0x5d605925, /// 0x400
			 0x5fa2fc7a, /// 0x404
			 0x839e9d71, /// 0x408
			 0x549aeb1b, /// 0x40c
			 0x24fa74da, /// 0x410
			 0x14928b66, /// 0x414
			 0xb5cb8e95, /// 0x418
			 0x0571d5b9, /// 0x41c
			 0x94bd8de3, /// 0x420
			 0x173dd0cb, /// 0x424
			 0xc613ea48, /// 0x428
			 0xc4b99a8b, /// 0x42c
			 0x6e41f945, /// 0x430
			 0xe509dc53, /// 0x434
			 0x526346a7, /// 0x438
			 0x94eed61f, /// 0x43c
			 0x05865a0f, /// 0x440
			 0x44478766, /// 0x444
			 0xe9ee652d, /// 0x448
			 0x1d310a8f, /// 0x44c
			 0x90d8e87c, /// 0x450
			 0x3acc86b8, /// 0x454
			 0xace3480a, /// 0x458
			 0x0eba45b5, /// 0x45c
			 0x9ce23384, /// 0x460
			 0xa5a94920, /// 0x464
			 0x3962b164, /// 0x468
			 0xb0e180b2, /// 0x46c
			 0x7a97deb2, /// 0x470
			 0x218f4dc6, /// 0x474
			 0xf64b4cd3, /// 0x478
			 0xaa0db77f, /// 0x47c
			 0x175b3b48, /// 0x480
			 0xb168e65e, /// 0x484
			 0x4ee2cc8a, /// 0x488
			 0x47ba7ba9, /// 0x48c
			 0xf54fd89e, /// 0x490
			 0xd7b50fab, /// 0x494
			 0x809bca60, /// 0x498
			 0xb96d9b00, /// 0x49c
			 0xd8e7d044, /// 0x4a0
			 0x8ab097c2, /// 0x4a4
			 0x288f3e6c, /// 0x4a8
			 0xda81a6d0, /// 0x4ac
			 0xff37c393, /// 0x4b0
			 0x3b110dc0, /// 0x4b4
			 0x20676f8a, /// 0x4b8
			 0xb51efbb2, /// 0x4bc
			 0xcdcd1f49, /// 0x4c0
			 0xa7ad7d42, /// 0x4c4
			 0x881fee9d, /// 0x4c8
			 0x44ad8dd9, /// 0x4cc
			 0x223650d5, /// 0x4d0
			 0x713fa7e2, /// 0x4d4
			 0xa5d72203, /// 0x4d8
			 0xd2143130, /// 0x4dc
			 0x36571331, /// 0x4e0
			 0x48243cdf, /// 0x4e4
			 0x57a8c032, /// 0x4e8
			 0xa33a7985, /// 0x4ec
			 0x704cb51e, /// 0x4f0
			 0x224b577e, /// 0x4f4
			 0xca9bfbfe, /// 0x4f8
			 0x0d06ad20, /// 0x4fc
			 0x94d77aae, /// 0x500
			 0x893d9802, /// 0x504
			 0xf6cde2d8, /// 0x508
			 0xaf6c1fdc, /// 0x50c
			 0x1fbaa42f, /// 0x510
			 0x9946772f, /// 0x514
			 0xf5a146dc, /// 0x518
			 0xad91828e, /// 0x51c
			 0x80ae46a5, /// 0x520
			 0x864723d7, /// 0x524
			 0x7290a584, /// 0x528
			 0xc3268621, /// 0x52c
			 0xcf0cc91f, /// 0x530
			 0xed5bfe43, /// 0x534
			 0xa39f2895, /// 0x538
			 0xccda17c3, /// 0x53c
			 0xb7c16bb6, /// 0x540
			 0xc4a8dd7e, /// 0x544
			 0x4c18d690, /// 0x548
			 0xc594330a, /// 0x54c
			 0xf9b8f760, /// 0x550
			 0x8ff6d4e4, /// 0x554
			 0x716fe6a7, /// 0x558
			 0xb47d8657, /// 0x55c
			 0x27eab120, /// 0x560
			 0x37596e03, /// 0x564
			 0xa0dbae5a, /// 0x568
			 0xfc0cda70, /// 0x56c
			 0xb232a88a, /// 0x570
			 0x03947976, /// 0x574
			 0xaec1ea5c, /// 0x578
			 0x6343173a, /// 0x57c
			 0xb3a27ba2, /// 0x580
			 0xd6993ab3, /// 0x584
			 0xc1652bb1, /// 0x588
			 0x05e5f69b, /// 0x58c
			 0xef32a05a, /// 0x590
			 0x1c0f8c70, /// 0x594
			 0x8fa07a3d, /// 0x598
			 0xd0b5636d, /// 0x59c
			 0xb9045c5e, /// 0x5a0
			 0x0c0a2dde, /// 0x5a4
			 0xf9ca5966, /// 0x5a8
			 0x6588bd74, /// 0x5ac
			 0xfbc2f251, /// 0x5b0
			 0x31728562, /// 0x5b4
			 0x060bd9bf, /// 0x5b8
			 0xeab21f1b, /// 0x5bc
			 0xe56f5a2a, /// 0x5c0
			 0x5048ca02, /// 0x5c4
			 0x7505ab44, /// 0x5c8
			 0xdad1eb46, /// 0x5cc
			 0x18bb500f, /// 0x5d0
			 0x6327d027, /// 0x5d4
			 0xc489b13e, /// 0x5d8
			 0xc798c6e2, /// 0x5dc
			 0x18df2ecf, /// 0x5e0
			 0x93eb0c57, /// 0x5e4
			 0x25bc92df, /// 0x5e8
			 0xe80996b5, /// 0x5ec
			 0xa0f8a290, /// 0x5f0
			 0x36769391, /// 0x5f4
			 0xd3af3115, /// 0x5f8
			 0x9e48df8f, /// 0x5fc
			 0x386bd7a3, /// 0x600
			 0xc1314dc7, /// 0x604
			 0xde3213af, /// 0x608
			 0x016d07bd, /// 0x60c
			 0xd69ff94a, /// 0x610
			 0x1dcc45eb, /// 0x614
			 0xb56b9723, /// 0x618
			 0x4225980e, /// 0x61c
			 0x21273420, /// 0x620
			 0xc340a107, /// 0x624
			 0xb296be60, /// 0x628
			 0xc08849e7, /// 0x62c
			 0x16afe8d4, /// 0x630
			 0x837fce0f, /// 0x634
			 0x586a5c77, /// 0x638
			 0x02999763, /// 0x63c
			 0x375b1c5b, /// 0x640
			 0xf14c4fbb, /// 0x644
			 0xa440e710, /// 0x648
			 0xdadbcaf3, /// 0x64c
			 0x89b3e033, /// 0x650
			 0xa3a30411, /// 0x654
			 0x12849b79, /// 0x658
			 0x3a9fc8cb, /// 0x65c
			 0x09618a13, /// 0x660
			 0xb6a64224, /// 0x664
			 0x0b06c064, /// 0x668
			 0xab43b9d0, /// 0x66c
			 0x1584ecc2, /// 0x670
			 0xe5c6df3c, /// 0x674
			 0xd7e48b78, /// 0x678
			 0xcc6630f6, /// 0x67c
			 0x77311628, /// 0x680
			 0xcb5b5ad3, /// 0x684
			 0x2f84dc49, /// 0x688
			 0x3bce460c, /// 0x68c
			 0x568a56e5, /// 0x690
			 0x5ff1463e, /// 0x694
			 0xfde42ed3, /// 0x698
			 0x65822391, /// 0x69c
			 0x53985d67, /// 0x6a0
			 0xf3e96810, /// 0x6a4
			 0x4524d8b7, /// 0x6a8
			 0x59246d67, /// 0x6ac
			 0x113d637d, /// 0x6b0
			 0x42c805e6, /// 0x6b4
			 0x9723652b, /// 0x6b8
			 0xc156fb6e, /// 0x6bc
			 0xa6ba49b9, /// 0x6c0
			 0x8a553913, /// 0x6c4
			 0x933e8a8b, /// 0x6c8
			 0xd94ad035, /// 0x6cc
			 0x75450ecd, /// 0x6d0
			 0xfc9426bb, /// 0x6d4
			 0xc0c6a72a, /// 0x6d8
			 0x34824cf1, /// 0x6dc
			 0xb20f2b04, /// 0x6e0
			 0x2f4b9bb6, /// 0x6e4
			 0xb9a330a5, /// 0x6e8
			 0x09f3d05c, /// 0x6ec
			 0x2669f447, /// 0x6f0
			 0x3c7c18a3, /// 0x6f4
			 0x1068e1a5, /// 0x6f8
			 0x01f8a374, /// 0x6fc
			 0x0806aef7, /// 0x700
			 0x8c0591d4, /// 0x704
			 0xda80d75e, /// 0x708
			 0xd9d261a8, /// 0x70c
			 0x706d53af, /// 0x710
			 0xc177a0e6, /// 0x714
			 0x203aade3, /// 0x718
			 0xd57863a6, /// 0x71c
			 0x5dbec5de, /// 0x720
			 0x29119e37, /// 0x724
			 0x42fa02fb, /// 0x728
			 0x9afff08d, /// 0x72c
			 0xac02a927, /// 0x730
			 0x486402e9, /// 0x734
			 0xa50d068e, /// 0x738
			 0x664a6f61, /// 0x73c
			 0xa00981a2, /// 0x740
			 0xc209625e, /// 0x744
			 0x4bd8c07e, /// 0x748
			 0xebf53bf5, /// 0x74c
			 0xd508b856, /// 0x750
			 0x10cbd600, /// 0x754
			 0xf31d5478, /// 0x758
			 0xfedf515f, /// 0x75c
			 0x22c3d19d, /// 0x760
			 0x13e9252b, /// 0x764
			 0xe89f1725, /// 0x768
			 0xffae825e, /// 0x76c
			 0x12004e5b, /// 0x770
			 0xcdbd8a5f, /// 0x774
			 0x17e86f20, /// 0x778
			 0x92dd5b6f, /// 0x77c
			 0x1db87a7a, /// 0x780
			 0x187f1afa, /// 0x784
			 0xb2b31ade, /// 0x788
			 0xed00f72d, /// 0x78c
			 0x93d22826, /// 0x790
			 0xe807d588, /// 0x794
			 0xd35c95b2, /// 0x798
			 0x4c37e0e9, /// 0x79c
			 0x5110d132, /// 0x7a0
			 0x1f82f447, /// 0x7a4
			 0xfea3e5b0, /// 0x7a8
			 0xd451d489, /// 0x7ac
			 0x9aeb21c5, /// 0x7b0
			 0x7e82d362, /// 0x7b4
			 0x5f610f80, /// 0x7b8
			 0x675c5752, /// 0x7bc
			 0x715eefd7, /// 0x7c0
			 0xf372e293, /// 0x7c4
			 0xa81b89ba, /// 0x7c8
			 0x07366f80, /// 0x7cc
			 0x0ec033d4, /// 0x7d0
			 0xf4eed83c, /// 0x7d4
			 0x4d0c6e93, /// 0x7d8
			 0x872f1b0c, /// 0x7dc
			 0xb045aa81, /// 0x7e0
			 0x51a22cf7, /// 0x7e4
			 0xdfa7c499, /// 0x7e8
			 0xde26a4fe, /// 0x7ec
			 0xca2b631e, /// 0x7f0
			 0xb7de9afd, /// 0x7f4
			 0x11dfa28c, /// 0x7f8
			 0xc3699a86, /// 0x7fc
			 0x733f0606, /// 0x800
			 0xe98189f8, /// 0x804
			 0xd5a91832, /// 0x808
			 0xbd678222, /// 0x80c
			 0x760dbb72, /// 0x810
			 0x41a74ae1, /// 0x814
			 0xe21f1512, /// 0x818
			 0x5d61de58, /// 0x81c
			 0xced31bff, /// 0x820
			 0xe7d2f8c8, /// 0x824
			 0x6128c4ab, /// 0x828
			 0x183d05bf, /// 0x82c
			 0xb4ad7757, /// 0x830
			 0xe1a2167f, /// 0x834
			 0xdd30ec39, /// 0x838
			 0x0eedf66b, /// 0x83c
			 0x41b2ec60, /// 0x840
			 0x07da791d, /// 0x844
			 0x821b4897, /// 0x848
			 0x0f0ab781, /// 0x84c
			 0x5fa2150a, /// 0x850
			 0xeedc30aa, /// 0x854
			 0xca09d9d9, /// 0x858
			 0xc800b740, /// 0x85c
			 0x75a206e3, /// 0x860
			 0xfb96399b, /// 0x864
			 0xbd4c704d, /// 0x868
			 0x0528f1aa, /// 0x86c
			 0x3d3a363a, /// 0x870
			 0x0d9dd608, /// 0x874
			 0x91a6909c, /// 0x878
			 0x2decec01, /// 0x87c
			 0x6d78f08b, /// 0x880
			 0xaf5c58b8, /// 0x884
			 0xa57b5389, /// 0x888
			 0x50ddc727, /// 0x88c
			 0x01398f00, /// 0x890
			 0x5b048728, /// 0x894
			 0xa01a33c0, /// 0x898
			 0x519c393c, /// 0x89c
			 0x0b2d6644, /// 0x8a0
			 0x73020c21, /// 0x8a4
			 0x8ffdfd57, /// 0x8a8
			 0xc579cae0, /// 0x8ac
			 0x9002d894, /// 0x8b0
			 0x5c3dbf4a, /// 0x8b4
			 0xb27a6118, /// 0x8b8
			 0xaadfeec5, /// 0x8bc
			 0x37fedcdf, /// 0x8c0
			 0x29299d9f, /// 0x8c4
			 0xb5eae7a6, /// 0x8c8
			 0xdfdea342, /// 0x8cc
			 0x32ee9aac, /// 0x8d0
			 0x39993855, /// 0x8d4
			 0xdf4133a4, /// 0x8d8
			 0xaf746bd5, /// 0x8dc
			 0x424b13cd, /// 0x8e0
			 0x55cfc770, /// 0x8e4
			 0xdc87937b, /// 0x8e8
			 0xc6506cb0, /// 0x8ec
			 0xda5606e8, /// 0x8f0
			 0xb4665f51, /// 0x8f4
			 0xef4f073d, /// 0x8f8
			 0x6b11fdb1, /// 0x8fc
			 0xf052af12, /// 0x900
			 0xeb32c2b6, /// 0x904
			 0x01cc20dd, /// 0x908
			 0xe0f447cc, /// 0x90c
			 0x513a91e7, /// 0x910
			 0xe79c98f8, /// 0x914
			 0x60c38aee, /// 0x918
			 0x4a5bbbeb, /// 0x91c
			 0x68385a60, /// 0x920
			 0x9ee04a42, /// 0x924
			 0x7843d25a, /// 0x928
			 0x16091ee3, /// 0x92c
			 0xa515537d, /// 0x930
			 0xd7cadc7c, /// 0x934
			 0x7dd9193b, /// 0x938
			 0x6714c75a, /// 0x93c
			 0x98dae402, /// 0x940
			 0x20dc61ff, /// 0x944
			 0x2e007bcd, /// 0x948
			 0x00f82a54, /// 0x94c
			 0x52d11ebe, /// 0x950
			 0x7a469b80, /// 0x954
			 0x2ed0dd32, /// 0x958
			 0xebfe9f52, /// 0x95c
			 0x8cde4de6, /// 0x960
			 0x0e9a9a8f, /// 0x964
			 0xd0f176c2, /// 0x968
			 0x02bd139a, /// 0x96c
			 0xff1f9106, /// 0x970
			 0x80476255, /// 0x974
			 0xa70938d1, /// 0x978
			 0xc0434ad6, /// 0x97c
			 0xf3ef9939, /// 0x980
			 0x2bf004fe, /// 0x984
			 0x55ddeb26, /// 0x988
			 0x666254c9, /// 0x98c
			 0x3fd0e621, /// 0x990
			 0x0e475da5, /// 0x994
			 0x6d3b39a8, /// 0x998
			 0xc88d8298, /// 0x99c
			 0x2c2b7fd0, /// 0x9a0
			 0x9ab37ea9, /// 0x9a4
			 0x6a893667, /// 0x9a8
			 0xe834d20b, /// 0x9ac
			 0x61e1d134, /// 0x9b0
			 0xd3c2af32, /// 0x9b4
			 0xf53bbae7, /// 0x9b8
			 0x387bce8f, /// 0x9bc
			 0x48bf741b, /// 0x9c0
			 0xb3fd3704, /// 0x9c4
			 0xa16e0dc9, /// 0x9c8
			 0xe07500d4, /// 0x9cc
			 0x34c8236b, /// 0x9d0
			 0xe091c835, /// 0x9d4
			 0xb191c9b8, /// 0x9d8
			 0x54b35ec7, /// 0x9dc
			 0xef48e062, /// 0x9e0
			 0x2c405576, /// 0x9e4
			 0xa812d13f, /// 0x9e8
			 0x54735f2e, /// 0x9ec
			 0x6f31b00d, /// 0x9f0
			 0x6b79ab92, /// 0x9f4
			 0x36112ab1, /// 0x9f8
			 0x14ee35ed, /// 0x9fc
			 0xc0a68d0f, /// 0xa00
			 0x3c3be23b, /// 0xa04
			 0x2758b967, /// 0xa08
			 0x9b98dda4, /// 0xa0c
			 0x9a01f31f, /// 0xa10
			 0x62b76485, /// 0xa14
			 0x0ebee347, /// 0xa18
			 0x8667fcfb, /// 0xa1c
			 0xc66e42f4, /// 0xa20
			 0x860d5292, /// 0xa24
			 0x966100d6, /// 0xa28
			 0x2132ddfb, /// 0xa2c
			 0x7f03f3b5, /// 0xa30
			 0xc8d9949c, /// 0xa34
			 0x3bb6e969, /// 0xa38
			 0x1080c648, /// 0xa3c
			 0x0e2409cd, /// 0xa40
			 0x685fe845, /// 0xa44
			 0x3b35eac9, /// 0xa48
			 0x60520985, /// 0xa4c
			 0x4fed7477, /// 0xa50
			 0xfb27a58c, /// 0xa54
			 0x671ec6ff, /// 0xa58
			 0x8fd6e13f, /// 0xa5c
			 0xcf2ebe2c, /// 0xa60
			 0x3bf52f33, /// 0xa64
			 0xa64c0c5b, /// 0xa68
			 0xfe3bfca5, /// 0xa6c
			 0xf0dc58d2, /// 0xa70
			 0x009cf5d4, /// 0xa74
			 0x832ca86b, /// 0xa78
			 0x686be75b, /// 0xa7c
			 0x56d15d76, /// 0xa80
			 0x57880d96, /// 0xa84
			 0x58720be1, /// 0xa88
			 0xb62671c1, /// 0xa8c
			 0x9febf9af, /// 0xa90
			 0x82bbec9d, /// 0xa94
			 0x2ab8a5d6, /// 0xa98
			 0x3772152d, /// 0xa9c
			 0x51b21077, /// 0xaa0
			 0x69d6e9c3, /// 0xaa4
			 0x61d20476, /// 0xaa8
			 0x933cc301, /// 0xaac
			 0x4b99d18d, /// 0xab0
			 0x1f73ecee, /// 0xab4
			 0x0f159dc7, /// 0xab8
			 0x46f0d014, /// 0xabc
			 0xa9db629f, /// 0xac0
			 0xb147f80d, /// 0xac4
			 0xaebed07f, /// 0xac8
			 0x546bf4d9, /// 0xacc
			 0xe5e6d4f2, /// 0xad0
			 0x2708ee5b, /// 0xad4
			 0xe3344120, /// 0xad8
			 0xb61e626b, /// 0xadc
			 0x73568bef, /// 0xae0
			 0x44383eb9, /// 0xae4
			 0x7ca68203, /// 0xae8
			 0x074a7c31, /// 0xaec
			 0x7f5f2dab, /// 0xaf0
			 0xbdabfb8d, /// 0xaf4
			 0x9e957887, /// 0xaf8
			 0xe78a9f0a, /// 0xafc
			 0xfb09c2aa, /// 0xb00
			 0x9f4f4124, /// 0xb04
			 0x6e47a6e6, /// 0xb08
			 0xa90a5f70, /// 0xb0c
			 0x2984f23a, /// 0xb10
			 0x3ea65b80, /// 0xb14
			 0xf16b6a2c, /// 0xb18
			 0xdf2732a6, /// 0xb1c
			 0x35a0f2df, /// 0xb20
			 0xcbdb3fac, /// 0xb24
			 0x02af9882, /// 0xb28
			 0x32e174cd, /// 0xb2c
			 0x112533e6, /// 0xb30
			 0x871abf5e, /// 0xb34
			 0xa48cd173, /// 0xb38
			 0x1e7a98be, /// 0xb3c
			 0xf450805f, /// 0xb40
			 0xc3267fe3, /// 0xb44
			 0x85b206b3, /// 0xb48
			 0xeed342a1, /// 0xb4c
			 0x795024f1, /// 0xb50
			 0x4ecc7cbd, /// 0xb54
			 0x208c2f77, /// 0xb58
			 0x1884cb7b, /// 0xb5c
			 0x2436fd10, /// 0xb60
			 0xcb865c90, /// 0xb64
			 0xb9788143, /// 0xb68
			 0xd644cb78, /// 0xb6c
			 0x8d79f951, /// 0xb70
			 0x60a910e7, /// 0xb74
			 0x2130d61b, /// 0xb78
			 0xc7ee2f8c, /// 0xb7c
			 0x19dfd243, /// 0xb80
			 0xbbe3657e, /// 0xb84
			 0x67469a30, /// 0xb88
			 0xaaa4794a, /// 0xb8c
			 0x30b366a1, /// 0xb90
			 0x6daadea4, /// 0xb94
			 0x48c500fc, /// 0xb98
			 0xed8f01cf, /// 0xb9c
			 0xe4bdc909, /// 0xba0
			 0xc414d9c9, /// 0xba4
			 0xfa5deaaf, /// 0xba8
			 0xb31b588f, /// 0xbac
			 0xc602ebba, /// 0xbb0
			 0x9df2f401, /// 0xbb4
			 0x519b5fd1, /// 0xbb8
			 0xed99e808, /// 0xbbc
			 0xa47da5a6, /// 0xbc0
			 0x94f17983, /// 0xbc4
			 0x932f3c43, /// 0xbc8
			 0x39c16fd7, /// 0xbcc
			 0x624e51f9, /// 0xbd0
			 0x9d90459f, /// 0xbd4
			 0x5952e02b, /// 0xbd8
			 0x027fbd87, /// 0xbdc
			 0x4aefe9a3, /// 0xbe0
			 0x6dc13a3d, /// 0xbe4
			 0x40ae3644, /// 0xbe8
			 0xe3c50138, /// 0xbec
			 0x8d096848, /// 0xbf0
			 0xce22dbe6, /// 0xbf4
			 0x71c46ca9, /// 0xbf8
			 0x8b622a28, /// 0xbfc
			 0x41f7b858, /// 0xc00
			 0xe411c071, /// 0xc04
			 0xbb1aa798, /// 0xc08
			 0xc4f0133e, /// 0xc0c
			 0x362f166f, /// 0xc10
			 0xc2c968d6, /// 0xc14
			 0x5a030dab, /// 0xc18
			 0x7f6a139a, /// 0xc1c
			 0x0a523ce8, /// 0xc20
			 0xdff34150, /// 0xc24
			 0x37b5be05, /// 0xc28
			 0x45aadf53, /// 0xc2c
			 0xc8699f2a, /// 0xc30
			 0x30420869, /// 0xc34
			 0x47cc967c, /// 0xc38
			 0x5a4f29bf, /// 0xc3c
			 0x1b5d8cda, /// 0xc40
			 0x4422ac88, /// 0xc44
			 0x3274767b, /// 0xc48
			 0x9295fad1, /// 0xc4c
			 0x315b27f8, /// 0xc50
			 0x64fa4c9a, /// 0xc54
			 0x4a61671e, /// 0xc58
			 0x93d181b9, /// 0xc5c
			 0x6c2fb389, /// 0xc60
			 0x5bf5c1b6, /// 0xc64
			 0xd85d337a, /// 0xc68
			 0x97c65e9c, /// 0xc6c
			 0x911eea16, /// 0xc70
			 0x8b3ae9c7, /// 0xc74
			 0x33a30ce5, /// 0xc78
			 0xaeaab182, /// 0xc7c
			 0x9e12e149, /// 0xc80
			 0x4bac4a9b, /// 0xc84
			 0xa7e80ada, /// 0xc88
			 0x003e6b9d, /// 0xc8c
			 0x1d5f3a87, /// 0xc90
			 0x3a77ca8e, /// 0xc94
			 0x67bce3ca, /// 0xc98
			 0xfdd2521f, /// 0xc9c
			 0x3cf6913d, /// 0xca0
			 0xea5f7c8f, /// 0xca4
			 0x5e0b346c, /// 0xca8
			 0x8d29cbe3, /// 0xcac
			 0xc41eb563, /// 0xcb0
			 0xd7406f50, /// 0xcb4
			 0xd7f49e8c, /// 0xcb8
			 0xbf074796, /// 0xcbc
			 0x405488bf, /// 0xcc0
			 0x8a390d75, /// 0xcc4
			 0xd7634b62, /// 0xcc8
			 0x30471c15, /// 0xccc
			 0x4db7d603, /// 0xcd0
			 0x4e8fcf5f, /// 0xcd4
			 0x03858de9, /// 0xcd8
			 0x8aabbf1a, /// 0xcdc
			 0xd59134ea, /// 0xce0
			 0xebbbd5f6, /// 0xce4
			 0xc66855b2, /// 0xce8
			 0xc66dc2fb, /// 0xcec
			 0x86165b10, /// 0xcf0
			 0x9b5377da, /// 0xcf4
			 0xb69062ee, /// 0xcf8
			 0xc6a60ffc, /// 0xcfc
			 0x81cedee3, /// 0xd00
			 0xec020499, /// 0xd04
			 0x85a1ed91, /// 0xd08
			 0x1d65c419, /// 0xd0c
			 0xe4fd8511, /// 0xd10
			 0xb42a5b47, /// 0xd14
			 0x68e7fddd, /// 0xd18
			 0x5c5c1ba4, /// 0xd1c
			 0x28c11daa, /// 0xd20
			 0xbab7af0f, /// 0xd24
			 0xbc04cbf8, /// 0xd28
			 0xd32589ca, /// 0xd2c
			 0x5466b559, /// 0xd30
			 0x7259f255, /// 0xd34
			 0xcb3b2684, /// 0xd38
			 0xc4dc9599, /// 0xd3c
			 0xf78d1d30, /// 0xd40
			 0x9a16cbde, /// 0xd44
			 0xde646d1e, /// 0xd48
			 0x887e92a0, /// 0xd4c
			 0x6a6b1f50, /// 0xd50
			 0xfe1eb86f, /// 0xd54
			 0x935c27a6, /// 0xd58
			 0x7590cdd5, /// 0xd5c
			 0xc55971da, /// 0xd60
			 0x3a4d0724, /// 0xd64
			 0xef57d23e, /// 0xd68
			 0x95554657, /// 0xd6c
			 0xbacde1c5, /// 0xd70
			 0x921d707b, /// 0xd74
			 0x4f6bf9f9, /// 0xd78
			 0xf512128c, /// 0xd7c
			 0xeef84527, /// 0xd80
			 0x742d4c0f, /// 0xd84
			 0xd29cdf7c, /// 0xd88
			 0xf2b0f8d2, /// 0xd8c
			 0xa31080dc, /// 0xd90
			 0xd8e87b7d, /// 0xd94
			 0xd027b484, /// 0xd98
			 0xd6a60cf6, /// 0xd9c
			 0x879ed45a, /// 0xda0
			 0x8111f13d, /// 0xda4
			 0x26bf1d82, /// 0xda8
			 0x428fbdd3, /// 0xdac
			 0x8483ddbe, /// 0xdb0
			 0x96eb7ec6, /// 0xdb4
			 0xd1fcca17, /// 0xdb8
			 0xc8d2940c, /// 0xdbc
			 0x58fe0b09, /// 0xdc0
			 0xdf6cf5dc, /// 0xdc4
			 0x066cd6e7, /// 0xdc8
			 0xf605da6d, /// 0xdcc
			 0xe028b81c, /// 0xdd0
			 0x09f14deb, /// 0xdd4
			 0x65606ef6, /// 0xdd8
			 0x014a5096, /// 0xddc
			 0xce361569, /// 0xde0
			 0xd7b192ab, /// 0xde4
			 0x9ab76dec, /// 0xde8
			 0x8141a148, /// 0xdec
			 0x193ba4ef, /// 0xdf0
			 0xf67561e0, /// 0xdf4
			 0xe137f66c, /// 0xdf8
			 0xb7c003d9, /// 0xdfc
			 0x60eae733, /// 0xe00
			 0x8618cf74, /// 0xe04
			 0xd4de90fa, /// 0xe08
			 0x5b31535e, /// 0xe0c
			 0xc2276330, /// 0xe10
			 0x304704a9, /// 0xe14
			 0xc50372de, /// 0xe18
			 0xcfd51a7a, /// 0xe1c
			 0x18a14bef, /// 0xe20
			 0x4eca0a5b, /// 0xe24
			 0x6491d807, /// 0xe28
			 0xa402c39e, /// 0xe2c
			 0xd239e2cd, /// 0xe30
			 0xeeed836e, /// 0xe34
			 0xb7fed32f, /// 0xe38
			 0x322fe32c, /// 0xe3c
			 0xd38031cd, /// 0xe40
			 0xbd008a38, /// 0xe44
			 0x71bd7c38, /// 0xe48
			 0x12ddded3, /// 0xe4c
			 0x845328a5, /// 0xe50
			 0x30a58c09, /// 0xe54
			 0x4d9b2c70, /// 0xe58
			 0x29b530a3, /// 0xe5c
			 0x21784662, /// 0xe60
			 0x46ca3f66, /// 0xe64
			 0xac3af871, /// 0xe68
			 0x797fd1ee, /// 0xe6c
			 0xfb1921dc, /// 0xe70
			 0x2d280de8, /// 0xe74
			 0x5c45040d, /// 0xe78
			 0x678f2249, /// 0xe7c
			 0x33f209ba, /// 0xe80
			 0xeb96ec21, /// 0xe84
			 0xfdb06aa4, /// 0xe88
			 0x3ed98abd, /// 0xe8c
			 0x43879a41, /// 0xe90
			 0x8a8f65ea, /// 0xe94
			 0xfd4c3743, /// 0xe98
			 0x114d6c0d, /// 0xe9c
			 0x8fdfd808, /// 0xea0
			 0xf2341762, /// 0xea4
			 0x4c28275b, /// 0xea8
			 0x045d1125, /// 0xeac
			 0xa3e50b96, /// 0xeb0
			 0x495be32d, /// 0xeb4
			 0x4a30cd6e, /// 0xeb8
			 0xaa428df6, /// 0xebc
			 0x97e88526, /// 0xec0
			 0x8ffa7648, /// 0xec4
			 0xfae4c727, /// 0xec8
			 0xc26067b4, /// 0xecc
			 0x847c287b, /// 0xed0
			 0x8e1a776d, /// 0xed4
			 0x39a7c15f, /// 0xed8
			 0x9b76c279, /// 0xedc
			 0xd070ece2, /// 0xee0
			 0xc1e1573f, /// 0xee4
			 0xe843caaf, /// 0xee8
			 0x426f36b1, /// 0xeec
			 0x60e0703a, /// 0xef0
			 0x63a04996, /// 0xef4
			 0x0c8bbc8d, /// 0xef8
			 0xb0390383, /// 0xefc
			 0x159c0b2c, /// 0xf00
			 0x2cafe1e1, /// 0xf04
			 0x43d8ea53, /// 0xf08
			 0x262129be, /// 0xf0c
			 0x541aea91, /// 0xf10
			 0xba790db7, /// 0xf14
			 0x9893345e, /// 0xf18
			 0xd91088a4, /// 0xf1c
			 0x92e7413c, /// 0xf20
			 0xcc9f906f, /// 0xf24
			 0x4d4799bc, /// 0xf28
			 0xb52ed5a3, /// 0xf2c
			 0x7224f0a7, /// 0xf30
			 0x226b169d, /// 0xf34
			 0x6ceea790, /// 0xf38
			 0xfd9f7b92, /// 0xf3c
			 0xa8039040, /// 0xf40
			 0xbb943e44, /// 0xf44
			 0x0a5b637f, /// 0xf48
			 0x506677c7, /// 0xf4c
			 0xf9d168d1, /// 0xf50
			 0x7b10cd01, /// 0xf54
			 0x60f3bb31, /// 0xf58
			 0x0e5526c9, /// 0xf5c
			 0x82a0de87, /// 0xf60
			 0x213f9875, /// 0xf64
			 0x226045f0, /// 0xf68
			 0xad5e66e4, /// 0xf6c
			 0xa5a998b3, /// 0xf70
			 0xfc795151, /// 0xf74
			 0xbbc3a8d9, /// 0xf78
			 0x682ad496, /// 0xf7c
			 0x573ff628, /// 0xf80
			 0x0f358d50, /// 0xf84
			 0xd9315e62, /// 0xf88
			 0xee410e37, /// 0xf8c
			 0x0a053bc9, /// 0xf90
			 0xfa1c61dd, /// 0xf94
			 0x9cc2902c, /// 0xf98
			 0xf08720af, /// 0xf9c
			 0xd5d4459d, /// 0xfa0
			 0x7f2d70fb, /// 0xfa4
			 0xdc98489c, /// 0xfa8
			 0x1fcb7b4b, /// 0xfac
			 0xf85714f7, /// 0xfb0
			 0x27943a6a, /// 0xfb4
			 0xee93c520, /// 0xfb8
			 0x41cb0cc0, /// 0xfbc
			 0xf58990a7, /// 0xfc0
			 0xfdec4093, /// 0xfc4
			 0xc50e261d, /// 0xfc8
			 0x5e06ede0, /// 0xfcc
			 0x0f0ea71e, /// 0xfd0
			 0xbd132606, /// 0xfd4
			 0x2717e07c, /// 0xfd8
			 0x5d50cd6e, /// 0xfdc
			 0xa081b87c, /// 0xfe0
			 0x79f3abba, /// 0xfe4
			 0xfecb67c5, /// 0xfe8
			 0x20147cec, /// 0xfec
			 0x5d308dfe, /// 0xff0
			 0xd6309c87, /// 0xff4
			 0x7a6da7bf, /// 0xff8
			 0xf7c1b646 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00004
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0000c
			 0x55555555,                                                  // 4 random values                             /// 00010
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00014
			 0x0e000001,                                                  // Trailing 1s:                                /// 00018
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0001c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00020
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00024
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0002c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00030
			 0x00011111,                                                  // 9.7958E-41                                  /// 00034
			 0x0c700000,                                                  // Leading 1s:                                 /// 00038
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0003c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00040
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00044
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00048
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0004c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00050
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00054
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0005c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00060
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00068
			 0x0e000003,                                                  // Trailing 1s:                                /// 0006c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00070
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00074
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x0c400000,                                                  // Leading 1s:                                 /// 0007c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00080
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0008c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00098
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0x7f800000,                                                  // inf                                         /// 000a0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000b0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000b4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000b8
			 0x3f028f5c,                                                  // 0.51                                        /// 000bc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000c0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000c8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000d0
			 0x4b000000,                                                  // 8388608.0                                   /// 000d4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000d8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000e0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000e4
			 0x00000000,                                                  // zero                                        /// 000e8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000ec
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000f0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000f4
			 0x7fc00001,                                                  // signaling NaN                               /// 000f8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000fc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00104
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00108
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0010c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00110
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00114
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0011c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00120
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00128
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0012c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00130
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00134
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00138
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0013c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00140
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00144
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00148
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0014c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00150
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00154
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00158
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0015c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00160
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00164
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00168
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0016c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00170
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00174
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00178
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0017c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00180
			 0x00000000,                                                  // zero                                        /// 00184
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00188
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0018c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00190
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00194
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00198
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0019c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001a0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001a4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001a8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001b0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001b8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x4b000000,                                                  // 8388608.0                                   /// 001c0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001cc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001d0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001d4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001d8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 001dc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001e0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001e4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001e8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001f0
			 0x7fc00001,                                                  // signaling NaN                               /// 001f4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001f8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001fc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00200
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00208
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0020c
			 0x7f800000,                                                  // inf                                         /// 00210
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00214
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00218
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0021c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00220
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00224
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00228
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0022c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00230
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x80000000,                                                  // -zero                                       /// 00238
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0023c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00240
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00248
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0024c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00250
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00254
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00258
			 0x00000000,                                                  // zero                                        /// 0025c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00260
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00264
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00268
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00270
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00274
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00278
			 0x00000000,                                                  // zero                                        /// 0027c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00280
			 0x0c700000,                                                  // Leading 1s:                                 /// 00284
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00288
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0028c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00290
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x80000000,                                                  // -zero                                       /// 00298
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0029c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002a4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002a8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002ac
			 0x55555555,                                                  // 4 random values                             /// 002b0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002b4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002b8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002c0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002c4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002d4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002d8
			 0x80000000,                                                  // -zero                                       /// 002dc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002e8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002f0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002f4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00300
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00304
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00308
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00310
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00314
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00318
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0031c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00320
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00324
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00328
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0032c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00330
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00334
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00338
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0033c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00344
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00348
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0034c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00350
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00354
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00358
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0035c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00364
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00368
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0036c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00370
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00374
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00378
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x55555555,                                                  // 4 random values                             /// 00380
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00384
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x0c700000,                                                  // Leading 1s:                                 /// 0038c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00394
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00398
			 0x3f028f5c,                                                  // 0.51                                        /// 0039c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003a0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003a4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003a8
			 0x7f800000,                                                  // inf                                         /// 003ac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003b0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003b4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003b8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003bc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003c0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003c4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003cc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003d0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003d4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003d8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003e0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003e4
			 0x0e000007,                                                  // Trailing 1s:                                /// 003e8
			 0x0c780000,                                                  // Leading 1s:                                 /// 003ec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003f0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003fc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00400
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00404
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00408
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0040c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00414
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00418
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0041c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00420
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00424
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0042c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00430
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00434
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00438
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0043c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00440
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00448
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00454
			 0x33333333,                                                  // 4 random values                             /// 00458
			 0x00000000,                                                  // zero                                        /// 0045c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00468
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0046c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00470
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00474
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00478
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0047c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00484
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00488
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0048c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00494
			 0x0c780000,                                                  // Leading 1s:                                 /// 00498
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0049c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x7f800000,                                                  // inf                                         /// 004a8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004ac
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004b0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004b4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004b8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004c4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004c8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004cc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004d0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004d4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004d8
			 0xcb000000,                                                  // -8388608.0                                  /// 004dc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004e4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004e8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004f0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004f4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004fc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00500
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00504
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00508
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0050c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00510
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00518
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0051c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00520
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00524
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00528
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0052c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00530
			 0xcb000000,                                                  // -8388608.0                                  /// 00534
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00538
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0053c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00544
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0054c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00550
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00554
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0055c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00560
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00568
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0056c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00570
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00578
			 0xbf028f5c,                                                  // -0.51                                       /// 0057c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00580
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00584
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00588
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0058c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00590
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00598
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005a0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x4b000000,                                                  // 8388608.0                                   /// 005ac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0xcb000000,                                                  // -8388608.0                                  /// 005bc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005c4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 005cc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005d0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005d4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005d8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0xcb000000,                                                  // -8388608.0                                  /// 005e0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005e8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005ec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005f0
			 0x33333333,                                                  // 4 random values                             /// 005f4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00604
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x00011111,                                                  // 9.7958E-41                                  /// 0060c
			 0x55555555,                                                  // 4 random values                             /// 00610
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00618
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0061c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00620
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00628
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0062c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00630
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00634
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00638
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0063c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0xffc00001,                                                  // -signaling NaN                              /// 00644
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0064c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00650
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00654
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00658
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00660
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00664
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00668
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0066c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00678
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0067c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0xbf028f5c,                                                  // -0.51                                       /// 00684
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00688
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00690
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00694
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0xbf028f5c,                                                  // -0.51                                       /// 0069c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006a0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x55555555,                                                  // 4 random values                             /// 006ac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006b0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006b8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006bc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006c8
			 0xff800000,                                                  // -inf                                        /// 006cc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006d0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006d4
			 0x0e000003,                                                  // Trailing 1s:                                /// 006d8
			 0x3f028f5c,                                                  // 0.51                                        /// 006dc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006e4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006e8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006fc
			 0x55555555,                                                  // 4 random values                             /// 00700
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00704
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00710
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00714
			 0x0c700000,                                                  // Leading 1s:                                 /// 00718
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0071c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00728
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00730
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00738
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0073c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00740
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00744
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00748
			 0x7fc00001,                                                  // signaling NaN                               /// 0074c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00754
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00758
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0075c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00760
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00764
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00774
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00778
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00780
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00784
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00788
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00790
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00794
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00798
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0079c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007a4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007ac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007b0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007b4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007b8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007c0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007c4
			 0x7fc00001,                                                  // signaling NaN                               /// 007c8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007d0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007d4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007e0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007f0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007f4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00800
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00808
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0080c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00810
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00814
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00818
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00820
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00824
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00828
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0082c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00830
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00838
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0083c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00840
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00844
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0084c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00854
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x3f028f5c,                                                  // 0.51                                        /// 0085c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00860
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00864
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00868
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0086c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00874
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00878
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00880
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00888
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0088c
			 0x7fc00001,                                                  // signaling NaN                               /// 00890
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0089c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008a4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008a8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008ac
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008b0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008b4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008b8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008bc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008c0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008c4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008c8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008cc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008d0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008e4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008ec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008f0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008f4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008fc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00900
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00904
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00908
			 0x00000000,                                                  // zero                                        /// 0090c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00910
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00914
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00918
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00928
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00934
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00938
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0093c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00940
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00944
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0094c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00950
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00954
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00958
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0095c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00964
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0096c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00970
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00974
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0097c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00980
			 0x0e000001,                                                  // Trailing 1s:                                /// 00984
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00988
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00990
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00994
			 0x00000000,                                                  // zero                                        /// 00998
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0099c
			 0x4b000000,                                                  // 8388608.0                                   /// 009a0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009a4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009ac
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009b4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009b8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009bc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009c4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009c8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009cc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009d4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009d8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009dc
			 0x7f800000,                                                  // inf                                         /// 009e0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009e4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009ec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009f4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009f8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a04
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a08
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a0c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a10
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a14
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a18
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a20
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a28
			 0xffc00001,                                                  // -signaling NaN                              /// 00a2c
			 0xffc00001,                                                  // -signaling NaN                              /// 00a30
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a34
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a38
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a40
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a4c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a50
			 0x4b000000,                                                  // 8388608.0                                   /// 00a54
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a5c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a60
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a6c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a70
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a78
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a80
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a84
			 0x3f028f5c,                                                  // 0.51                                        /// 00a88
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a94
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a98
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a9c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00aa0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00aa4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00aa8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00aac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ab0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ab8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ac4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00acc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ad0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ad4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ad8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ae0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ae4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ae8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00af0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00af4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00af8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b04
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b08
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b0c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b10
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b14
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b18
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b1c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b20
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b24
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b28
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b30
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b34
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b40
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b4c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b50
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b54
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b58
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b68
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b6c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b70
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b74
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b78
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b7c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b80
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b84
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b88
			 0x00000000,                                                  // zero                                        /// 00b8c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b90
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b94
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b98
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b9c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ba0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ba4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ba8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bb0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bb8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bc0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bc4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bc8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bcc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bd8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bdc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00be4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bf0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bfc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c0c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c10
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c14
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c18
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c1c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c20
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c28
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c30
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c40
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c4c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c50
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c54
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c5c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c60
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0xcb000000,                                                  // -8388608.0                                  /// 00c6c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c74
			 0xffc00001,                                                  // -signaling NaN                              /// 00c78
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c7c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c80
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c84
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c90
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c94
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c98
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c9c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x55555555,                                                  // 4 random values                             /// 00ca4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cb0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00cbc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cc0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cc4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cc8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cd0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cd8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cdc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ce4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ce8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00cec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cf4
			 0xcb000000,                                                  // -8388608.0                                  /// 00cf8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cfc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d00
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d04
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d10
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d14
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d18
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d20
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d24
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d28
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d2c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d38
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d3c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d40
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d48
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d50
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d54
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d58
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d60
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d68
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d6c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d70
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d74
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d7c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d80
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d84
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d8c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0xff800000,                                                  // -inf                                        /// 00d94
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d9c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00da0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00da4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x00000000,                                                  // zero                                        /// 00dac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00db0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00db4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00db8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00dc4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00dc8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00dcc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dd0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00dd4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ddc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00de0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00de4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00dec
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00df0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00df4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00df8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dfc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e00
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e04
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e10
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e14
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e20
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e24
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e28
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e2c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e30
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e38
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e48
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e4c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e50
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e54
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e58
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e5c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e60
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e64
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e68
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e6c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e70
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e74
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e78
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e80
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e8c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e90
			 0x33333333,                                                  // 4 random values                             /// 00e94
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e98
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ea0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ea4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ea8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00eac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00eb0
			 0x00000000,                                                  // zero                                        /// 00eb4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00eb8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ebc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ec4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ecc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ed0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ed4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ed8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00edc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ee0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ee4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ee8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00eec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ef0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ef4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ef8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00efc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f00
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f04
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f08
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f0c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f10
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f18
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f1c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f20
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f28
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f2c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f30
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f34
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f38
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f3c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f40
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f44
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f48
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f4c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f50
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f54
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f5c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f60
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f6c
			 0x00000000,                                                  // zero                                        /// 00f70
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f74
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f78
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f7c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f80
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f88
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f90
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f94
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f98
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fa0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fac
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fb0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fb4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fb8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fbc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fc0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fc8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fd8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fdc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fe0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fe8
			 0xff800000,                                                  // -inf                                        /// 00fec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ff0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ff4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x0e00001f                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0xee7db8c9, /// 0x0
			 0xe5d91d43, /// 0x4
			 0xc503e957, /// 0x8
			 0xd6ccf7b8, /// 0xc
			 0xa0dcbd4c, /// 0x10
			 0xa7c49a63, /// 0x14
			 0xf5df4f6b, /// 0x18
			 0x809410eb, /// 0x1c
			 0xd83dba6f, /// 0x20
			 0x4a952164, /// 0x24
			 0x4c6b42c8, /// 0x28
			 0xfcf9aecc, /// 0x2c
			 0x014c38be, /// 0x30
			 0x7f390f37, /// 0x34
			 0x1fdea0f9, /// 0x38
			 0xb5b504ce, /// 0x3c
			 0xa73ebfba, /// 0x40
			 0x08e59708, /// 0x44
			 0x12a2f96e, /// 0x48
			 0x5d78ccda, /// 0x4c
			 0xdf56eb42, /// 0x50
			 0xc6219ff9, /// 0x54
			 0x96a303a7, /// 0x58
			 0xf6983181, /// 0x5c
			 0x1f78037f, /// 0x60
			 0x5edc95f4, /// 0x64
			 0x05b14d00, /// 0x68
			 0xf5310ae9, /// 0x6c
			 0xd9731c10, /// 0x70
			 0xb9063f06, /// 0x74
			 0x7a292712, /// 0x78
			 0x61f28098, /// 0x7c
			 0x40c139a6, /// 0x80
			 0xf3b01c2e, /// 0x84
			 0x7f94738a, /// 0x88
			 0xee1dffe8, /// 0x8c
			 0xc0514715, /// 0x90
			 0x927447ab, /// 0x94
			 0x021bc2dc, /// 0x98
			 0x400d61b4, /// 0x9c
			 0x094c8770, /// 0xa0
			 0xe4618618, /// 0xa4
			 0x46581614, /// 0xa8
			 0xf1142005, /// 0xac
			 0xdf41138f, /// 0xb0
			 0xe1bc38bd, /// 0xb4
			 0x245d4d70, /// 0xb8
			 0x257ba509, /// 0xbc
			 0xcd460105, /// 0xc0
			 0xf928fe00, /// 0xc4
			 0xe304d733, /// 0xc8
			 0x02e31996, /// 0xcc
			 0x83b3abab, /// 0xd0
			 0xee3ce3d5, /// 0xd4
			 0xad134d49, /// 0xd8
			 0xbb0c28de, /// 0xdc
			 0xea498ee5, /// 0xe0
			 0xaaeac3b1, /// 0xe4
			 0x953ada56, /// 0xe8
			 0x4d36dc68, /// 0xec
			 0xd8e7ee5b, /// 0xf0
			 0x5b848168, /// 0xf4
			 0xc701353b, /// 0xf8
			 0x3eab221b, /// 0xfc
			 0x809d84c8, /// 0x100
			 0x97a466fd, /// 0x104
			 0xdd8be16c, /// 0x108
			 0x6b27dcfc, /// 0x10c
			 0x64f154e0, /// 0x110
			 0x39aad9bd, /// 0x114
			 0x1d3f9237, /// 0x118
			 0xba67d693, /// 0x11c
			 0x60fe3a8e, /// 0x120
			 0x6aaf7e4c, /// 0x124
			 0x0635c801, /// 0x128
			 0x6f48aaa1, /// 0x12c
			 0xd9f4e0a8, /// 0x130
			 0x822bea23, /// 0x134
			 0xe64a1c55, /// 0x138
			 0x9b75b578, /// 0x13c
			 0x0baeb9b3, /// 0x140
			 0x71c3fab0, /// 0x144
			 0xbeb0a771, /// 0x148
			 0x07cbd51b, /// 0x14c
			 0x0d2a4ea3, /// 0x150
			 0x11503d0e, /// 0x154
			 0x73fe5a59, /// 0x158
			 0xb85336be, /// 0x15c
			 0x4f109716, /// 0x160
			 0x9bbe024d, /// 0x164
			 0x1e1c4119, /// 0x168
			 0x0f4b2890, /// 0x16c
			 0x935ed8ee, /// 0x170
			 0xd602f64d, /// 0x174
			 0x9f39d7cf, /// 0x178
			 0x5532d52a, /// 0x17c
			 0xadb73c16, /// 0x180
			 0xeda42545, /// 0x184
			 0x69747f11, /// 0x188
			 0xfb2d90da, /// 0x18c
			 0x33c51470, /// 0x190
			 0x422bb579, /// 0x194
			 0xd6d009e9, /// 0x198
			 0x6866ab6c, /// 0x19c
			 0xdd835839, /// 0x1a0
			 0x0af448a6, /// 0x1a4
			 0x841b3324, /// 0x1a8
			 0x55e07c81, /// 0x1ac
			 0x8824e1ed, /// 0x1b0
			 0xe13a9cd4, /// 0x1b4
			 0x9f044c35, /// 0x1b8
			 0x89904f4d, /// 0x1bc
			 0x02bfc3f1, /// 0x1c0
			 0xb1dfa9a2, /// 0x1c4
			 0x19ab4b66, /// 0x1c8
			 0xf97a08b1, /// 0x1cc
			 0xa2b4f40b, /// 0x1d0
			 0xa98bcaf9, /// 0x1d4
			 0x8e91a3a5, /// 0x1d8
			 0xbcd1a736, /// 0x1dc
			 0x90a38112, /// 0x1e0
			 0x5b1a6bba, /// 0x1e4
			 0xb3a3c1c3, /// 0x1e8
			 0xdad56c3f, /// 0x1ec
			 0x03d14083, /// 0x1f0
			 0xc45a126f, /// 0x1f4
			 0x457b8838, /// 0x1f8
			 0x279aac5c, /// 0x1fc
			 0x84120b7e, /// 0x200
			 0xbc9da76d, /// 0x204
			 0x09be8ffa, /// 0x208
			 0xe7763d03, /// 0x20c
			 0x10750fcb, /// 0x210
			 0x9ff42706, /// 0x214
			 0xcd41ac18, /// 0x218
			 0xb08eea61, /// 0x21c
			 0x7decaadd, /// 0x220
			 0xc55ed724, /// 0x224
			 0x1fcbb260, /// 0x228
			 0xbb45a427, /// 0x22c
			 0xf7836864, /// 0x230
			 0x72b572de, /// 0x234
			 0xdeb643f4, /// 0x238
			 0xa1dcdbc7, /// 0x23c
			 0x038a1a11, /// 0x240
			 0xe47045ad, /// 0x244
			 0xc2345daa, /// 0x248
			 0x86290da5, /// 0x24c
			 0x43a24b39, /// 0x250
			 0x42f74fbc, /// 0x254
			 0xef975587, /// 0x258
			 0x89878a25, /// 0x25c
			 0xc495dfc7, /// 0x260
			 0x7ea6cdbf, /// 0x264
			 0x99bb076a, /// 0x268
			 0x1f0449e3, /// 0x26c
			 0xab61448b, /// 0x270
			 0x970d7e00, /// 0x274
			 0x4c89ebd8, /// 0x278
			 0x3845875e, /// 0x27c
			 0xfd9e5eda, /// 0x280
			 0x3ebad4eb, /// 0x284
			 0x77f3b630, /// 0x288
			 0x8493096c, /// 0x28c
			 0x4e9ff987, /// 0x290
			 0xb983f2b0, /// 0x294
			 0x08999cfb, /// 0x298
			 0x815cdaf9, /// 0x29c
			 0xb028c20f, /// 0x2a0
			 0x030be740, /// 0x2a4
			 0xca317877, /// 0x2a8
			 0xe2365133, /// 0x2ac
			 0x610854c2, /// 0x2b0
			 0xa7bc4f53, /// 0x2b4
			 0xefe03b71, /// 0x2b8
			 0xbbc64a8f, /// 0x2bc
			 0x373b02fd, /// 0x2c0
			 0x558cdd86, /// 0x2c4
			 0xdfb7b964, /// 0x2c8
			 0xfb5a53d5, /// 0x2cc
			 0x30726820, /// 0x2d0
			 0x7042dd74, /// 0x2d4
			 0xa3ccecb9, /// 0x2d8
			 0x8273dd49, /// 0x2dc
			 0xedf16f71, /// 0x2e0
			 0xa036252d, /// 0x2e4
			 0xb3c1e800, /// 0x2e8
			 0x401f750a, /// 0x2ec
			 0x4db80e1e, /// 0x2f0
			 0x30ace8df, /// 0x2f4
			 0x70685abb, /// 0x2f8
			 0x045fba43, /// 0x2fc
			 0x229149a5, /// 0x300
			 0x77d77d84, /// 0x304
			 0x661fd0c3, /// 0x308
			 0x6a95fad8, /// 0x30c
			 0x97707bd7, /// 0x310
			 0x258768df, /// 0x314
			 0xf246f365, /// 0x318
			 0x2f381dbc, /// 0x31c
			 0x991f7fb3, /// 0x320
			 0x2573cba1, /// 0x324
			 0x557f62ce, /// 0x328
			 0x1cb8f0c6, /// 0x32c
			 0x9afc61ed, /// 0x330
			 0x297aad33, /// 0x334
			 0x3f9a5647, /// 0x338
			 0xe332c111, /// 0x33c
			 0x7ac86a77, /// 0x340
			 0x6557f296, /// 0x344
			 0xf91627d9, /// 0x348
			 0xc5bbf4d3, /// 0x34c
			 0x9e615474, /// 0x350
			 0x48694d99, /// 0x354
			 0xcac41307, /// 0x358
			 0xe71cf9bf, /// 0x35c
			 0xf7f995c1, /// 0x360
			 0x6b032f63, /// 0x364
			 0x8e3aea4a, /// 0x368
			 0xd642dfa5, /// 0x36c
			 0xf9d5b24b, /// 0x370
			 0x6d4f2b77, /// 0x374
			 0xb5e23928, /// 0x378
			 0x93187720, /// 0x37c
			 0x728c6b28, /// 0x380
			 0x8b9325fd, /// 0x384
			 0x13886027, /// 0x388
			 0xe5971e01, /// 0x38c
			 0xbc3f9f1e, /// 0x390
			 0xceb062e7, /// 0x394
			 0xdc6e2a10, /// 0x398
			 0x9e8a6226, /// 0x39c
			 0x28cdc83e, /// 0x3a0
			 0x4ece5497, /// 0x3a4
			 0x684ea8fd, /// 0x3a8
			 0x7013e308, /// 0x3ac
			 0xaa2eb243, /// 0x3b0
			 0xf77fbac9, /// 0x3b4
			 0x4538c25e, /// 0x3b8
			 0x51f02058, /// 0x3bc
			 0x20ff70df, /// 0x3c0
			 0x81cc1f6a, /// 0x3c4
			 0xbefa54de, /// 0x3c8
			 0x5f884d15, /// 0x3cc
			 0xed473e35, /// 0x3d0
			 0x330ad44c, /// 0x3d4
			 0x4b9837f9, /// 0x3d8
			 0x8ab7ee6d, /// 0x3dc
			 0xec1783c0, /// 0x3e0
			 0x805143f2, /// 0x3e4
			 0x853c4775, /// 0x3e8
			 0xc51ff5c4, /// 0x3ec
			 0x2dc95b2d, /// 0x3f0
			 0x392063f4, /// 0x3f4
			 0xa3164b2d, /// 0x3f8
			 0xb768f274, /// 0x3fc
			 0x4e0b60d3, /// 0x400
			 0xb1cf6ddb, /// 0x404
			 0x100a84f8, /// 0x408
			 0xfb8419c9, /// 0x40c
			 0x21e5f15a, /// 0x410
			 0xd88d47fa, /// 0x414
			 0x7d342b49, /// 0x418
			 0xe0ffc423, /// 0x41c
			 0x60b5b2a0, /// 0x420
			 0x7cb71277, /// 0x424
			 0x9c0b2449, /// 0x428
			 0x9ddf0bf6, /// 0x42c
			 0x6ff92869, /// 0x430
			 0xdf96f42e, /// 0x434
			 0xf7e3f8f7, /// 0x438
			 0x20d7bc82, /// 0x43c
			 0x0064c788, /// 0x440
			 0xba718d84, /// 0x444
			 0x5c393d69, /// 0x448
			 0xcdcf7e73, /// 0x44c
			 0x2f25c297, /// 0x450
			 0xd2b6c053, /// 0x454
			 0x86c8a193, /// 0x458
			 0xdc67aa98, /// 0x45c
			 0xd58f99bf, /// 0x460
			 0xe29abd5a, /// 0x464
			 0x9b9aaa5d, /// 0x468
			 0x2d696d96, /// 0x46c
			 0x285d3738, /// 0x470
			 0x2f60681b, /// 0x474
			 0x3ddff21a, /// 0x478
			 0xc7a6f007, /// 0x47c
			 0x936cd532, /// 0x480
			 0xc9d0e0a3, /// 0x484
			 0xca84f489, /// 0x488
			 0x8e8bf8e9, /// 0x48c
			 0xccfa176b, /// 0x490
			 0x76f98107, /// 0x494
			 0x5509219d, /// 0x498
			 0x3da06677, /// 0x49c
			 0x09d279cc, /// 0x4a0
			 0xf2aa57a2, /// 0x4a4
			 0xdfcbfe72, /// 0x4a8
			 0x39ab2e97, /// 0x4ac
			 0x9212f05a, /// 0x4b0
			 0x88a9c5d3, /// 0x4b4
			 0xe348e49b, /// 0x4b8
			 0xa3401685, /// 0x4bc
			 0x8a45ba2e, /// 0x4c0
			 0xe3b82b47, /// 0x4c4
			 0xfa9b8336, /// 0x4c8
			 0x0ac5bf4e, /// 0x4cc
			 0xf23a53dd, /// 0x4d0
			 0x3179b7ce, /// 0x4d4
			 0x7cddbc26, /// 0x4d8
			 0x5443e030, /// 0x4dc
			 0x4982dc10, /// 0x4e0
			 0xf075a5a3, /// 0x4e4
			 0x83a444e5, /// 0x4e8
			 0xca5cfc21, /// 0x4ec
			 0x28660cf0, /// 0x4f0
			 0x444166c6, /// 0x4f4
			 0xb0242b31, /// 0x4f8
			 0x5a653d62, /// 0x4fc
			 0x6596bf4f, /// 0x500
			 0xb3977863, /// 0x504
			 0x9a80ad39, /// 0x508
			 0x037810fe, /// 0x50c
			 0x2a559d6f, /// 0x510
			 0x69484ec2, /// 0x514
			 0x3b7e1710, /// 0x518
			 0xfad2b51f, /// 0x51c
			 0xa0aae588, /// 0x520
			 0x01197fd4, /// 0x524
			 0xdc53f139, /// 0x528
			 0x5185d29f, /// 0x52c
			 0xbf1da1e7, /// 0x530
			 0x14855734, /// 0x534
			 0x0c9b4e06, /// 0x538
			 0xaece23a2, /// 0x53c
			 0xd48bc971, /// 0x540
			 0xeb1072cb, /// 0x544
			 0x0a8cc35f, /// 0x548
			 0x73a49d16, /// 0x54c
			 0x1cfe1ad6, /// 0x550
			 0xebda96c6, /// 0x554
			 0xd9003a21, /// 0x558
			 0x5de7fa06, /// 0x55c
			 0x2a8e6d98, /// 0x560
			 0x60605669, /// 0x564
			 0xf835be2d, /// 0x568
			 0xe59d2848, /// 0x56c
			 0x81411353, /// 0x570
			 0x2a9e15c6, /// 0x574
			 0x74daba52, /// 0x578
			 0x3bf80931, /// 0x57c
			 0xd0932557, /// 0x580
			 0x8035f172, /// 0x584
			 0x2741b12e, /// 0x588
			 0xc95f06c7, /// 0x58c
			 0x536964fe, /// 0x590
			 0xa28e226a, /// 0x594
			 0x45de62a1, /// 0x598
			 0xd76673cf, /// 0x59c
			 0xf77308e4, /// 0x5a0
			 0x01ded6e3, /// 0x5a4
			 0x829a51b4, /// 0x5a8
			 0x18df98b2, /// 0x5ac
			 0xc9081990, /// 0x5b0
			 0xd2fc89d2, /// 0x5b4
			 0xe549f352, /// 0x5b8
			 0x0d0ed6b5, /// 0x5bc
			 0x96e83427, /// 0x5c0
			 0x1b5971f3, /// 0x5c4
			 0x9be001ad, /// 0x5c8
			 0xdb2a8fd0, /// 0x5cc
			 0x0d58add0, /// 0x5d0
			 0xcdadbb6d, /// 0x5d4
			 0x8a7abe27, /// 0x5d8
			 0x04301454, /// 0x5dc
			 0xc93834dd, /// 0x5e0
			 0x9fd94098, /// 0x5e4
			 0x6d526f4d, /// 0x5e8
			 0xe0ab5045, /// 0x5ec
			 0x016943c0, /// 0x5f0
			 0xe99ee61f, /// 0x5f4
			 0x8a75b70f, /// 0x5f8
			 0x4442bd91, /// 0x5fc
			 0x7854d3f9, /// 0x600
			 0xeecff2e9, /// 0x604
			 0xc1a9b0da, /// 0x608
			 0xb507826f, /// 0x60c
			 0x6b0ecec9, /// 0x610
			 0xfe72b620, /// 0x614
			 0x8720aa5e, /// 0x618
			 0xcf588bab, /// 0x61c
			 0x4ec1a6ce, /// 0x620
			 0x028d4bf1, /// 0x624
			 0x44b6f20e, /// 0x628
			 0x7c583636, /// 0x62c
			 0xb522f4a1, /// 0x630
			 0x1966e51f, /// 0x634
			 0x9bdb41ee, /// 0x638
			 0x1b3bf342, /// 0x63c
			 0xc6f4cedd, /// 0x640
			 0x21bb20ba, /// 0x644
			 0xc92b57aa, /// 0x648
			 0x18a31caf, /// 0x64c
			 0x32835b5f, /// 0x650
			 0x92892a90, /// 0x654
			 0x9b00cb14, /// 0x658
			 0x944b9f36, /// 0x65c
			 0x11db30e7, /// 0x660
			 0x6b3362d4, /// 0x664
			 0xf5e2daf1, /// 0x668
			 0x78a7491b, /// 0x66c
			 0xd3212779, /// 0x670
			 0x97a6c444, /// 0x674
			 0x8e06b690, /// 0x678
			 0x7b927a17, /// 0x67c
			 0x338e7c55, /// 0x680
			 0xbcbbeb43, /// 0x684
			 0x6578d597, /// 0x688
			 0x6856ac59, /// 0x68c
			 0x2f581b06, /// 0x690
			 0xe8954c20, /// 0x694
			 0x8759bacb, /// 0x698
			 0x9368cba0, /// 0x69c
			 0xc8eb39dd, /// 0x6a0
			 0x4b2b76fc, /// 0x6a4
			 0xad5ae274, /// 0x6a8
			 0x5fc76534, /// 0x6ac
			 0x1df96cd1, /// 0x6b0
			 0xbed1c39f, /// 0x6b4
			 0x615ae16e, /// 0x6b8
			 0x9dfa6cb6, /// 0x6bc
			 0xc08a2444, /// 0x6c0
			 0x63684728, /// 0x6c4
			 0x758d41a0, /// 0x6c8
			 0x4540044f, /// 0x6cc
			 0xc1e4510e, /// 0x6d0
			 0x796cc5b0, /// 0x6d4
			 0x7c64aa2c, /// 0x6d8
			 0xff45c820, /// 0x6dc
			 0x09bf30a8, /// 0x6e0
			 0xfb735396, /// 0x6e4
			 0xaf96c9a8, /// 0x6e8
			 0xbd5224bf, /// 0x6ec
			 0x9b420ef8, /// 0x6f0
			 0x9dd45d84, /// 0x6f4
			 0xca893bd0, /// 0x6f8
			 0x6a920604, /// 0x6fc
			 0x5a45d22d, /// 0x700
			 0xa3933b10, /// 0x704
			 0x85e0d8f4, /// 0x708
			 0xa681c2c6, /// 0x70c
			 0x406e0aa0, /// 0x710
			 0x7d825de1, /// 0x714
			 0x4482fd5e, /// 0x718
			 0x9abcbea8, /// 0x71c
			 0xa97b03ef, /// 0x720
			 0x3126cf79, /// 0x724
			 0xcd6cf9f7, /// 0x728
			 0xa67c0e55, /// 0x72c
			 0x830c9a16, /// 0x730
			 0x538e10e9, /// 0x734
			 0xc0187c9c, /// 0x738
			 0xadff10b4, /// 0x73c
			 0xbe023ce6, /// 0x740
			 0x1d76cf00, /// 0x744
			 0xed784fe9, /// 0x748
			 0x3ef5b908, /// 0x74c
			 0x4e3feec7, /// 0x750
			 0x5db96e7b, /// 0x754
			 0x2789d42f, /// 0x758
			 0x79973a88, /// 0x75c
			 0xb0b76a0f, /// 0x760
			 0xfed36dc8, /// 0x764
			 0xfe9a29ee, /// 0x768
			 0x57b38b8b, /// 0x76c
			 0x9c670c47, /// 0x770
			 0x7a3eaf5e, /// 0x774
			 0x541f7e42, /// 0x778
			 0x2cf259cd, /// 0x77c
			 0x6aedaa0c, /// 0x780
			 0xc19968ee, /// 0x784
			 0x79479b11, /// 0x788
			 0xec8dc411, /// 0x78c
			 0x7292e271, /// 0x790
			 0x91aaaebc, /// 0x794
			 0x08785b4b, /// 0x798
			 0x718d3f07, /// 0x79c
			 0xa4443103, /// 0x7a0
			 0x722f2cc2, /// 0x7a4
			 0x387aedc4, /// 0x7a8
			 0x11c36a8a, /// 0x7ac
			 0xfe766b83, /// 0x7b0
			 0x3d1fd5be, /// 0x7b4
			 0xb494d650, /// 0x7b8
			 0xd7b4fcc5, /// 0x7bc
			 0x27ee0e28, /// 0x7c0
			 0x2b68c5bd, /// 0x7c4
			 0xc715a5c2, /// 0x7c8
			 0xdb10627c, /// 0x7cc
			 0xb8319bac, /// 0x7d0
			 0x41551811, /// 0x7d4
			 0xfefdf599, /// 0x7d8
			 0xd9bbc328, /// 0x7dc
			 0x4695f42e, /// 0x7e0
			 0xdefc01ad, /// 0x7e4
			 0x4e55cae2, /// 0x7e8
			 0xd7356422, /// 0x7ec
			 0x7c3fe6f5, /// 0x7f0
			 0x45213f8a, /// 0x7f4
			 0x5b027780, /// 0x7f8
			 0x14dcd1aa, /// 0x7fc
			 0xc5ec3ebe, /// 0x800
			 0x54f95d48, /// 0x804
			 0xb31456e2, /// 0x808
			 0x00433617, /// 0x80c
			 0x173968bc, /// 0x810
			 0x0f6aea92, /// 0x814
			 0x632d2bc5, /// 0x818
			 0x1e3ae3b9, /// 0x81c
			 0x12bdb722, /// 0x820
			 0x023d65ee, /// 0x824
			 0xefef5d24, /// 0x828
			 0x290b88fc, /// 0x82c
			 0x717917dc, /// 0x830
			 0x1754eb8c, /// 0x834
			 0xa2834b54, /// 0x838
			 0x3c17e058, /// 0x83c
			 0xb0fc5fef, /// 0x840
			 0xf966ca55, /// 0x844
			 0xe55a9e33, /// 0x848
			 0xd2d10fd9, /// 0x84c
			 0x325a11a8, /// 0x850
			 0x76d67d9a, /// 0x854
			 0xccba8a2b, /// 0x858
			 0x39ece7a1, /// 0x85c
			 0x8de867f1, /// 0x860
			 0xc7c1f8fb, /// 0x864
			 0xd4a79824, /// 0x868
			 0x5b172e4d, /// 0x86c
			 0x0e68b95b, /// 0x870
			 0xca3d6e95, /// 0x874
			 0x45db8c2b, /// 0x878
			 0xc5b6192a, /// 0x87c
			 0xde6d4bbe, /// 0x880
			 0x8904652e, /// 0x884
			 0x7d517178, /// 0x888
			 0xa356b285, /// 0x88c
			 0x4062e8bf, /// 0x890
			 0x96730540, /// 0x894
			 0x9f33294a, /// 0x898
			 0xa0f5cc13, /// 0x89c
			 0x314ed353, /// 0x8a0
			 0xab3c9fc4, /// 0x8a4
			 0xe9766a87, /// 0x8a8
			 0xdf137b67, /// 0x8ac
			 0xf97919c1, /// 0x8b0
			 0x1dbb7530, /// 0x8b4
			 0x34ca672c, /// 0x8b8
			 0x3574377c, /// 0x8bc
			 0x0ff938b5, /// 0x8c0
			 0x1a30f7e0, /// 0x8c4
			 0xb8741275, /// 0x8c8
			 0x23287f8e, /// 0x8cc
			 0x05aa58c3, /// 0x8d0
			 0x8d5e85e2, /// 0x8d4
			 0x7f92e9fc, /// 0x8d8
			 0x5e684bd5, /// 0x8dc
			 0xf7b5c275, /// 0x8e0
			 0x64170f2a, /// 0x8e4
			 0xd426744f, /// 0x8e8
			 0x47ad7356, /// 0x8ec
			 0xf3971616, /// 0x8f0
			 0x138bba42, /// 0x8f4
			 0xcf85434f, /// 0x8f8
			 0xf1a43d23, /// 0x8fc
			 0xdbb1feaf, /// 0x900
			 0x7bcf8e41, /// 0x904
			 0xade0a3d1, /// 0x908
			 0x2c613ad8, /// 0x90c
			 0xaf08bcca, /// 0x910
			 0xfd020300, /// 0x914
			 0x99f8b634, /// 0x918
			 0x5adb0896, /// 0x91c
			 0xe8d40997, /// 0x920
			 0x6de63992, /// 0x924
			 0x48e3e994, /// 0x928
			 0x68d32df1, /// 0x92c
			 0xbdce94ff, /// 0x930
			 0x665dd508, /// 0x934
			 0x373691a1, /// 0x938
			 0x9b46df4b, /// 0x93c
			 0xfc004391, /// 0x940
			 0x3de614aa, /// 0x944
			 0x8d077770, /// 0x948
			 0xb484b48c, /// 0x94c
			 0x81ea77ff, /// 0x950
			 0x20b3cd70, /// 0x954
			 0x93fc854f, /// 0x958
			 0x7c222330, /// 0x95c
			 0x12cd3b69, /// 0x960
			 0x800b5d6c, /// 0x964
			 0xcb5eb3a6, /// 0x968
			 0x8a8bb7c0, /// 0x96c
			 0x8d7d3424, /// 0x970
			 0x7d0c24a0, /// 0x974
			 0x6f7d7ca5, /// 0x978
			 0x66bbd805, /// 0x97c
			 0x2caff828, /// 0x980
			 0xe775a10b, /// 0x984
			 0x239fdd0a, /// 0x988
			 0x7bd28202, /// 0x98c
			 0x332e9e82, /// 0x990
			 0x379d6339, /// 0x994
			 0xd78e5dd2, /// 0x998
			 0xc9bc0425, /// 0x99c
			 0x3206e770, /// 0x9a0
			 0x8b1d6c2e, /// 0x9a4
			 0xccb5a364, /// 0x9a8
			 0x431275de, /// 0x9ac
			 0xcd4a543d, /// 0x9b0
			 0x55b94373, /// 0x9b4
			 0xf4e04c3d, /// 0x9b8
			 0xc22f7c35, /// 0x9bc
			 0xcead714c, /// 0x9c0
			 0xd16fadce, /// 0x9c4
			 0xd1ffa05d, /// 0x9c8
			 0xd7069b53, /// 0x9cc
			 0xe8392084, /// 0x9d0
			 0x330ef98e, /// 0x9d4
			 0x204f23f5, /// 0x9d8
			 0xfec132bd, /// 0x9dc
			 0x375b3433, /// 0x9e0
			 0x3eee33b2, /// 0x9e4
			 0x196aff6e, /// 0x9e8
			 0x6993186b, /// 0x9ec
			 0x61c21189, /// 0x9f0
			 0x7ba473ae, /// 0x9f4
			 0x46fbd22e, /// 0x9f8
			 0xd863b40d, /// 0x9fc
			 0xfac7f55a, /// 0xa00
			 0xd5082d37, /// 0xa04
			 0x211e7d81, /// 0xa08
			 0xdcccd6aa, /// 0xa0c
			 0xdc2c7d47, /// 0xa10
			 0x04496309, /// 0xa14
			 0x3ebc6516, /// 0xa18
			 0xb3971855, /// 0xa1c
			 0x63b74f1a, /// 0xa20
			 0x8c90358c, /// 0xa24
			 0xc21e3aea, /// 0xa28
			 0x0443c3b1, /// 0xa2c
			 0xc619438c, /// 0xa30
			 0x09f70641, /// 0xa34
			 0x3748d89e, /// 0xa38
			 0x7690f9a2, /// 0xa3c
			 0xb2820fcb, /// 0xa40
			 0xddbce8d1, /// 0xa44
			 0x691a1b18, /// 0xa48
			 0xdb6db3bb, /// 0xa4c
			 0x488b6d37, /// 0xa50
			 0x02d2e788, /// 0xa54
			 0x44abd28a, /// 0xa58
			 0x089e81e1, /// 0xa5c
			 0xa052b043, /// 0xa60
			 0x7ebb5ba5, /// 0xa64
			 0xaa7ef830, /// 0xa68
			 0xff54fcfd, /// 0xa6c
			 0x868cc097, /// 0xa70
			 0x7524be6c, /// 0xa74
			 0x1b2b4ee6, /// 0xa78
			 0xc0e1008a, /// 0xa7c
			 0xb71336cc, /// 0xa80
			 0xb91f3db7, /// 0xa84
			 0xec3ceb19, /// 0xa88
			 0xdc58eea9, /// 0xa8c
			 0x70daa694, /// 0xa90
			 0x91aad21b, /// 0xa94
			 0x59743278, /// 0xa98
			 0x8fef14c1, /// 0xa9c
			 0x63ef9d61, /// 0xaa0
			 0x5e5ee68c, /// 0xaa4
			 0xee9ddcf1, /// 0xaa8
			 0x714ce11d, /// 0xaac
			 0x0ce3ab87, /// 0xab0
			 0xae6445bc, /// 0xab4
			 0xa9ef29cd, /// 0xab8
			 0x70a5f4d8, /// 0xabc
			 0x8e216f22, /// 0xac0
			 0xd1982531, /// 0xac4
			 0xe6c74ecd, /// 0xac8
			 0xaebe4784, /// 0xacc
			 0x3c188e8f, /// 0xad0
			 0x421b3a78, /// 0xad4
			 0x8d741214, /// 0xad8
			 0xdf7dc313, /// 0xadc
			 0xa18421e3, /// 0xae0
			 0x5d767a0f, /// 0xae4
			 0x63c3e3e9, /// 0xae8
			 0x8aa6033b, /// 0xaec
			 0x4b5579e9, /// 0xaf0
			 0xe04d33c5, /// 0xaf4
			 0x58fc2f43, /// 0xaf8
			 0x80ebe909, /// 0xafc
			 0x73f3efe7, /// 0xb00
			 0x3bbf4c3f, /// 0xb04
			 0x3cecf483, /// 0xb08
			 0xcdbc3c45, /// 0xb0c
			 0x99fff783, /// 0xb10
			 0xdbef8366, /// 0xb14
			 0x73ee6eff, /// 0xb18
			 0x9643244f, /// 0xb1c
			 0xd12f4b3a, /// 0xb20
			 0xfcf43223, /// 0xb24
			 0x5c2b69d1, /// 0xb28
			 0x61bd3bfd, /// 0xb2c
			 0x2378c207, /// 0xb30
			 0x335e8144, /// 0xb34
			 0xe295f25f, /// 0xb38
			 0x8e8df757, /// 0xb3c
			 0x1ebe9050, /// 0xb40
			 0x21635fea, /// 0xb44
			 0x83eb87d0, /// 0xb48
			 0xf560227e, /// 0xb4c
			 0x19df3293, /// 0xb50
			 0x394f8058, /// 0xb54
			 0xd062073e, /// 0xb58
			 0x2f56a8a9, /// 0xb5c
			 0x46350b06, /// 0xb60
			 0x3ec9947c, /// 0xb64
			 0xbd268d80, /// 0xb68
			 0xd785fdf9, /// 0xb6c
			 0x9f192fcf, /// 0xb70
			 0x194d8b1d, /// 0xb74
			 0xab3baaaa, /// 0xb78
			 0xc0b8b080, /// 0xb7c
			 0x66b260d6, /// 0xb80
			 0xaa6c0231, /// 0xb84
			 0xfaaf7036, /// 0xb88
			 0xdd76e479, /// 0xb8c
			 0x96825b74, /// 0xb90
			 0xed840bac, /// 0xb94
			 0xb4d6da39, /// 0xb98
			 0x87ac5b58, /// 0xb9c
			 0xba6e8aa3, /// 0xba0
			 0xa75b9e3d, /// 0xba4
			 0x2b1e244f, /// 0xba8
			 0x354ebfbd, /// 0xbac
			 0xa14d19e8, /// 0xbb0
			 0xd5cc46b3, /// 0xbb4
			 0xf040a54e, /// 0xbb8
			 0x3fd3cc60, /// 0xbbc
			 0x5714a3ff, /// 0xbc0
			 0x0f17ee67, /// 0xbc4
			 0x3e325bbc, /// 0xbc8
			 0xfee54b29, /// 0xbcc
			 0x6bb0551c, /// 0xbd0
			 0x2feb28b3, /// 0xbd4
			 0x85b67e1d, /// 0xbd8
			 0xa215fd4c, /// 0xbdc
			 0x5e3af0be, /// 0xbe0
			 0x65b6d897, /// 0xbe4
			 0x86ebc934, /// 0xbe8
			 0x1fab3492, /// 0xbec
			 0xd911069c, /// 0xbf0
			 0x9a009e76, /// 0xbf4
			 0x8db3d43b, /// 0xbf8
			 0xeadd4833, /// 0xbfc
			 0x1e43ed9a, /// 0xc00
			 0xea0d993d, /// 0xc04
			 0xd909c24a, /// 0xc08
			 0x9de4bc49, /// 0xc0c
			 0x900aed75, /// 0xc10
			 0xdcbf05ab, /// 0xc14
			 0xf06428c4, /// 0xc18
			 0x6c58f1ba, /// 0xc1c
			 0xc83433e0, /// 0xc20
			 0x2e9dcb45, /// 0xc24
			 0xb80d3bdb, /// 0xc28
			 0x6cfc8054, /// 0xc2c
			 0xfb5b6a1a, /// 0xc30
			 0x07864d8d, /// 0xc34
			 0xc0842d97, /// 0xc38
			 0x629af2fa, /// 0xc3c
			 0x95ab6dc8, /// 0xc40
			 0xcbfeab30, /// 0xc44
			 0xe363b90e, /// 0xc48
			 0xfc511793, /// 0xc4c
			 0xe543a18e, /// 0xc50
			 0x42af5d6f, /// 0xc54
			 0x7680ee6b, /// 0xc58
			 0x1de203ec, /// 0xc5c
			 0xc4db2fe8, /// 0xc60
			 0x6dd36a15, /// 0xc64
			 0x94990233, /// 0xc68
			 0xa9ed7e05, /// 0xc6c
			 0xca802066, /// 0xc70
			 0xa292164d, /// 0xc74
			 0x5d1cb0a4, /// 0xc78
			 0x62e6abfc, /// 0xc7c
			 0xf2e90d2f, /// 0xc80
			 0x25299827, /// 0xc84
			 0x29b881a0, /// 0xc88
			 0xaa816c2a, /// 0xc8c
			 0xd6501026, /// 0xc90
			 0xc5395662, /// 0xc94
			 0x11dd04e5, /// 0xc98
			 0xfdb358ed, /// 0xc9c
			 0xb89aeccc, /// 0xca0
			 0x944ae786, /// 0xca4
			 0x111f6049, /// 0xca8
			 0xa4322f3a, /// 0xcac
			 0x0eb86962, /// 0xcb0
			 0x436528dc, /// 0xcb4
			 0x407cc6be, /// 0xcb8
			 0x73860a90, /// 0xcbc
			 0xb8ec33c0, /// 0xcc0
			 0x7c5fc115, /// 0xcc4
			 0x7e3dfb2c, /// 0xcc8
			 0x98bc98c6, /// 0xccc
			 0x1a29be43, /// 0xcd0
			 0xe0c3a640, /// 0xcd4
			 0xb9f8a080, /// 0xcd8
			 0x90f2cd67, /// 0xcdc
			 0x9d9a17c4, /// 0xce0
			 0xe8dc280b, /// 0xce4
			 0xc699a777, /// 0xce8
			 0x59e5049e, /// 0xcec
			 0x5dad927d, /// 0xcf0
			 0x07bd370e, /// 0xcf4
			 0xad921dfd, /// 0xcf8
			 0x769514b5, /// 0xcfc
			 0x9ead3fa7, /// 0xd00
			 0xe3d6db87, /// 0xd04
			 0xbc3b2111, /// 0xd08
			 0xa76ed0df, /// 0xd0c
			 0xd5cb5010, /// 0xd10
			 0xdb85627d, /// 0xd14
			 0x808cc17a, /// 0xd18
			 0x87822112, /// 0xd1c
			 0x139fe3ad, /// 0xd20
			 0x8085a160, /// 0xd24
			 0x07f97d32, /// 0xd28
			 0x1faf61cd, /// 0xd2c
			 0xc6946beb, /// 0xd30
			 0x2724e1c8, /// 0xd34
			 0x78427a3f, /// 0xd38
			 0x4a68d858, /// 0xd3c
			 0xa9373803, /// 0xd40
			 0x16fba0a1, /// 0xd44
			 0x098cdc9e, /// 0xd48
			 0xa767edfa, /// 0xd4c
			 0x00cdedc2, /// 0xd50
			 0x52e5d8ae, /// 0xd54
			 0xd50193dd, /// 0xd58
			 0xb4251a79, /// 0xd5c
			 0xd71d188e, /// 0xd60
			 0xab6a105d, /// 0xd64
			 0x09d2b789, /// 0xd68
			 0xd9f5ef66, /// 0xd6c
			 0x3d7f9388, /// 0xd70
			 0x3de9348b, /// 0xd74
			 0x939732e7, /// 0xd78
			 0xc864963e, /// 0xd7c
			 0x34c6b1ea, /// 0xd80
			 0x7449555a, /// 0xd84
			 0xb97024af, /// 0xd88
			 0x45304997, /// 0xd8c
			 0x27c97ef0, /// 0xd90
			 0x15f53218, /// 0xd94
			 0xa87658f1, /// 0xd98
			 0x40b2504a, /// 0xd9c
			 0x93faee8e, /// 0xda0
			 0xe284f9c8, /// 0xda4
			 0x8e90befb, /// 0xda8
			 0xc6f0261b, /// 0xdac
			 0x6095869c, /// 0xdb0
			 0xf8d354aa, /// 0xdb4
			 0xf8d573a1, /// 0xdb8
			 0x56ac1804, /// 0xdbc
			 0x12134642, /// 0xdc0
			 0xcbc4348f, /// 0xdc4
			 0x250938aa, /// 0xdc8
			 0x067e0f67, /// 0xdcc
			 0xac4b1963, /// 0xdd0
			 0x2530b288, /// 0xdd4
			 0x7b5dec96, /// 0xdd8
			 0x5062aa30, /// 0xddc
			 0x2b3ea073, /// 0xde0
			 0xeeaa17ec, /// 0xde4
			 0x46210f29, /// 0xde8
			 0x93a71bdf, /// 0xdec
			 0xb58ec584, /// 0xdf0
			 0x7adfca98, /// 0xdf4
			 0x79de5a7a, /// 0xdf8
			 0xc69a800e, /// 0xdfc
			 0x0775028e, /// 0xe00
			 0xf8ec0364, /// 0xe04
			 0x732c93fa, /// 0xe08
			 0x2eea6b29, /// 0xe0c
			 0xc4fa5a7a, /// 0xe10
			 0x0d50e0be, /// 0xe14
			 0x1318e801, /// 0xe18
			 0x19af7b83, /// 0xe1c
			 0xc6ebdfd6, /// 0xe20
			 0xb0af67b0, /// 0xe24
			 0x6bfc2d3c, /// 0xe28
			 0x546066b2, /// 0xe2c
			 0xfb9dd179, /// 0xe30
			 0x8861cfe8, /// 0xe34
			 0x800e6aab, /// 0xe38
			 0x49f991c2, /// 0xe3c
			 0xdc4d1c33, /// 0xe40
			 0x5e9f65ba, /// 0xe44
			 0xe900addd, /// 0xe48
			 0xa663c998, /// 0xe4c
			 0x1cff034b, /// 0xe50
			 0x9ba99c84, /// 0xe54
			 0xcd8052b6, /// 0xe58
			 0x26b054dc, /// 0xe5c
			 0xd2602fd7, /// 0xe60
			 0xf5926a70, /// 0xe64
			 0x7f29e714, /// 0xe68
			 0xaf84a064, /// 0xe6c
			 0xbc25d39c, /// 0xe70
			 0xaa4c0e84, /// 0xe74
			 0x791ea17c, /// 0xe78
			 0xd3e3a56b, /// 0xe7c
			 0xed04adcc, /// 0xe80
			 0x5953b592, /// 0xe84
			 0x5b467ffb, /// 0xe88
			 0x2de51bbd, /// 0xe8c
			 0x9c31f532, /// 0xe90
			 0x19db5b3d, /// 0xe94
			 0x86d7f6a4, /// 0xe98
			 0xa92f0657, /// 0xe9c
			 0x9d69d66d, /// 0xea0
			 0xdec83297, /// 0xea4
			 0x6acef358, /// 0xea8
			 0x7ee66f9f, /// 0xeac
			 0xaeb2c9f6, /// 0xeb0
			 0x02e9e5b6, /// 0xeb4
			 0xacd50e62, /// 0xeb8
			 0xa855122c, /// 0xebc
			 0x0efe0603, /// 0xec0
			 0xa5326dec, /// 0xec4
			 0x18b0e107, /// 0xec8
			 0x2271f784, /// 0xecc
			 0x5e3cc899, /// 0xed0
			 0xbdb0c5ac, /// 0xed4
			 0x13867db8, /// 0xed8
			 0x26567b77, /// 0xedc
			 0x7ee4524a, /// 0xee0
			 0x7959ebda, /// 0xee4
			 0x9c8ed29c, /// 0xee8
			 0x9b4bc875, /// 0xeec
			 0x4f86216e, /// 0xef0
			 0x570077e3, /// 0xef4
			 0x7435c1d3, /// 0xef8
			 0x53f81f96, /// 0xefc
			 0x3f2dce8d, /// 0xf00
			 0xb8ca924c, /// 0xf04
			 0x9974181a, /// 0xf08
			 0xade990df, /// 0xf0c
			 0xa2dd3048, /// 0xf10
			 0xb887713f, /// 0xf14
			 0xcecfe48a, /// 0xf18
			 0x243e6ea6, /// 0xf1c
			 0x06b30681, /// 0xf20
			 0xa4f02c4c, /// 0xf24
			 0xbf23de51, /// 0xf28
			 0xcbaf4044, /// 0xf2c
			 0xc2fc468a, /// 0xf30
			 0x36ea4a17, /// 0xf34
			 0x7e2d0a76, /// 0xf38
			 0xa04e5464, /// 0xf3c
			 0x095b86a4, /// 0xf40
			 0x69aee704, /// 0xf44
			 0x5a5314b0, /// 0xf48
			 0x8b65fd85, /// 0xf4c
			 0xdf884270, /// 0xf50
			 0x68874ea1, /// 0xf54
			 0xf098718d, /// 0xf58
			 0x6922f5bf, /// 0xf5c
			 0x07e0ccd0, /// 0xf60
			 0x6bb5aa32, /// 0xf64
			 0x2206b6ba, /// 0xf68
			 0xc62e4d69, /// 0xf6c
			 0x7bc30f85, /// 0xf70
			 0xd4e7eac0, /// 0xf74
			 0xe2923f25, /// 0xf78
			 0x05fb7d78, /// 0xf7c
			 0x32d57965, /// 0xf80
			 0xf671dcb3, /// 0xf84
			 0x567cec41, /// 0xf88
			 0x14bcc6cc, /// 0xf8c
			 0x522a53a1, /// 0xf90
			 0x3982a02b, /// 0xf94
			 0xb39b0f41, /// 0xf98
			 0xe57f75b5, /// 0xf9c
			 0x8fee6118, /// 0xfa0
			 0x4a55d222, /// 0xfa4
			 0xe8e97519, /// 0xfa8
			 0xa53b56b5, /// 0xfac
			 0x266520bf, /// 0xfb0
			 0x85de368d, /// 0xfb4
			 0x17973d79, /// 0xfb8
			 0x735ab9f2, /// 0xfbc
			 0xb332d908, /// 0xfc0
			 0x6f22291a, /// 0xfc4
			 0x95ba8767, /// 0xfc8
			 0x8e2be6b2, /// 0xfcc
			 0x4eb27948, /// 0xfd0
			 0xeb5ca022, /// 0xfd4
			 0x40baaccd, /// 0xfd8
			 0x346b02cd, /// 0xfdc
			 0xa4270024, /// 0xfe0
			 0x95c003df, /// 0xfe4
			 0x35bcdb60, /// 0xfe8
			 0x9a0bcb1a, /// 0xfec
			 0x8bd127c3, /// 0xff0
			 0xc9b832f8, /// 0xff4
			 0x29a9e725, /// 0xff8
			 0xf0f85785 /// last
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
			 0x00000010,
			 0x0000019c,
			 0x00000380,
			 0x000001d8,
			 0x00000088,
			 0x0000062c,
			 0x00000754,
			 0x00000278,

			 /// vpu register f2
			 0x41100000,
			 0x41e00000,
			 0x41b00000,
			 0x40e00000,
			 0x41c80000,
			 0x41200000,
			 0x41500000,
			 0x40e00000,

			 /// vpu register f3
			 0x41c00000,
			 0x41d80000,
			 0x41880000,
			 0x41e00000,
			 0x41d00000,
			 0x41700000,
			 0x41900000,
			 0x41100000,

			 /// vpu register f4
			 0x41b00000,
			 0x40400000,
			 0x41f00000,
			 0x41200000,
			 0x40c00000,
			 0x41300000,
			 0x40800000,
			 0x41500000,

			 /// vpu register f5
			 0x41880000,
			 0x41a00000,
			 0x40a00000,
			 0x41700000,
			 0x40e00000,
			 0x3f800000,
			 0x41b80000,
			 0x41b00000,

			 /// vpu register f6
			 0x41e00000,
			 0x40400000,
			 0x40400000,
			 0x41980000,
			 0x41700000,
			 0x41500000,
			 0x3f800000,
			 0x41c80000,

			 /// vpu register f7
			 0x40e00000,
			 0x41800000,
			 0x40000000,
			 0x41d80000,
			 0x41c80000,
			 0x40000000,
			 0x41980000,
			 0x40800000,

			 /// vpu register f8
			 0x41c00000,
			 0x41c00000,
			 0x41b80000,
			 0x41f80000,
			 0x3f800000,
			 0x41800000,
			 0x41900000,
			 0x41b00000,

			 /// vpu register f9
			 0x41b80000,
			 0x41980000,
			 0x41e80000,
			 0x41000000,
			 0x41800000,
			 0x41700000,
			 0x41e00000,
			 0x41d00000,

			 /// vpu register f10
			 0x42000000,
			 0x41400000,
			 0x40e00000,
			 0x41d80000,
			 0x41800000,
			 0x41a00000,
			 0x41400000,
			 0x41880000,

			 /// vpu register f11
			 0x40000000,
			 0x41500000,
			 0x41e00000,
			 0x3f800000,
			 0x40400000,
			 0x41a00000,
			 0x41a00000,
			 0x41000000,

			 /// vpu register f12
			 0x41f80000,
			 0x41980000,
			 0x41600000,
			 0x41600000,
			 0x40800000,
			 0x41a00000,
			 0x41d00000,
			 0x41100000,

			 /// vpu register f13
			 0x41880000,
			 0x41100000,
			 0x41d80000,
			 0x41b00000,
			 0x40800000,
			 0x41400000,
			 0x41f80000,
			 0x41f00000,

			 /// vpu register f14
			 0x41c00000,
			 0x40800000,
			 0x41500000,
			 0x41a00000,
			 0x41c00000,
			 0x3f800000,
			 0x41600000,
			 0x41e00000,

			 /// vpu register f15
			 0x41c00000,
			 0x40a00000,
			 0x41c00000,
			 0x41d00000,
			 0x41a00000,
			 0x41a80000,
			 0x41100000,
			 0x41200000,

			 /// vpu register f16
			 0x41900000,
			 0x3f800000,
			 0x41a00000,
			 0x41100000,
			 0x41c80000,
			 0x42000000,
			 0x40c00000,
			 0x41c80000,

			 /// vpu register f17
			 0x41d80000,
			 0x41c00000,
			 0x41980000,
			 0x41f00000,
			 0x41c80000,
			 0x41b00000,
			 0x41c80000,
			 0x40400000,

			 /// vpu register f18
			 0x41880000,
			 0x41880000,
			 0x41b00000,
			 0x40c00000,
			 0x41800000,
			 0x41e00000,
			 0x41b00000,
			 0x40a00000,

			 /// vpu register f19
			 0x41200000,
			 0x40400000,
			 0x41d00000,
			 0x40800000,
			 0x41400000,
			 0x42000000,
			 0x41a00000,
			 0x41b80000,

			 /// vpu register f20
			 0x41a80000,
			 0x41f00000,
			 0x41d00000,
			 0x41f00000,
			 0x42000000,
			 0x41e80000,
			 0x41d80000,
			 0x41f80000,

			 /// vpu register f21
			 0x41f80000,
			 0x41d80000,
			 0x41500000,
			 0x42000000,
			 0x41a80000,
			 0x41d80000,
			 0x41980000,
			 0x41a80000,

			 /// vpu register f22
			 0x41f00000,
			 0x40a00000,
			 0x40000000,
			 0x41400000,
			 0x41c80000,
			 0x41c00000,
			 0x41a00000,
			 0x41a80000,

			 /// vpu register f23
			 0x41980000,
			 0x40000000,
			 0x41880000,
			 0x41880000,
			 0x41100000,
			 0x41d00000,
			 0x41e00000,
			 0x41d80000,

			 /// vpu register f24
			 0x41400000,
			 0x41980000,
			 0x41e00000,
			 0x41800000,
			 0x40000000,
			 0x41e00000,
			 0x41000000,
			 0x40400000,

			 /// vpu register f25
			 0x42000000,
			 0x41c80000,
			 0x41b00000,
			 0x41d80000,
			 0x41800000,
			 0x41e00000,
			 0x40000000,
			 0x41b80000,

			 /// vpu register f26
			 0x41b80000,
			 0x41f00000,
			 0x41c80000,
			 0x41a00000,
			 0x40c00000,
			 0x41a80000,
			 0x40e00000,
			 0x40e00000,

			 /// vpu register f27
			 0x41900000,
			 0x41300000,
			 0x40000000,
			 0x41900000,
			 0x41f00000,
			 0x42000000,
			 0x41e00000,
			 0x41a00000,

			 /// vpu register f28
			 0x41f00000,
			 0x41d00000,
			 0x40a00000,
			 0x42000000,
			 0x40800000,
			 0x41a80000,
			 0x41500000,
			 0x42000000,

			 /// vpu register f29
			 0x41e80000,
			 0x41f00000,
			 0x40a00000,
			 0x40a00000,
			 0x40e00000,
			 0x41f00000,
			 0x41500000,
			 0x41100000,

			 /// vpu register f30
			 0x41600000,
			 0x40a00000,
			 0x40a00000,
			 0x41a00000,
			 0x41e80000,
			 0x41b80000,
			 0x41d00000,
			 0x41100000,

			 /// vpu register f31
			 0x40c00000,
			 0x41e80000,
			 0x41000000,
			 0x41b00000,
			 0x41900000,
			 0x41d80000,
			 0x41200000,
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
		"mova.m.x x11\n"                                      ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x12\n"                                      ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x12\n"                                      ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x12\n"                                      ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x14\n"                                      ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x11\n"                                      ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x13\n"                                      ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x11\n"                                      ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x13\n"                                      ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x12\n"                                      ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x14\n"                                      ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x12\n"                                      ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x15\n"                                      ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x13\n"                                      ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x12\n"                                      ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x14\n"                                      ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x11\n"                                      ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x12\n"                                      ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x15\n"                                      ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x13\n"                                      ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x12\n"                                      ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x14\n"                                      ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x12\n"                                      ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x11\n"                                      ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x11\n"                                      ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x11\n"                                      ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x12\n"                                      ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x11\n"                                      ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x15\n"                                      ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x12\n"                                      ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x12\n"                                      ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x14\n"                                      ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x13\n"                                      ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x13\n"                                      ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x15\n"                                      ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x13\n"                                      ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x15\n"                                      ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x14\n"                                      ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x14\n"                                      ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x12\n"                                      ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x13\n"                                      ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x14\n"                                      ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x11\n"                                      ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x12\n"                                      ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x12\n"                                      ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x11\n"                                      ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x13\n"                                      ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x14\n"                                      ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x11\n"                                      ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x11\n"                                      ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x14\n"                                      ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x13\n"                                      ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x15\n"                                      ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x13\n"                                      ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x12\n"                                      ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x11\n"                                      ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x12\n"                                      ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x12\n"                                      ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x13\n"                                      ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x11\n"                                      ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x13\n"                                      ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x12\n"                                      ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x12\n"                                      ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x11\n"                                      ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x15\n"                                      ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x15\n"                                      ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x14\n"                                      ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x11\n"                                      ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x14\n"                                      ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x15\n"                                      ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x11\n"                                      ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x14\n"                                      ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x11\n"                                      ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x13\n"                                      ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x12\n"                                      ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x14\n"                                      ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x14\n"                                      ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x15\n"                                      ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x11\n"                                      ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x12\n"                                      ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x13\n"                                      ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x12\n"                                      ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x14\n"                                      ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x12\n"                                      ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x15\n"                                      ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x15\n"                                      ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x11\n"                                      ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x14\n"                                      ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x11\n"                                      ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x15\n"                                      ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x13\n"                                      ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x11\n"                                      ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x12\n"                                      ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x11\n"                                      ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x13\n"                                      ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x12\n"                                      ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x15\n"                                      ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x13\n"                                      ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"mova.m.x x14\n"                                      ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
