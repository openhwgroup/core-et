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
			 0x0c600000,                                                  // Leading 1s:                                 /// 00000
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0000c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00010
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00014
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00018
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0001c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00020
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x0c700000,                                                  // Leading 1s:                                 /// 00028
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0002c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00030
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00034
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00038
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0003c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00040
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00044
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0004c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00054
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0005c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00060
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0006c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00070
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00074
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00078
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0007c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00080
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00084
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00088
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00090
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00094
			 0x0e000001,                                                  // Trailing 1s:                                /// 00098
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0009c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000a0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000a4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000a8
			 0x0c700000,                                                  // Leading 1s:                                 /// 000ac
			 0x0c400000,                                                  // Leading 1s:                                 /// 000b0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000b4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000b8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000bc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000cc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000d8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000dc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000e0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000e4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000ec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000f0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000f4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00100
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00104
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00108
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0010c
			 0x7fc00001,                                                  // signaling NaN                               /// 00110
			 0x0c600000,                                                  // Leading 1s:                                 /// 00114
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00118
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0011c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00120
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00124
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00128
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0012c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00130
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00134
			 0x00000000,                                                  // zero                                        /// 00138
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0013c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00140
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00148
			 0x00011111,                                                  // 9.7958E-41                                  /// 0014c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x3f028f5c,                                                  // 0.51                                        /// 00154
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00158
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0015c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00164
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00168
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0016c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00174
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00178
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0017c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00180
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00184
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00188
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00190
			 0xcb000000,                                                  // -8388608.0                                  /// 00194
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00198
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001a0
			 0xcb000000,                                                  // -8388608.0                                  /// 001a4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x80011111,                                                  // -9.7958E-41                                 /// 001ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001b4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001c0
			 0x0e000007,                                                  // Trailing 1s:                                /// 001c4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001c8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001cc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001d4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001e0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001e4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x0e000007,                                                  // Trailing 1s:                                /// 001f0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001fc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00200
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00204
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00208
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0020c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00210
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00214
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00218
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0021c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00224
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x0e000003,                                                  // Trailing 1s:                                /// 0022c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00230
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00234
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00238
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0023c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00240
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00244
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00248
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0024c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00250
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00254
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00258
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00260
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00264
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00268
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0026c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00270
			 0x3f028f5c,                                                  // 0.51                                        /// 00274
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00278
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00280
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00288
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00290
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00294
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00298
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002a0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002a4
			 0xff800000,                                                  // -inf                                        /// 002a8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002b8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002bc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002c4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002c8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002cc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002d0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002d8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002dc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002e0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002e4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002f0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002f4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00300
			 0x00011111,                                                  // 9.7958E-41                                  /// 00304
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00308
			 0x55555555,                                                  // 4 random values                             /// 0030c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00310
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00314
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00318
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00328
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00330
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00334
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00338
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0033c
			 0x4b000000,                                                  // 8388608.0                                   /// 00340
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00344
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0034c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00354
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00358
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0035c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0xff800000,                                                  // -inf                                        /// 00374
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00378
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0037c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00380
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00384
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00388
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0038c
			 0x4b000000,                                                  // 8388608.0                                   /// 00390
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00394
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00398
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0039c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003a0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003a8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003ac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003c0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003c4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003c8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003cc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003d0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003d4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0x00011111,                                                  // 9.7958E-41                                  /// 003e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003e8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003f0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003fc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00400
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00404
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00408
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00410
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00414
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00418
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00420
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00424
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0042c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00430
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00434
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00438
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0043c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00440
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0044c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00454
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0045c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00460
			 0x00011111,                                                  // 9.7958E-41                                  /// 00464
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00470
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00478
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0047c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00480
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00484
			 0x4b000000,                                                  // 8388608.0                                   /// 00488
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00490
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00494
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00498
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0049c
			 0x0c780000,                                                  // Leading 1s:                                 /// 004a0
			 0x3f028f5c,                                                  // 0.51                                        /// 004a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004a8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004ac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004b0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004b8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004bc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004c0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004c4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004c8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004cc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004d0
			 0x55555555,                                                  // 4 random values                             /// 004d4
			 0x0c600000,                                                  // Leading 1s:                                 /// 004d8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004e0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004e4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004e8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004f0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004f4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 004fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00500
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00504
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00508
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0050c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00510
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00518
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0051c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00520
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00524
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00530
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00538
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00544
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00548
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0054c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00554
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00558
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0055c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00560
			 0xff800000,                                                  // -inf                                        /// 00564
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00568
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0056c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00570
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00574
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00578
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0057c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00584
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00588
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0058c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00598
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0059c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005b8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005bc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005c0
			 0x3f028f5c,                                                  // 0.51                                        /// 005c4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005c8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005d0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 005d8
			 0xcb000000,                                                  // -8388608.0                                  /// 005dc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005e0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005e8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005f0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005f8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005fc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00600
			 0x0c400000,                                                  // Leading 1s:                                 /// 00604
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00608
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0060c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00610
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00614
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00618
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0061c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00620
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0062c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00630
			 0x7fc00001,                                                  // signaling NaN                               /// 00634
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00638
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0063c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00640
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00648
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x4b000000,                                                  // 8388608.0                                   /// 00650
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00654
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00658
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0065c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00664
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0066c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00670
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00678
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00680
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00684
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00688
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0068c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00690
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00694
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00698
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0069c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006a0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006b4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006b8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006bc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006c0
			 0x0c700000,                                                  // Leading 1s:                                 /// 006c4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006c8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006d4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006d8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006dc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006e0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006e4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006e8
			 0xcb000000,                                                  // -8388608.0                                  /// 006ec
			 0x55555555,                                                  // 4 random values                             /// 006f0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006f8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006fc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00700
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0070c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00714
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00724
			 0x7fc00001,                                                  // signaling NaN                               /// 00728
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0072c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00730
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00738
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00744
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00748
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0074c
			 0xcb000000,                                                  // -8388608.0                                  /// 00750
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00754
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00758
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00760
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00764
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00768
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00770
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00774
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00778
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0077c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00780
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00784
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00788
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00790
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00794
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00798
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0079c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007a4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007a8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007ac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007b0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007b8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007bc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007c0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007c4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007cc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007d0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007d8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007dc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007e0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007e4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007e8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007f0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007f4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007f8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007fc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00808
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0080c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00810
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00814
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00820
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00824
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0082c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00830
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x80011111,                                                  // -9.7958E-41                                 /// 00838
			 0x33333333,                                                  // 4 random values                             /// 0083c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00840
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00844
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00848
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0084c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00854
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0085c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00860
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00864
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00870
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00874
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0087c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00880
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00884
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00888
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0088c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00890
			 0x0c400000,                                                  // Leading 1s:                                 /// 00894
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00898
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0089c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x80000000,                                                  // -zero                                       /// 008a4
			 0x00000000,                                                  // zero                                        /// 008a8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008ac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008b0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008b4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008b8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008bc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 008c0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008c4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008c8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008d0
			 0x80000000,                                                  // -zero                                       /// 008d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008d8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008dc
			 0xffc00001,                                                  // -signaling NaN                              /// 008e0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008e4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008e8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008ec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008f0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008f8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008fc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00900
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00904
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00908
			 0x0e000001,                                                  // Trailing 1s:                                /// 0090c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00914
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0091c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00920
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0092c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00930
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x0e000007,                                                  // Trailing 1s:                                /// 0093c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00940
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00944
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00948
			 0x0c700000,                                                  // Leading 1s:                                 /// 0094c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00950
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00954
			 0x33333333,                                                  // 4 random values                             /// 00958
			 0x80011111,                                                  // -9.7958E-41                                 /// 0095c
			 0x55555555,                                                  // 4 random values                             /// 00960
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00968
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0096c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00978
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0097c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00980
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00988
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00990
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00998
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0099c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009a0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009a8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009ac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009b0
			 0x00011111,                                                  // 9.7958E-41                                  /// 009b4
			 0x0c400000,                                                  // Leading 1s:                                 /// 009b8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009bc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009c0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009c4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009c8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009cc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009d0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009d4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009d8
			 0x0e000003,                                                  // Trailing 1s:                                /// 009dc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009e0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x4b000000,                                                  // 8388608.0                                   /// 009ec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009f0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009f4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009fc
			 0x33333333,                                                  // 4 random values                             /// 00a00
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a08
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a0c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a10
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a14
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a18
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a1c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a20
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a28
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a34
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a3c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0xff800000,                                                  // -inf                                        /// 00a44
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a48
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a4c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a50
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a54
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a5c
			 0x55555555,                                                  // 4 random values                             /// 00a60
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a64
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a68
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a6c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a70
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a78
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a7c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a80
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a88
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a8c
			 0x3f028f5c,                                                  // 0.51                                        /// 00a90
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a94
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0xbf028f5c,                                                  // -0.51                                       /// 00a9c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00aa0
			 0x7fc00001,                                                  // signaling NaN                               /// 00aa4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00aa8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00aac
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ab0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ab4
			 0x33333333,                                                  // 4 random values                             /// 00ab8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00abc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ac0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ac4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ac8
			 0x55555555,                                                  // 4 random values                             /// 00acc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ad0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ad4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ad8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00adc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ae0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ae4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00aec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00af0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00af4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00af8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00afc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b00
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b04
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b08
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b0c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b18
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b1c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b20
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b24
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b28
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b2c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b34
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b38
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b3c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b40
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b48
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b4c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b50
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b58
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b5c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b60
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b64
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b68
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b6c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b70
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b74
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b7c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b80
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b8c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b90
			 0x7f800000,                                                  // inf                                         /// 00b94
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b98
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b9c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ba0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ba4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ba8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x33333333,                                                  // 4 random values                             /// 00bb8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bbc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bc0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bc8
			 0x55555555,                                                  // 4 random values                             /// 00bcc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bd8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bdc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00be0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00be4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00be8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bf0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bf4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bf8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00bfc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c00
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c04
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x7f800000,                                                  // inf                                         /// 00c0c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c14
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c18
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c1c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c28
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c2c
			 0x3f028f5c,                                                  // 0.51                                        /// 00c30
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c34
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c38
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c40
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c44
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c48
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c58
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c5c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c60
			 0x80000000,                                                  // -zero                                       /// 00c64
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c68
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c6c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c74
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c7c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c80
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c84
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c8c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c90
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c94
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ca0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ca4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ca8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cb4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cb8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cbc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cc4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ccc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cd0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cd4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cd8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cdc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ce0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ce4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ce8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cf0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cf4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cf8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cfc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d00
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d04
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d0c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d10
			 0x80000000,                                                  // -zero                                       /// 00d14
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d18
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d20
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d24
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d28
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d2c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d30
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d38
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d40
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d44
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d48
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d4c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d50
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d54
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d58
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d60
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d64
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d68
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d70
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d78
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d7c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d80
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d84
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d88
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d8c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d98
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d9c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00da0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00da4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00da8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00dac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00db0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00db4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00db8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00dbc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00dc0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00dc4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00dc8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00dcc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00dd4
			 0x3f028f5c,                                                  // 0.51                                        /// 00dd8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ddc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00de8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00dec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00df0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00df4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00df8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00dfc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e00
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e08
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e0c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e10
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e14
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e1c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e24
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e28
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e2c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e38
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e3c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e40
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e48
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e4c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e50
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e54
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e5c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e60
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e64
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e68
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e70
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e74
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0xffc00001,                                                  // -signaling NaN                              /// 00e7c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e80
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e84
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e88
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e94
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e98
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ea0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ea4
			 0x4b000000,                                                  // 8388608.0                                   /// 00ea8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00eac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00eb0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00eb4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00eb8
			 0x00000000,                                                  // zero                                        /// 00ebc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ec0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ec4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ec8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ecc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ed0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ed4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00edc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ee0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ee4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00eec
			 0x80000000,                                                  // -zero                                       /// 00ef0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ef4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ef8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00efc
			 0x33333333,                                                  // 4 random values                             /// 00f00
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f14
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f18
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f1c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f20
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f24
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f28
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f2c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f30
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f34
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f38
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f3c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f40
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f44
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f48
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f4c
			 0x55555555,                                                  // 4 random values                             /// 00f50
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f58
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f5c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f64
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f68
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f6c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f74
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f78
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f7c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f80
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f84
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f88
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f8c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f90
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f9c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fac
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fb0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fb4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fbc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fc0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fc4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fc8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fcc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fd0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fd4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fdc
			 0xcb000000,                                                  // -8388608.0                                  /// 00fe0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fe4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fe8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ff4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ff8
			 0x7f800000 // infinity                                      // SP +ve numbers                              /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00000
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00008
			 0xbf028f5c,                                                  // -0.51                                       /// 0000c
			 0xbf028f5c,                                                  // -0.51                                       /// 00010
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x80011111,                                                  // -9.7958E-41                                 /// 00018
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0001c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00020
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00024
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00028
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0002c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00030
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00034
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00038
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0003c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00040
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0004c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00050
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00058
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0005c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00060
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00068
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0006c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00070
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00078
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00080
			 0x33333333,                                                  // 4 random values                             /// 00084
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00088
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0008c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00090
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00094
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00098
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0009c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000a4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000a8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000b8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000c4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000d4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000dc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000e0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000f0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000f4
			 0x80000000,                                                  // -zero                                       /// 000f8
			 0xbf028f5c,                                                  // -0.51                                       /// 000fc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00104
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0010c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00110
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00114
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00118
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00120
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00124
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00128
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00138
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0013c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00140
			 0xffc00001,                                                  // -signaling NaN                              /// 00144
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00148
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00150
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00154
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00158
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00160
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00168
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0016c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00170
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00174
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00178
			 0x3f028f5c,                                                  // 0.51                                        /// 0017c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0xcb000000,                                                  // -8388608.0                                  /// 00184
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00188
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0xffc00001,                                                  // -signaling NaN                              /// 00190
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00194
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00198
			 0xcb000000,                                                  // -8388608.0                                  /// 0019c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001a0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001a4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001ac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001b8
			 0x3f028f5c,                                                  // 0.51                                        /// 001bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 001c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001c8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001cc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001d4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x0e000003,                                                  // Trailing 1s:                                /// 001dc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001e0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001e4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x55555555,                                                  // 4 random values                             /// 001ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001f0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001f4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001fc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00200
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00204
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00208
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0020c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00210
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00214
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00218
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0021c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00220
			 0xffc00001,                                                  // -signaling NaN                              /// 00224
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00228
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00238
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0023c
			 0xffc00001,                                                  // -signaling NaN                              /// 00240
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00244
			 0x00000000,                                                  // zero                                        /// 00248
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0024c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00254
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00258
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0025c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x0e000001,                                                  // Trailing 1s:                                /// 00264
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00268
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00270
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00274
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0027c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00284
			 0x3f028f5c,                                                  // 0.51                                        /// 00288
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0028c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00290
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00294
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00298
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0029c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002a8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002b0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002b4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002b8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002bc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002c0
			 0xff800000,                                                  // -inf                                        /// 002c4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002c8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002cc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002d8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002e0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002e4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002f4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002fc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00300
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00304
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00308
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0030c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00314
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00318
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0031c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00320
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00324
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00330
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00334
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00338
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00344
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00350
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0035c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00360
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0xffc00001,                                                  // -signaling NaN                              /// 00368
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0036c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00370
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00378
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0037c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00380
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00384
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00388
			 0x55555555,                                                  // 4 random values                             /// 0038c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00390
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0039c
			 0x0c600000,                                                  // Leading 1s:                                 /// 003a0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003a4
			 0x7fc00001,                                                  // signaling NaN                               /// 003a8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003b0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003b8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003c0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003c8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003cc
			 0x0c780000,                                                  // Leading 1s:                                 /// 003d0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003d4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003d8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003dc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003e8
			 0x0c780000,                                                  // Leading 1s:                                 /// 003ec
			 0x0c700000,                                                  // Leading 1s:                                 /// 003f0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003f4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003f8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00400
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0040c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00410
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00414
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0041c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00424
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00428
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00430
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00438
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00440
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00444
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00448
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00454
			 0x0e000001,                                                  // Trailing 1s:                                /// 00458
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0045c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00460
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00464
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00468
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00470
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00474
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0047c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00480
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00484
			 0x55555555,                                                  // 4 random values                             /// 00488
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0xbf028f5c,                                                  // -0.51                                       /// 00490
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00494
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x00000000,                                                  // zero                                        /// 0049c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004a0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004a8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x80011111,                                                  // -9.7958E-41                                 /// 004b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004b4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004b8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004bc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004c0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004c8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 004d0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004d4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004d8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004dc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004e0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004e4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004e8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004ec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004f0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 004f8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00504
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00508
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0050c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00510
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0051c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00524
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00530
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00534
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00538
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0053c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00540
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x3f028f5c,                                                  // 0.51                                        /// 00548
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0054c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00550
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00554
			 0x80011111,                                                  // -9.7958E-41                                 /// 00558
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0055c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00560
			 0x0c600000,                                                  // Leading 1s:                                 /// 00564
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00568
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0056c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00570
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x00000000,                                                  // zero                                        /// 0057c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00584
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0058c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00590
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00594
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00598
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005a4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005ac
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005b0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005b8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005bc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 005c4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005c8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005cc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005d0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005d4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005d8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005e4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005e8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005f4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x33333333,                                                  // 4 random values                             /// 00604
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0060c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00610
			 0xffc00001,                                                  // -signaling NaN                              /// 00614
			 0x00011111,                                                  // 9.7958E-41                                  /// 00618
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0061c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00620
			 0x80011111,                                                  // -9.7958E-41                                 /// 00624
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00628
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0062c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00630
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00634
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00638
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0063c
			 0xbf028f5c,                                                  // -0.51                                       /// 00640
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00644
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00648
			 0x0c600000,                                                  // Leading 1s:                                 /// 0064c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00650
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00654
			 0x55555555,                                                  // 4 random values                             /// 00658
			 0x80011111,                                                  // -9.7958E-41                                 /// 0065c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00660
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00664
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00668
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00670
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00674
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00678
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0067c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00680
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00684
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00694
			 0x0c400000,                                                  // Leading 1s:                                 /// 00698
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0069c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006a0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006ac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006b8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006c4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006cc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006d8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006e0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006e8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006ec
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006f0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006f4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006fc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00708
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0070c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00718
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0071c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00720
			 0x80000000,                                                  // -zero                                       /// 00724
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0072c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00734
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00740
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00744
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00748
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0074c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00758
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0075c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00760
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00768
			 0x33333333,                                                  // 4 random values                             /// 0076c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00770
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00778
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0077c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00784
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00788
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0078c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00790
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00794
			 0x55555555,                                                  // 4 random values                             /// 00798
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0079c
			 0x7fc00001,                                                  // signaling NaN                               /// 007a0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 007a4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007b0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007b8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007c4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007c8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007cc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007d0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007d8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007e0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007e8
			 0x00000000,                                                  // zero                                        /// 007ec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007f0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007fc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00800
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x0e000007,                                                  // Trailing 1s:                                /// 00808
			 0x33333333,                                                  // 4 random values                             /// 0080c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00814
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0081c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00820
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00824
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00828
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0082c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00830
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00834
			 0x00000000,                                                  // zero                                        /// 00838
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0083c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00840
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00844
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00850
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00858
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0085c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00860
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00864
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00868
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0086c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x33333333,                                                  // 4 random values                             /// 00878
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00880
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00888
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0088c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00890
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00894
			 0x0c600000,                                                  // Leading 1s:                                 /// 00898
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0089c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 008a8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008ac
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008b0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008b4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x00000000,                                                  // zero                                        /// 008bc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008c0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008c4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008c8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008cc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008d8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008dc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008e4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008f0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008f4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008f8
			 0x0c600000,                                                  // Leading 1s:                                 /// 008fc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00908
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0090c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00910
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00914
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00918
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00920
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00924
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00928
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0092c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00930
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00934
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00938
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0093c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00940
			 0x00000000,                                                  // zero                                        /// 00944
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0094c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00950
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00954
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00958
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0095c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00960
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00964
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0096c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00970
			 0x00000000,                                                  // zero                                        /// 00974
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00978
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00984
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00988
			 0x3f028f5c,                                                  // 0.51                                        /// 0098c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00990
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00994
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00998
			 0x4b000000,                                                  // 8388608.0                                   /// 0099c
			 0x7fc00001,                                                  // signaling NaN                               /// 009a0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009a8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009ac
			 0x00011111,                                                  // 9.7958E-41                                  /// 009b0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009b4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009b8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009bc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009c0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009c4
			 0x7f800000,                                                  // inf                                         /// 009c8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009cc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009d0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009d4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009d8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009dc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009e0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009e4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009ec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009f0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009f4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009fc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a00
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a04
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a08
			 0x7fc00001,                                                  // signaling NaN                               /// 00a0c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a10
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a14
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a18
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a1c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a20
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a28
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a2c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a34
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a38
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a3c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a44
			 0x33333333,                                                  // 4 random values                             /// 00a48
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a4c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a50
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a54
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a58
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a5c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a60
			 0x4b000000,                                                  // 8388608.0                                   /// 00a64
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a68
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a6c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a70
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a74
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a78
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a7c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a80
			 0xcb000000,                                                  // -8388608.0                                  /// 00a84
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a88
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a8c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a90
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a94
			 0x00000000,                                                  // zero                                        /// 00a98
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a9c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00aa0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00aa4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00aa8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00aac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ab0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ab8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00abc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ac0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ac4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ac8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00acc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ad0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ad4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00adc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ae0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ae4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ae8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00aec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00af0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0xcb000000,                                                  // -8388608.0                                  /// 00af8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00afc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b00
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b04
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x7fc00001,                                                  // signaling NaN                               /// 00b10
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b14
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b18
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b20
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b24
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b30
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b34
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b38
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b3c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b40
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b44
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b48
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b4c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b50
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b54
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b58
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b60
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b64
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b74
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b78
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b7c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b80
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b88
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b8c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b90
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b94
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b9c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ba0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ba4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0xffc00001,                                                  // -signaling NaN                              /// 00bac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bb0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bb4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bb8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bbc
			 0x7f800000,                                                  // inf                                         /// 00bc0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bc4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bd0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bd4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bdc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00be0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00be4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00be8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bf4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bf8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bfc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c04
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c08
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c10
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c14
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c18
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c1c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c20
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c24
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c28
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c2c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c30
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c34
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c38
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c3c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c44
			 0xcb000000,                                                  // -8388608.0                                  /// 00c48
			 0x33333333,                                                  // 4 random values                             /// 00c4c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c50
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c54
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c58
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c5c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c64
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c6c
			 0x7f800000,                                                  // inf                                         /// 00c70
			 0xbf028f5c,                                                  // -0.51                                       /// 00c74
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c78
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c84
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c88
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c8c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c94
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c9c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ca0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ca4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ca8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cb0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cb8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cc0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cc4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00cc8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ccc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cd0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00cd4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ce0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ce4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ce8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cf0
			 0x00000000,                                                  // zero                                        /// 00cf4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cfc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d00
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d04
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d0c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d10
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d14
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d18
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d1c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d20
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d24
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d28
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d34
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d38
			 0xffc00001,                                                  // -signaling NaN                              /// 00d3c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d40
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d44
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d48
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d4c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d50
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x7fc00001,                                                  // signaling NaN                               /// 00d58
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d5c
			 0x80000000,                                                  // -zero                                       /// 00d60
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d68
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d6c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d70
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d74
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d78
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0x33333333,                                                  // 4 random values                             /// 00d80
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d88
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d8c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d94
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d98
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d9c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00da4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00da8
			 0x7f800000,                                                  // inf                                         /// 00dac
			 0x0c700000,                                                  // Leading 1s:                                 /// 00db0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00db4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00db8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00dbc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00dc0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00dc4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dc8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00dcc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00dd0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dd4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00dd8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00de0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00de4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00de8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00df4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00df8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e04
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e0c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e10
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e18
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e1c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e20
			 0x4b000000,                                                  // 8388608.0                                   /// 00e24
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e28
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e30
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e34
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e38
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e3c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e40
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e44
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e4c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e50
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e54
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e58
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e5c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e60
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e64
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e68
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e6c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e70
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e78
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e7c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e80
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e84
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e88
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e8c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e90
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e94
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e98
			 0xcb000000,                                                  // -8388608.0                                  /// 00e9c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ea8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00eb0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00eb4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00eb8
			 0x4b000000,                                                  // 8388608.0                                   /// 00ebc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ec0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ec4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ec8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ed0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ed4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ed8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ee4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00eec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ef0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ef4
			 0x00000000,                                                  // zero                                        /// 00ef8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00efc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f00
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f04
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f08
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f0c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f10
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f14
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f18
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f1c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f20
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f28
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f2c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f30
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f38
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f3c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f44
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f48
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f4c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f50
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f5c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f60
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f64
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f68
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f78
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f7c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f84
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f88
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f8c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f94
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f98
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f9c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fa0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fa4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fac
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fb0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fb4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fb8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fc0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fc4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fc8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fcc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fd0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fd4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fdc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fe0
			 0xff800000,                                                  // -inf                                        /// 00fe4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fe8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ff0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ff4
			 0xbf028f5c,                                                  // -0.51                                       /// 00ff8
			 0x80800003 // min norm + 3ulp                               // subnormals -ve                              /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00000
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0000c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00010
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00014
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0001c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00024
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0002c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00030
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00034
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00038
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0003c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00040
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00044
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0004c
			 0x3f028f5c,                                                  // 0.51                                        /// 00050
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00054
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00058
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0005c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00064
			 0x0e000003,                                                  // Trailing 1s:                                /// 00068
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0006c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00070
			 0x80011111,                                                  // -9.7958E-41                                 /// 00074
			 0xff800000,                                                  // -inf                                        /// 00078
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00084
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0008c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00090
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00094
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00098
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0009c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000a4
			 0x3f028f5c,                                                  // 0.51                                        /// 000a8
			 0x0c600000,                                                  // Leading 1s:                                 /// 000ac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000b0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000b4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000b8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000bc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000c0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000c4
			 0x00011111,                                                  // 9.7958E-41                                  /// 000c8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000d4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000d8
			 0x7fc00001,                                                  // signaling NaN                               /// 000dc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000e0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000e4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000ec
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000f8
			 0x3f028f5c,                                                  // 0.51                                        /// 000fc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00100
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00104
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00108
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0010c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00110
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x4b000000,                                                  // 8388608.0                                   /// 00118
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0011c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00120
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00128
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0012c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00130
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0013c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00144
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00150
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00154
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00158
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00160
			 0x7f800000,                                                  // inf                                         /// 00164
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00168
			 0x33333333,                                                  // 4 random values                             /// 0016c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00170
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00178
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00184
			 0x4b000000,                                                  // 8388608.0                                   /// 00188
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0018c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00194
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0xffc00001,                                                  // -signaling NaN                              /// 0019c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001a0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001a4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001a8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001b0
			 0x0c400000,                                                  // Leading 1s:                                 /// 001b4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001b8
			 0x00011111,                                                  // 9.7958E-41                                  /// 001bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001c0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001c4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001c8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001cc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001d0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 001d4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001d8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001e0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001e4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001e8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 001f4
			 0x00000000,                                                  // zero                                        /// 001f8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00204
			 0x00011111,                                                  // 9.7958E-41                                  /// 00208
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0020c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00210
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00214
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0021c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00220
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00228
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00230
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00234
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0023c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00248
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0024c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00250
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00254
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00258
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0025c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00260
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00264
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0026c
			 0x4b000000,                                                  // 8388608.0                                   /// 00270
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0027c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00280
			 0x55555555,                                                  // 4 random values                             /// 00284
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00288
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0028c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00290
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00294
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00298
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0029c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002a4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002ac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002bc
			 0x0c600000,                                                  // Leading 1s:                                 /// 002c0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002c8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002cc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002d4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002d8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002e0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002e4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002e8
			 0x7f800000,                                                  // inf                                         /// 002ec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002f0
			 0x0c600000,                                                  // Leading 1s:                                 /// 002f4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002f8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00300
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00304
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00308
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0030c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00310
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00314
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00318
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00320
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00328
			 0x80011111,                                                  // -9.7958E-41                                 /// 0032c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00330
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00334
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00338
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0033c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00340
			 0x80000000,                                                  // -zero                                       /// 00344
			 0x00000000,                                                  // zero                                        /// 00348
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00350
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0xff800000,                                                  // -inf                                        /// 00358
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0035c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00364
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00368
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0036c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00370
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00374
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00378
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0037c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00380
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00384
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00388
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0038c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00390
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00394
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003a4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003a8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003b0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003b4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003b8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003bc
			 0x80011111,                                                  // -9.7958E-41                                 /// 003c0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003c4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003cc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003d0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003d4
			 0x3f028f5c,                                                  // 0.51                                        /// 003d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003dc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003e4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003f0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003f4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00400
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00404
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00408
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0040c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00410
			 0x00000000,                                                  // zero                                        /// 00414
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00418
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0041c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00428
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0042c
			 0x55555555,                                                  // 4 random values                             /// 00430
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00434
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00438
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0043c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00440
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00444
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00448
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0044c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00450
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00454
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00458
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00460
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00464
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00468
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0046c
			 0xff800000,                                                  // -inf                                        /// 00470
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00478
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00480
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00484
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00488
			 0x80000000,                                                  // -zero                                       /// 0048c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00490
			 0x7f800000,                                                  // inf                                         /// 00494
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00498
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0049c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004a4
			 0xbf028f5c,                                                  // -0.51                                       /// 004a8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004b0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004b4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004b8
			 0x7f800000,                                                  // inf                                         /// 004bc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004c0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004c4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004cc
			 0x55555555,                                                  // 4 random values                             /// 004d0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004d4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004d8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004e8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004ec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004f0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004fc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00504
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00508
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0050c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00510
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00514
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00518
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0051c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00520
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00528
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00530
			 0x80011111,                                                  // -9.7958E-41                                 /// 00534
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00538
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00540
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00544
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0054c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00550
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00558
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0055c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00560
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00564
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00568
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0056c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00570
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00574
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00578
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0057c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00580
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00584
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00588
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0058c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00594
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0059c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005a0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005b0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005b4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005b8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005c0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 005c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005cc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005d0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005d4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005d8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005dc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005e0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005e4
			 0x4b000000,                                                  // 8388608.0                                   /// 005e8
			 0xcb000000,                                                  // -8388608.0                                  /// 005ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005f0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005f4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005f8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005fc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00604
			 0x0c400000,                                                  // Leading 1s:                                 /// 00608
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0060c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00614
			 0x0c700000,                                                  // Leading 1s:                                 /// 00618
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0061c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00620
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00628
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00630
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00634
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00638
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00644
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00648
			 0x4b000000,                                                  // 8388608.0                                   /// 0064c
			 0x80000000,                                                  // -zero                                       /// 00650
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00654
			 0xff800000,                                                  // -inf                                        /// 00658
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0065c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00660
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00668
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0066c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00670
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00674
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00678
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0067c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00680
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00684
			 0x80011111,                                                  // -9.7958E-41                                 /// 00688
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0068c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00690
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00698
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006a0
			 0x33333333,                                                  // 4 random values                             /// 006a4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006a8
			 0xff800000,                                                  // -inf                                        /// 006ac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x55555555,                                                  // 4 random values                             /// 006b4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006c4
			 0xff800000,                                                  // -inf                                        /// 006c8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006cc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006d0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006d4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006dc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006e0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006e8
			 0xbf028f5c,                                                  // -0.51                                       /// 006ec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x0c700000,                                                  // Leading 1s:                                 /// 006f4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006fc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00704
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00708
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00710
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00714
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00718
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00720
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00724
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x3f028f5c,                                                  // 0.51                                        /// 0072c
			 0x3f028f5c,                                                  // 0.51                                        /// 00730
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00734
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00738
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0073c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00744
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00748
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0074c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00750
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00754
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00758
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0075c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00760
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00764
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00768
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00774
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00778
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0077c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00780
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00784
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00788
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0078c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00790
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00794
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00798
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0079c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007a4
			 0x00000000,                                                  // zero                                        /// 007a8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007ac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007b0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007b8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007bc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007c4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007cc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007d4
			 0x00011111,                                                  // 9.7958E-41                                  /// 007d8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007dc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007e0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007e8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007f0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007f8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0xcb000000,                                                  // -8388608.0                                  /// 00800
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00808
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00810
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0081c
			 0x80000000,                                                  // -zero                                       /// 00820
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0082c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00830
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00838
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0083c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00840
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00844
			 0x0e000001,                                                  // Trailing 1s:                                /// 00848
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0084c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00850
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00858
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0085c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00860
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0xff800000,                                                  // -inf                                        /// 00868
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0086c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00870
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00874
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00878
			 0xffc00001,                                                  // -signaling NaN                              /// 0087c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00880
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00884
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00888
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0088c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00894
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00898
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008a0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008a4
			 0x3f028f5c,                                                  // 0.51                                        /// 008a8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008ac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008b0
			 0x55555555,                                                  // 4 random values                             /// 008b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008bc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008c8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008cc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008d0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008dc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008e8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008f0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008f4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008f8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008fc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00900
			 0x0c700000,                                                  // Leading 1s:                                 /// 00904
			 0x0e000007,                                                  // Trailing 1s:                                /// 00908
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0090c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00918
			 0x55555555,                                                  // 4 random values                             /// 0091c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00920
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00924
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00928
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0092c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00930
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00934
			 0x33333333,                                                  // 4 random values                             /// 00938
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0093c
			 0x00000000,                                                  // zero                                        /// 00940
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00944
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0094c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00954
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00958
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0095c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00960
			 0x7f800000,                                                  // inf                                         /// 00964
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x55555555,                                                  // 4 random values                             /// 0096c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00970
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00974
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00978
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0097c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x0c600000,                                                  // Leading 1s:                                 /// 0098c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00990
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x7fc00001,                                                  // signaling NaN                               /// 00998
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0099c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009a0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009a8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009b0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009b8
			 0xbf028f5c,                                                  // -0.51                                       /// 009bc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009c0
			 0x80000000,                                                  // -zero                                       /// 009c4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009cc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009d0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009d4
			 0x3f028f5c,                                                  // 0.51                                        /// 009d8
			 0x0e000003,                                                  // Trailing 1s:                                /// 009dc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009e4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009ec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009f0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009f4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a00
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a04
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a08
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x80000000,                                                  // -zero                                       /// 00a10
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a14
			 0xffc00001,                                                  // -signaling NaN                              /// 00a18
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a1c
			 0x4b000000,                                                  // 8388608.0                                   /// 00a20
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a24
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a30
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a34
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a38
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a3c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a40
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a44
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a48
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a4c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a50
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a54
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a58
			 0xbf028f5c,                                                  // -0.51                                       /// 00a5c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a60
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a64
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a6c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a74
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a78
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a7c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a80
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a84
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a88
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a90
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a94
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a98
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x80000000,                                                  // -zero                                       /// 00aa0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00aa4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00aa8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00aac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ab0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ab4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00abc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ac0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ac4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ac8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00acc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ad4
			 0xbf028f5c,                                                  // -0.51                                       /// 00ad8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00adc
			 0x00000000,                                                  // zero                                        /// 00ae0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ae8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00aec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00af0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00af4
			 0x7f800000,                                                  // inf                                         /// 00af8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00afc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b00
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b0c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b1c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b20
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b24
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b28
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b2c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b30
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b34
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b38
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b3c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b40
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b44
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b4c
			 0x33333333,                                                  // 4 random values                             /// 00b50
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b5c
			 0x33333333,                                                  // 4 random values                             /// 00b60
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b64
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b68
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0xff800000,                                                  // -inf                                        /// 00b74
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x00000000,                                                  // zero                                        /// 00b7c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b80
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b84
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b88
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b90
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b94
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ba0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ba4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ba8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00bb4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bb8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bbc
			 0x7f800000,                                                  // inf                                         /// 00bc0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bc4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bc8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bcc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bd0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bd4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bd8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bdc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00be0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00be4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bf0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bf4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x7f800000,                                                  // inf                                         /// 00bfc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c08
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c0c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c10
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0xff800000,                                                  // -inf                                        /// 00c18
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c1c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c20
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c28
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c30
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c38
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c3c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c40
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c44
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c48
			 0x3f028f5c,                                                  // 0.51                                        /// 00c4c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c50
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c54
			 0x00000000,                                                  // zero                                        /// 00c58
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c60
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c64
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c6c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c74
			 0xbf028f5c,                                                  // -0.51                                       /// 00c78
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c7c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c88
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c90
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c98
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ca0
			 0xff800000,                                                  // -inf                                        /// 00ca4
			 0x7f800000,                                                  // inf                                         /// 00ca8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cbc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cc4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cc8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ccc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cd0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ce0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ce4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ce8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cf0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cf8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00cfc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d00
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d0c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d10
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d14
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d18
			 0x00000000,                                                  // zero                                        /// 00d1c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d20
			 0x33333333,                                                  // 4 random values                             /// 00d24
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d28
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d2c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d30
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d34
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d38
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d3c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d44
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d48
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d50
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d58
			 0xffc00001,                                                  // -signaling NaN                              /// 00d5c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d60
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d64
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d68
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d6c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d70
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d78
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d7c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d80
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d84
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d88
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d8c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d90
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d94
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d98
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d9c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00da0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00da4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00da8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00db0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00db8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00dbc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00dc0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dc4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00dc8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00dcc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00dd0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00dd4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00dd8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ddc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00de0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00de4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00de8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00df0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00df4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00df8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00dfc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e04
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e08
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e0c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e14
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e18
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e20
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e28
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e34
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e38
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e3c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e4c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e50
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e54
			 0x4b000000,                                                  // 8388608.0                                   /// 00e58
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e5c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e60
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e64
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e68
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e6c
			 0x80000000,                                                  // -zero                                       /// 00e70
			 0x3f028f5c,                                                  // 0.51                                        /// 00e74
			 0x4b000000,                                                  // 8388608.0                                   /// 00e78
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e7c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e80
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e88
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e8c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e90
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e94
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e98
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e9c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ea0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ea8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00eb8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ebc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ec0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ec8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ecc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ed0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ed4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ed8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00edc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ee0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ee8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00eec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ef0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ef4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00efc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f04
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f08
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f0c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f10
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f14
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f18
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f1c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f20
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f24
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f28
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f30
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f34
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f38
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f3c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f4c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f50
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f54
			 0xbf028f5c,                                                  // -0.51                                       /// 00f58
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f5c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f60
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f64
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f68
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f74
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f7c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f80
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f84
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f88
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f90
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f98
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f9c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fa4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fa8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fb0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fb4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fb8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fbc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0xffc00001,                                                  // -signaling NaN                              /// 00fc8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fcc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fd4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fd8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fdc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fe0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fe4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fe8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ff4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ff8
			 0x00000002 // min subnorm + 1 ulp                           // SP +ve numbers                              /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00000
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00008
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00010
			 0x7fc00001,                                                  // signaling NaN                               /// 00014
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0xbf028f5c,                                                  // -0.51                                       /// 0001c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00028
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00034
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0003c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0004c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00058
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0005c
			 0xffc00001,                                                  // -signaling NaN                              /// 00060
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00064
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00068
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0006c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00070
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00074
			 0x0c600000,                                                  // Leading 1s:                                 /// 00078
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00080
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00084
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0008c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00090
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0009c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000a0
			 0x80011111,                                                  // -9.7958E-41                                 /// 000a4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000a8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000b8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000bc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000c0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000c4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000c8
			 0x80011111,                                                  // -9.7958E-41                                 /// 000cc
			 0x0c780000,                                                  // Leading 1s:                                 /// 000d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000d4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000d8
			 0x3f028f5c,                                                  // 0.51                                        /// 000dc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000e0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000e4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000ec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000f8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000fc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00100
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00108
			 0x00011111,                                                  // 9.7958E-41                                  /// 0010c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00114
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0011c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00124
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0012c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00130
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00134
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00138
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00144
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00148
			 0x80000000,                                                  // -zero                                       /// 0014c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00154
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00158
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0015c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00160
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0016c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00170
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00174
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0017c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00180
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00188
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0018c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00194
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00198
			 0xffc00001,                                                  // -signaling NaN                              /// 0019c
			 0xff800000,                                                  // -inf                                        /// 001a0
			 0x0c600000,                                                  // Leading 1s:                                 /// 001a4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001a8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001ac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 001b8
			 0x7fc00001,                                                  // signaling NaN                               /// 001bc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001c4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001cc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001d8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001e0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001e4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001f0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001f4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001f8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00200
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00204
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00214
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00218
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0021c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00224
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00228
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0022c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00234
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00238
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0023c
			 0x7fc00001,                                                  // signaling NaN                               /// 00240
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00258
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00264
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00268
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0026c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00270
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00274
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0027c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00280
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00284
			 0x4b000000,                                                  // 8388608.0                                   /// 00288
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0028c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00290
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00294
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00298
			 0x0e000003,                                                  // Trailing 1s:                                /// 0029c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002a0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002a4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002ac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002b0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002b8
			 0xffc00001,                                                  // -signaling NaN                              /// 002bc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002c0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002c8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0xcb000000,                                                  // -8388608.0                                  /// 002d4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002e4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x0c400000,                                                  // Leading 1s:                                 /// 002ec
			 0x7f800000,                                                  // inf                                         /// 002f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x0c600000,                                                  // Leading 1s:                                 /// 002f8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002fc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00300
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00308
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0030c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00310
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00314
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00318
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0031c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00320
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00324
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00328
			 0x0c600000,                                                  // Leading 1s:                                 /// 0032c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00330
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00334
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00338
			 0xffc00001,                                                  // -signaling NaN                              /// 0033c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00340
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00344
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00348
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00350
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x0c600000,                                                  // Leading 1s:                                 /// 00358
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00360
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0036c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00370
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00374
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00378
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0037c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00380
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00384
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00390
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00394
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00398
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0039c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003a4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003a8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003ac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003b8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003c4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003c8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003cc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003d0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 003d8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003e8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003ec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003f0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003f8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003fc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00404
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00408
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00420
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00428
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0042c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00430
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00434
			 0x0c780000,                                                  // Leading 1s:                                 /// 00438
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0043c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00440
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00444
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00448
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0044c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00450
			 0x0e000001,                                                  // Trailing 1s:                                /// 00454
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0045c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00460
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00464
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00468
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0046c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00470
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00474
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00478
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0047c
			 0x33333333,                                                  // 4 random values                             /// 00480
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00484
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00488
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0048c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00490
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00494
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00498
			 0x7fc00001,                                                  // signaling NaN                               /// 0049c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004a0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004a4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004ac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004b0
			 0x0e000007,                                                  // Trailing 1s:                                /// 004b4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004b8
			 0x0c700000,                                                  // Leading 1s:                                 /// 004bc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004c0
			 0x7fc00001,                                                  // signaling NaN                               /// 004c4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004c8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 004cc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004d8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004dc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004e0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004e4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00504
			 0xffc00001,                                                  // -signaling NaN                              /// 00508
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0050c
			 0x80000000,                                                  // -zero                                       /// 00510
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00514
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0051c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00520
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00524
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00528
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0052c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00530
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00534
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0053c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00540
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00544
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00548
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0054c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00554
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00558
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0055c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00568
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0056c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00570
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0xbf028f5c,                                                  // -0.51                                       /// 00578
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0057c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00584
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0058c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00590
			 0x0e000001,                                                  // Trailing 1s:                                /// 00594
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00598
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005a0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005a8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005ac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005b4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005c4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005c8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x0c600000,                                                  // Leading 1s:                                 /// 005d0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005d4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005d8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005ec
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x0c780000,                                                  // Leading 1s:                                 /// 005f4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005f8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 005fc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00600
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00604
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00608
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0060c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00610
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00614
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0061c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00624
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00628
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00630
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00634
			 0x80011111,                                                  // -9.7958E-41                                 /// 00638
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0063c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00640
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00648
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0064c
			 0x55555555,                                                  // 4 random values                             /// 00650
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00654
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00658
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0065c
			 0x00000000,                                                  // zero                                        /// 00660
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00664
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00668
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x7fc00001,                                                  // signaling NaN                               /// 00670
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00678
			 0x7f800000,                                                  // inf                                         /// 0067c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00680
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x00000000,                                                  // zero                                        /// 00690
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00694
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0069c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 006a4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006a8
			 0x0e000003,                                                  // Trailing 1s:                                /// 006ac
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006b4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006b8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006bc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006c0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006c4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006c8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006cc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006d0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006d4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006d8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006dc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006e8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006fc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00700
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x55555555,                                                  // 4 random values                             /// 00708
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00710
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00714
			 0x0c600000,                                                  // Leading 1s:                                 /// 00718
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0071c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x0e000007,                                                  // Trailing 1s:                                /// 00724
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00728
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0072c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00730
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00734
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00738
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0073c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00740
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0074c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00750
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00754
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00758
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00760
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00764
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00768
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0076c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00770
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00774
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00778
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0077c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00780
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00784
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00788
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0078c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00790
			 0x0e000001,                                                  // Trailing 1s:                                /// 00794
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007a0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007b0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007b4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007b8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007c0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007c4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007cc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007d4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007e0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007e4
			 0xff800000,                                                  // -inf                                        /// 007e8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007ec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007fc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00800
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00808
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0080c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00814
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00818
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0081c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00820
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00824
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00828
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00830
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00834
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00840
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00844
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00848
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0084c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00850
			 0xbf028f5c,                                                  // -0.51                                       /// 00854
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x4b000000,                                                  // 8388608.0                                   /// 00860
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00868
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00870
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00874
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00878
			 0x0e000003,                                                  // Trailing 1s:                                /// 0087c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00880
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00884
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00888
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0088c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00890
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00894
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0089c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008a4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008a8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 008ac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008b4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008bc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008c0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008c8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008d0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008d4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008d8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008e0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008e4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008ec
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008f0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x4b000000,                                                  // 8388608.0                                   /// 008fc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00900
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00908
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00914
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00918
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x7f800000,                                                  // inf                                         /// 00920
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00928
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0092c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00934
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00938
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0093c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00940
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00948
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0094c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00954
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x0c780000,                                                  // Leading 1s:                                 /// 0095c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00968
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0096c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00970
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00974
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00978
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0097c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00980
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00984
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00988
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0xcb000000,                                                  // -8388608.0                                  /// 00990
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00994
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00998
			 0x0c700000,                                                  // Leading 1s:                                 /// 0099c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009b0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009b4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009b8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009bc
			 0xcb000000,                                                  // -8388608.0                                  /// 009c0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009c4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009c8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009cc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009d4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x7f800000,                                                  // inf                                         /// 009dc
			 0x00011111,                                                  // 9.7958E-41                                  /// 009e0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009e8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009ec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009f8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009fc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a00
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a04
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a0c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a10
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a1c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a20
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a24
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a28
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a2c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a34
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a38
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a3c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a44
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a48
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a4c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a50
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a54
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a5c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a64
			 0x00000000,                                                  // zero                                        /// 00a68
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a6c
			 0xbf028f5c,                                                  // -0.51                                       /// 00a70
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a74
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a78
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a7c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a80
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a84
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a88
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a8c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a90
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a98
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a9c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00aa0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00aa4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00aa8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00aac
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ab0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ab8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ac0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ac4
			 0x3f028f5c,                                                  // 0.51                                        /// 00ac8
			 0x3f028f5c,                                                  // 0.51                                        /// 00acc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ad0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ad4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00adc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ae0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ae4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00aec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00af4
			 0xcb000000,                                                  // -8388608.0                                  /// 00af8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00afc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b00
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b04
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x00000000,                                                  // zero                                        /// 00b10
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b14
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b18
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b1c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b20
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b24
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b28
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b2c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b30
			 0x7fc00001,                                                  // signaling NaN                               /// 00b34
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b38
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b3c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b48
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b50
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b54
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b58
			 0x33333333,                                                  // 4 random values                             /// 00b5c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b6c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b70
			 0x7fc00001,                                                  // signaling NaN                               /// 00b74
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b7c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b80
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b88
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b90
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b94
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b98
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b9c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ba0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ba4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ba8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bb4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bb8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bbc
			 0x7fc00001,                                                  // signaling NaN                               /// 00bc0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bc4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x55555555,                                                  // 4 random values                             /// 00bd0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bd4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bd8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bdc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00be4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00be8
			 0xbf028f5c,                                                  // -0.51                                       /// 00bec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bf0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bf4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bfc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0xcb000000,                                                  // -8388608.0                                  /// 00c04
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c08
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c0c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c14
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c18
			 0x7fc00001,                                                  // signaling NaN                               /// 00c1c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c20
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c24
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c28
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c2c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c30
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c34
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c38
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c3c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c40
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c44
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c4c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c50
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c58
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c5c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c64
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c68
			 0x7f800000,                                                  // inf                                         /// 00c6c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c70
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c74
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c78
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c7c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c80
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x7fc00001,                                                  // signaling NaN                               /// 00c88
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c8c
			 0x4b000000,                                                  // 8388608.0                                   /// 00c90
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c94
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ca0
			 0x55555555,                                                  // 4 random values                             /// 00ca4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ca8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cb0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cb4
			 0x55555555,                                                  // 4 random values                             /// 00cb8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cbc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cc4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cc8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ccc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cd0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cd4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cdc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ce4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00cec
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cf4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cf8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cfc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d00
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d08
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d10
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d1c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d20
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0xff800000,                                                  // -inf                                        /// 00d28
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d2c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d30
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d34
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d3c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d40
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d44
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d48
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d50
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d54
			 0x4b000000,                                                  // 8388608.0                                   /// 00d58
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d5c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d64
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d68
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d6c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d70
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d74
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d78
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d7c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d80
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d84
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d88
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d8c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d90
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d94
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d98
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d9c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00da4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00da8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00dac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00db0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00db4
			 0xff800000,                                                  // -inf                                        /// 00db8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00dc8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00dcc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00dd0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00dd8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ddc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00de0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00de4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00de8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00dec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00df8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e08
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e0c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e10
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e14
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e18
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e1c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e20
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e28
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e2c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e34
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e38
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e3c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e44
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e4c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e50
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e54
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e58
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e5c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e60
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e64
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e68
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e6c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e70
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e78
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e7c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e80
			 0x7fc00001,                                                  // signaling NaN                               /// 00e84
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e88
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e8c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e94
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e98
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e9c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ea0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ea4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ea8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00eac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0xcb000000,                                                  // -8388608.0                                  /// 00eb4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ebc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ec0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ec8
			 0xcb000000,                                                  // -8388608.0                                  /// 00ecc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ed0
			 0x00000000,                                                  // zero                                        /// 00ed4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ed8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00edc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ee0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ee4
			 0xff800000,                                                  // -inf                                        /// 00ee8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00eec
			 0xffc00001,                                                  // -signaling NaN                              /// 00ef0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ef4
			 0x00000000,                                                  // zero                                        /// 00ef8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00efc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f00
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f04
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f08
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f0c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f10
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f18
			 0xcb000000,                                                  // -8388608.0                                  /// 00f1c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f20
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f2c
			 0xffc00001,                                                  // -signaling NaN                              /// 00f30
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x00000000,                                                  // zero                                        /// 00f38
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f3c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f40
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f48
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f4c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f50
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f60
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f64
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f68
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f74
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f78
			 0x55555555,                                                  // 4 random values                             /// 00f7c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f80
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f84
			 0x33333333,                                                  // 4 random values                             /// 00f88
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x3f028f5c,                                                  // 0.51                                        /// 00f94
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f9c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fa0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fb4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fb8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fbc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fc0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fd0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fd4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fd8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fdc
			 0x55555555,                                                  // 4 random values                             /// 00fe0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fe8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ff0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x80000001                                                  // -1.4E-45 (-denorm)                          /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00000
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00004
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00008
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0000c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00010
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00014
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00020
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00024
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00028
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0002c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00030
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00034
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00038
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0xcb000000,                                                  // -8388608.0                                  /// 00040
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x0e000003,                                                  // Trailing 1s:                                /// 0004c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00054
			 0x80011111,                                                  // -9.7958E-41                                 /// 00058
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0005c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00060
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00064
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0006c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00070
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00074
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00078
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0007c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00080
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00094
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00098
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000a0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000a4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000a8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000ac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x7f800000,                                                  // inf                                         /// 000b8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000c4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000cc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 000d4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000dc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000ec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000f0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000f8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000fc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00100
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00104
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0010c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00110
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0011c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00124
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00128
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00130
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00134
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00138
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0013c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00144
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00148
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0014c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00150
			 0x4b000000,                                                  // 8388608.0                                   /// 00154
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00158
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00160
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00164
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0016c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00170
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0017c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00180
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00184
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0xbf028f5c,                                                  // -0.51                                       /// 0018c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00198
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0019c
			 0xbf028f5c,                                                  // -0.51                                       /// 001a0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001a4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001a8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0xbf028f5c,                                                  // -0.51                                       /// 001b0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001b4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001b8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001bc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001c8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001d0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001e0
			 0x0c700000,                                                  // Leading 1s:                                 /// 001e4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001e8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x0c400000,                                                  // Leading 1s:                                 /// 001f4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001f8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001fc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00200
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00204
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00208
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0020c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00210
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00214
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0021c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00220
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00224
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00228
			 0x0e000007,                                                  // Trailing 1s:                                /// 0022c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00230
			 0x0e000007,                                                  // Trailing 1s:                                /// 00234
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00238
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00240
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0024c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00250
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00254
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00258
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0025c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00260
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00264
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00268
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0026c
			 0x3f028f5c,                                                  // 0.51                                        /// 00270
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00274
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00280
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00288
			 0xffc00001,                                                  // -signaling NaN                              /// 0028c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00290
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00298
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0029c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002a0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002a4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0xff800000,                                                  // -inf                                        /// 002b0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002b4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002b8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002c0
			 0x0e000003,                                                  // Trailing 1s:                                /// 002c4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002c8
			 0x55555555,                                                  // 4 random values                             /// 002cc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002d4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002e0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002ec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002f4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002f8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002fc
			 0x7f800000,                                                  // inf                                         /// 00300
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00304
			 0x80000000,                                                  // -zero                                       /// 00308
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0030c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00310
			 0x0c780000,                                                  // Leading 1s:                                 /// 00314
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0031c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00320
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0032c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00330
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00334
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00338
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00344
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00348
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0034c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00350
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00358
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0035c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00364
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00368
			 0x0e000001,                                                  // Trailing 1s:                                /// 0036c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00370
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00378
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00380
			 0x00011111,                                                  // 9.7958E-41                                  /// 00384
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00388
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0038c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00390
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x0e000001,                                                  // Trailing 1s:                                /// 00398
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003a0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003a4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003ac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003b0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003b4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003b8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003bc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003c0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003c8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003cc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003d0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003d4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003d8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003dc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003f0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003f4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003fc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00400
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00404
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00408
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x33333333,                                                  // 4 random values                             /// 00410
			 0x7fc00001,                                                  // signaling NaN                               /// 00414
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00418
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0041c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00420
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00424
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00428
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0042c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00430
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00434
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00438
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0043c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00440
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00444
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00448
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x7fc00001,                                                  // signaling NaN                               /// 00450
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00458
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0045c
			 0xffc00001,                                                  // -signaling NaN                              /// 00460
			 0xcb000000,                                                  // -8388608.0                                  /// 00464
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0046c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00470
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00478
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0047c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00484
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00488
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0048c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00490
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00494
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00498
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004a4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004a8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004ac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004b0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004b4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004bc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004c4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004c8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004d8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004dc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004e0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004e4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004e8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004ec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004f0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004f4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004f8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004fc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00500
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00504
			 0x0c780000,                                                  // Leading 1s:                                 /// 00508
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0050c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00510
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00518
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00520
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00528
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0052c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00530
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00538
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0053c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00540
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00544
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00548
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0054c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00550
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00554
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00558
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0055c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00560
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00570
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0057c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00580
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00584
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0058c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00590
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00594
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00598
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005a0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005a4
			 0x3f028f5c,                                                  // 0.51                                        /// 005a8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005ac
			 0x0e000007,                                                  // Trailing 1s:                                /// 005b0
			 0xbf028f5c,                                                  // -0.51                                       /// 005b4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005b8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005bc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005c4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005c8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005d8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005dc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005e4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005e8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x0e000001,                                                  // Trailing 1s:                                /// 005f0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005f4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005f8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00604
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00608
			 0x0c780000,                                                  // Leading 1s:                                 /// 0060c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00610
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00618
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0061c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00620
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00624
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0062c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00630
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00634
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0063c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00640
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00644
			 0x0e000007,                                                  // Trailing 1s:                                /// 00648
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0064c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00650
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00654
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0065c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00660
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00664
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00668
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0066c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00670
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0067c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00680
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00684
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00688
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00694
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00698
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0069c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006a8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006ac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006b0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006b8
			 0x33333333,                                                  // 4 random values                             /// 006bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006c0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 006c8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006d0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006d4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006d8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006dc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006e0
			 0x4b000000,                                                  // 8388608.0                                   /// 006e4
			 0x55555555,                                                  // 4 random values                             /// 006e8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006ec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006f4
			 0xbf028f5c,                                                  // -0.51                                       /// 006f8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00704
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00708
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0070c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00710
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00714
			 0xffc00001,                                                  // -signaling NaN                              /// 00718
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0071c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00724
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00728
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0072c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00730
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00734
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00738
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00740
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00744
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00748
			 0x00000000,                                                  // zero                                        /// 0074c
			 0x7fc00001,                                                  // signaling NaN                               /// 00750
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00754
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00758
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00764
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00768
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00770
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00774
			 0x80000000,                                                  // -zero                                       /// 00778
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0077c
			 0xbf028f5c,                                                  // -0.51                                       /// 00780
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00788
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0078c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00790
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00794
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00798
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0079c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007a0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007a4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007a8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007b0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007b4
			 0x0c600000,                                                  // Leading 1s:                                 /// 007b8
			 0x0e000003,                                                  // Trailing 1s:                                /// 007bc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007c4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007c8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007cc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007d0
			 0x0e000001,                                                  // Trailing 1s:                                /// 007d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007e0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007e4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x0e000003,                                                  // Trailing 1s:                                /// 007ec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007f0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007f4
			 0x33333333,                                                  // 4 random values                             /// 007f8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00800
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00804
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00808
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0080c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00814
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00818
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0081c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00820
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0082c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00830
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00834
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00840
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0084c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00850
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00854
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00858
			 0xcb000000,                                                  // -8388608.0                                  /// 0085c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00860
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00864
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00868
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0xffc00001,                                                  // -signaling NaN                              /// 00870
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0087c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00880
			 0x00000000,                                                  // zero                                        /// 00884
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00888
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0088c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00890
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00894
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00898
			 0x0c600000,                                                  // Leading 1s:                                 /// 0089c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008a4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008a8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008b0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008b4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008b8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008bc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008c4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008d0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008d4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008d8
			 0x33333333,                                                  // 4 random values                             /// 008dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008e4
			 0x00000000,                                                  // zero                                        /// 008e8
			 0x4b000000,                                                  // 8388608.0                                   /// 008ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008f0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008f4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00900
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00904
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00908
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0090c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00910
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00914
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00918
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0091c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00920
			 0x55555555,                                                  // 4 random values                             /// 00924
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00928
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00930
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00934
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00938
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0093c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00940
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00944
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0094c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00950
			 0x0e000001,                                                  // Trailing 1s:                                /// 00954
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00958
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0095c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00960
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00964
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0096c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00970
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00974
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00978
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0097c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00984
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00988
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00990
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00994
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00998
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0099c
			 0x0e000007,                                                  // Trailing 1s:                                /// 009a0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009a4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009a8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009ac
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009b0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009b4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009b8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x3f028f5c,                                                  // 0.51                                        /// 009c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009cc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009d0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009d8
			 0x80011111,                                                  // -9.7958E-41                                 /// 009dc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009e4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009e8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009ec
			 0x7fc00001,                                                  // signaling NaN                               /// 009f0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009f4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009f8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009fc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a04
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a08
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a0c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a10
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a14
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a18
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a1c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a20
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a24
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a28
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a30
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a34
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a38
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a3c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a40
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a44
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a48
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a50
			 0xffc00001,                                                  // -signaling NaN                              /// 00a54
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a58
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a5c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a60
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a64
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a6c
			 0xffc00001,                                                  // -signaling NaN                              /// 00a70
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a74
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a78
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a7c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a80
			 0xff800000,                                                  // -inf                                        /// 00a84
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a88
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a8c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a94
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a98
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a9c
			 0xffc00001,                                                  // -signaling NaN                              /// 00aa0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00aa4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00aa8
			 0xffc00001,                                                  // -signaling NaN                              /// 00aac
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ab4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ab8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00abc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ac0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ac4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ac8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00acc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ad4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ad8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00adc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ae0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ae4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ae8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00aec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00af4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x7f800000,                                                  // inf                                         /// 00afc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b00
			 0xff800000,                                                  // -inf                                        /// 00b04
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b08
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b0c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b18
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b20
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b24
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b28
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b2c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b34
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b38
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b44
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b4c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b50
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b58
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b60
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b68
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b6c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b70
			 0xffc00001,                                                  // -signaling NaN                              /// 00b74
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b78
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b84
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0xcb000000,                                                  // -8388608.0                                  /// 00b8c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b94
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b98
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b9c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ba0
			 0x33333333,                                                  // 4 random values                             /// 00ba4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ba8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bb0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00bb4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00bbc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bc0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bc4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bcc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bd0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bd4
			 0x4b000000,                                                  // 8388608.0                                   /// 00bd8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bdc
			 0xbf028f5c,                                                  // -0.51                                       /// 00be0
			 0x7fc00001,                                                  // signaling NaN                               /// 00be4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00be8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bf0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bf4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bf8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bfc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c04
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c08
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c10
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c14
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c1c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c20
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c24
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c2c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c30
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c34
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c38
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c3c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c40
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c44
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c48
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c4c
			 0x3f028f5c,                                                  // 0.51                                        /// 00c50
			 0x4b000000,                                                  // 8388608.0                                   /// 00c54
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c58
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x80000000,                                                  // -zero                                       /// 00c78
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c80
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c88
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c94
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c98
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c9c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ca4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cb0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cb8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cc0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cc4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00cc8
			 0x55555555,                                                  // 4 random values                             /// 00ccc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cd0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00cd4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cd8
			 0x33333333,                                                  // 4 random values                             /// 00cdc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ce0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ce4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cf4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cf8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cfc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d00
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d04
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d08
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d0c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d14
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d18
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d1c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d28
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d2c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d30
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d34
			 0xcb000000,                                                  // -8388608.0                                  /// 00d38
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d40
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d44
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d48
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d4c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d50
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d54
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d58
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d5c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d64
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d68
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d6c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d70
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d74
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d78
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d7c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d80
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d8c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d90
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d98
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d9c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00da0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00da4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00da8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00db0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00db8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00dbc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dc0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00dc4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00dcc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dd0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00dd4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dd8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ddc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00de0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00de4
			 0x80000000,                                                  // -zero                                       /// 00de8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00df0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00df8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e08
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x7fc00001,                                                  // signaling NaN                               /// 00e10
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e18
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x33333333,                                                  // 4 random values                             /// 00e20
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e24
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e34
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e38
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e40
			 0x4b000000,                                                  // 8388608.0                                   /// 00e44
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e48
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e4c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e50
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e54
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e5c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e64
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e68
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e6c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e80
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e84
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e88
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e8c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e90
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e98
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e9c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ea0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ea4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ea8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00eb4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00eb8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ebc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ec4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ed0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ed4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00edc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ee0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ee4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00eec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ef0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00efc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f00
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f08
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f0c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f10
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f18
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f1c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f20
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f24
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f2c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f34
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f3c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f44
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f48
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f4c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f50
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f54
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f58
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f5c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f60
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f64
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f68
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f6c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f74
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f78
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f7c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f80
			 0x7f800000,                                                  // inf                                         /// 00f84
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f8c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f90
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f94
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f98
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f9c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fa0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fa4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fa8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fc4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fc8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fcc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fd8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fdc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fe0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fe4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fec
			 0x80000000,                                                  // -zero                                       /// 00ff0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ff8
			 0xffc00001                                                  // -signaling NaN                              /// last
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
			 0x000005b4,
			 0x00000728,
			 0x000006b0,
			 0x000000d0,
			 0x000002b0,
			 0x00000448,
			 0x000001a8,
			 0x00000760,

			 /// vpu register f2
			 0x41f80000,
			 0x41f00000,
			 0x40400000,
			 0x41b80000,
			 0x3f800000,
			 0x41c80000,
			 0x41700000,
			 0x42000000,

			 /// vpu register f3
			 0x41a80000,
			 0x42000000,
			 0x40a00000,
			 0x41200000,
			 0x41800000,
			 0x40a00000,
			 0x41f80000,
			 0x41d00000,

			 /// vpu register f4
			 0x41e00000,
			 0x41a80000,
			 0x41a00000,
			 0x41100000,
			 0x41f80000,
			 0x41800000,
			 0x41600000,
			 0x41880000,

			 /// vpu register f5
			 0x41f80000,
			 0x41880000,
			 0x41a80000,
			 0x41980000,
			 0x41d00000,
			 0x41300000,
			 0x42000000,
			 0x41900000,

			 /// vpu register f6
			 0x41c00000,
			 0x41200000,
			 0x41000000,
			 0x41c00000,
			 0x42000000,
			 0x41800000,
			 0x3f800000,
			 0x41d80000,

			 /// vpu register f7
			 0x41c80000,
			 0x41b00000,
			 0x3f800000,
			 0x41e80000,
			 0x41b80000,
			 0x41100000,
			 0x41880000,
			 0x41a00000,

			 /// vpu register f8
			 0x41600000,
			 0x42000000,
			 0x40a00000,
			 0x41200000,
			 0x41b00000,
			 0x41800000,
			 0x40c00000,
			 0x41400000,

			 /// vpu register f9
			 0x41500000,
			 0x40000000,
			 0x41a80000,
			 0x41a00000,
			 0x41a00000,
			 0x40000000,
			 0x40c00000,
			 0x41800000,

			 /// vpu register f10
			 0x41d80000,
			 0x40a00000,
			 0x41d00000,
			 0x41d80000,
			 0x41100000,
			 0x41900000,
			 0x41f00000,
			 0x41e00000,

			 /// vpu register f11
			 0x41900000,
			 0x40e00000,
			 0x41300000,
			 0x41100000,
			 0x41700000,
			 0x41300000,
			 0x41000000,
			 0x40800000,

			 /// vpu register f12
			 0x41980000,
			 0x40800000,
			 0x41a00000,
			 0x41800000,
			 0x41c80000,
			 0x41500000,
			 0x40c00000,
			 0x40800000,

			 /// vpu register f13
			 0x3f800000,
			 0x41e00000,
			 0x41880000,
			 0x40400000,
			 0x41e00000,
			 0x41b00000,
			 0x41700000,
			 0x41b00000,

			 /// vpu register f14
			 0x41880000,
			 0x41f00000,
			 0x41b00000,
			 0x41100000,
			 0x41e00000,
			 0x41880000,
			 0x41980000,
			 0x41200000,

			 /// vpu register f15
			 0x41200000,
			 0x41880000,
			 0x40400000,
			 0x41980000,
			 0x41400000,
			 0x41b00000,
			 0x41300000,
			 0x42000000,

			 /// vpu register f16
			 0x41f80000,
			 0x41500000,
			 0x40e00000,
			 0x41f80000,
			 0x41000000,
			 0x41500000,
			 0x41e80000,
			 0x40c00000,

			 /// vpu register f17
			 0x40c00000,
			 0x40000000,
			 0x41100000,
			 0x3f800000,
			 0x41f80000,
			 0x41800000,
			 0x3f800000,
			 0x41300000,

			 /// vpu register f18
			 0x41600000,
			 0x41880000,
			 0x41d00000,
			 0x41c80000,
			 0x41b00000,
			 0x41e80000,
			 0x41200000,
			 0x40e00000,

			 /// vpu register f19
			 0x40c00000,
			 0x41d80000,
			 0x41980000,
			 0x41700000,
			 0x41e00000,
			 0x41100000,
			 0x41c00000,
			 0x41880000,

			 /// vpu register f20
			 0x41c00000,
			 0x41e80000,
			 0x41300000,
			 0x41a00000,
			 0x41880000,
			 0x41e80000,
			 0x41700000,
			 0x41880000,

			 /// vpu register f21
			 0x41700000,
			 0x41d80000,
			 0x41b00000,
			 0x41a80000,
			 0x41e80000,
			 0x41800000,
			 0x41c00000,
			 0x42000000,

			 /// vpu register f22
			 0x41100000,
			 0x41500000,
			 0x41e00000,
			 0x41c80000,
			 0x40000000,
			 0x41f00000,
			 0x41e80000,
			 0x41400000,

			 /// vpu register f23
			 0x40000000,
			 0x41f00000,
			 0x41f00000,
			 0x41300000,
			 0x41400000,
			 0x41b80000,
			 0x40a00000,
			 0x41200000,

			 /// vpu register f24
			 0x41c00000,
			 0x41880000,
			 0x40800000,
			 0x41700000,
			 0x41e80000,
			 0x40000000,
			 0x41d80000,
			 0x41a80000,

			 /// vpu register f25
			 0x40400000,
			 0x41980000,
			 0x42000000,
			 0x41200000,
			 0x42000000,
			 0x41200000,
			 0x41980000,
			 0x41600000,

			 /// vpu register f26
			 0x40800000,
			 0x41200000,
			 0x40400000,
			 0x41d80000,
			 0x40000000,
			 0x41400000,
			 0x40a00000,
			 0x41500000,

			 /// vpu register f27
			 0x41a80000,
			 0x40000000,
			 0x40800000,
			 0x41200000,
			 0x41d00000,
			 0x41600000,
			 0x41900000,
			 0x41980000,

			 /// vpu register f28
			 0x40400000,
			 0x40e00000,
			 0x41200000,
			 0x41600000,
			 0x40000000,
			 0x41100000,
			 0x41a80000,
			 0x41700000,

			 /// vpu register f29
			 0x41000000,
			 0x41880000,
			 0x41a80000,
			 0x41e00000,
			 0x40000000,
			 0x40e00000,
			 0x41b80000,
			 0x41f80000,

			 /// vpu register f30
			 0x41f80000,
			 0x41200000,
			 0x41c00000,
			 0x41f80000,
			 0x41200000,
			 0x41200000,
			 0x41700000,
			 0x41f80000,

			 /// vpu register f31
			 0x41c80000,
			 0x40e00000,
			 0x41900000,
			 0x41100000,
			 0x41400000,
			 0x41400000,
			 0x41400000,
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
		"fminu.pi f9, f22, f30\n"                             ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f5, f2, f14\n"                              ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f13, f14, f17\n"                            ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f29, f13, f4\n"                             ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f19, f22, f19\n"                            ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f19, f23, f7\n"                             ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f29, f1, f22\n"                             ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f4, f9, f14\n"                              ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f19, f22, f21\n"                            ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f28, f19, f29\n"                            ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f19, f15, f16\n"                            ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f16, f9, f16\n"                             ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f25, f20, f10\n"                            ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f10, f30, f28\n"                            ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f2, f17, f29\n"                             ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f4, f15, f1\n"                              ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f27, f25, f2\n"                             ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f29, f26, f16\n"                            ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f23, f25, f6\n"                             ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f29, f3, f1\n"                              ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f4, f23, f5\n"                              ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f22, f21, f21\n"                            ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f29, f20, f14\n"                            ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f24, f14, f20\n"                            ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f4, f22, f23\n"                             ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f18, f28, f0\n"                             ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f13, f12, f27\n"                            ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f24, f9, f13\n"                             ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f23, f21, f30\n"                            ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f13, f19, f6\n"                             ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f21, f20, f10\n"                            ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f23, f20, f6\n"                             ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f28, f13, f3\n"                             ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f4, f16, f1\n"                              ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f29, f0, f4\n"                              ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f8, f7, f15\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f17, f29, f28\n"                            ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f22, f10, f11\n"                            ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f4, f23, f5\n"                              ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f22, f10, f0\n"                             ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f2, f22, f12\n"                             ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f15, f5, f14\n"                             ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f6, f12, f20\n"                             ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f16, f26, f9\n"                             ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f8, f28, f28\n"                             ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f7, f15, f3\n"                              ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f11, f17, f25\n"                            ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f4, f2, f20\n"                              ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f11, f17, f14\n"                            ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f14, f8, f24\n"                             ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f26, f25, f5\n"                             ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f8, f30, f14\n"                             ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f23, f27, f30\n"                            ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f25, f3, f3\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f29, f0, f15\n"                             ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f6, f8, f3\n"                               ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f25, f25, f11\n"                            ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f4, f3, f9\n"                               ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f15, f1, f9\n"                              ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f19, f5, f22\n"                             ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f21, f6, f18\n"                             ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f12, f14, f27\n"                            ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f29, f9, f27\n"                             ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f30, f11, f24\n"                            ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f27, f14, f12\n"                            ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f16, f0, f18\n"                             ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f8, f12, f9\n"                              ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f5, f29, f18\n"                             ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f8, f14, f15\n"                             ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f23, f14, f15\n"                            ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f24, f16, f2\n"                             ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f24, f6, f6\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f3, f24, f24\n"                             ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f14, f8, f23\n"                             ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f9, f18, f28\n"                             ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f2, f12, f0\n"                              ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f12, f5, f26\n"                             ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f24, f4, f0\n"                              ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f7, f19, f15\n"                             ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f21, f29, f1\n"                             ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f3, f4, f13\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f22, f17, f13\n"                            ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f29, f24, f22\n"                            ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f10, f7, f15\n"                             ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f30, f13, f12\n"                            ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f22, f17, f28\n"                            ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f29, f6, f23\n"                             ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f25, f1, f23\n"                             ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f25, f19, f23\n"                            ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f18, f4, f17\n"                             ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f4, f8, f27\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f27, f5, f24\n"                             ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f29, f27, f6\n"                             ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f2, f23, f2\n"                              ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f4, f16, f4\n"                              ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f27, f15, f1\n"                             ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f26, f28, f12\n"                            ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f13, f4, f21\n"                             ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fminu.pi f4, f16, f11\n"                             ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
