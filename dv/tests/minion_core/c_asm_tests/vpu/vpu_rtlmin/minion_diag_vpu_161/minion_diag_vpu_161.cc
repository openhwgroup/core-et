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
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00008
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0000c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00014
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00018
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0001c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00020
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00024
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x33333333,                                                  // 4 random values                             /// 00030
			 0xcb000000,                                                  // -8388608.0                                  /// 00034
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00038
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0003c
			 0x4b000000,                                                  // 8388608.0                                   /// 00040
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00044
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0004c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00050
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00054
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00060
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00064
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00068
			 0x0e000007,                                                  // Trailing 1s:                                /// 0006c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00070
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00078
			 0x80000000,                                                  // -zero                                       /// 0007c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00084
			 0x0c700000,                                                  // Leading 1s:                                 /// 00088
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00090
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00098
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0009c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000a0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000a4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000a8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000ac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 000b0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000b4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 000b8
			 0x0c700000,                                                  // Leading 1s:                                 /// 000bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000c4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000c8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000cc
			 0x3f028f5c,                                                  // 0.51                                        /// 000d0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000d4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000d8
			 0x80000000,                                                  // -zero                                       /// 000dc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000e4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000e8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000f0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000f4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0xcb000000,                                                  // -8388608.0                                  /// 000fc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00100
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00104
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00108
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00110
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00114
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00118
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0011c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00120
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00124
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00128
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0012c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00130
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00138
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00140
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00144
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00150
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00154
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00158
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00160
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00164
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00168
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0016c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00174
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00178
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0017c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00180
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00184
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00188
			 0x0e000003,                                                  // Trailing 1s:                                /// 0018c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00190
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00194
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001a0
			 0x80011111,                                                  // -9.7958E-41                                 /// 001a4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001a8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001ac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001b0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001b4
			 0xcb000000,                                                  // -8388608.0                                  /// 001b8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x7fc00001,                                                  // signaling NaN                               /// 001c0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001c4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001c8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001cc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x0e000003,                                                  // Trailing 1s:                                /// 001d4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001d8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001dc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001e0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001e4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001e8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001ec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001f0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001f4
			 0x0c400000,                                                  // Leading 1s:                                 /// 001f8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0020c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00210
			 0x00000000,                                                  // zero                                        /// 00214
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0021c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00220
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00224
			 0x0c600000,                                                  // Leading 1s:                                 /// 00228
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0022c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00230
			 0x0e000003,                                                  // Trailing 1s:                                /// 00234
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0023c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00244
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00248
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00250
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00254
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00258
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0025c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00260
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00264
			 0xbf028f5c,                                                  // -0.51                                       /// 00268
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0026c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00278
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0027c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00280
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00284
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00288
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0028c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00290
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00294
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00298
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002a0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002a4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002b0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002b8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002c4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002c8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002d4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002e0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002e4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002e8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002fc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00300
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00308
			 0x0e000003,                                                  // Trailing 1s:                                /// 0030c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00310
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00318
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0031c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00320
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00324
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00330
			 0x0c700000,                                                  // Leading 1s:                                 /// 00334
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00338
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0033c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00340
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00344
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00348
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00354
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00358
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0035c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00360
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00368
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0036c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00370
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0037c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00380
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00384
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00388
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0038c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00394
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00398
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0039c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003a0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003a8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003ac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003b0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003b4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003bc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003c0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003c8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003cc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003d4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003d8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003e0
			 0x00000000,                                                  // zero                                        /// 003e4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003e8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003ec
			 0x0e000007,                                                  // Trailing 1s:                                /// 003f0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003f4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003f8
			 0x7f800000,                                                  // inf                                         /// 003fc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00404
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00408
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0040c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00410
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00414
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00418
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00420
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00424
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00428
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00430
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00434
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0043c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0xbf028f5c,                                                  // -0.51                                       /// 00448
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00450
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00454
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00458
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0xff800000,                                                  // -inf                                        /// 00460
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00468
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0046c
			 0x7fc00001,                                                  // signaling NaN                               /// 00470
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00474
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00478
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0047c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00480
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00484
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0048c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00490
			 0x0c400000,                                                  // Leading 1s:                                 /// 00494
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004a0
			 0x0e000007,                                                  // Trailing 1s:                                /// 004a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004a8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004b0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004b4
			 0xcb000000,                                                  // -8388608.0                                  /// 004b8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004c0
			 0x4b000000,                                                  // 8388608.0                                   /// 004c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004d0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004d4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004d8
			 0xcb000000,                                                  // -8388608.0                                  /// 004dc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004e4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004e8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004ec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x00011111,                                                  // 9.7958E-41                                  /// 004f4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004f8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00508
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0050c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00510
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0051c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00520
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00524
			 0x0c780000,                                                  // Leading 1s:                                 /// 00528
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0052c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00530
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00534
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00538
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00540
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00544
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0054c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00550
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00554
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00558
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0055c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00560
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00564
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00568
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0056c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00574
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00578
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00580
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0058c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00590
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00594
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00598
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0059c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005a0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005a4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x0e000003,                                                  // Trailing 1s:                                /// 005ac
			 0x00000000,                                                  // zero                                        /// 005b0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005b8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005cc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005d0
			 0x55555555,                                                  // 4 random values                             /// 005d4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005d8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005dc
			 0x33333333,                                                  // 4 random values                             /// 005e0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005e4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x80000000,                                                  // -zero                                       /// 005ec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005f0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005f4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005f8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005fc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00600
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00604
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00608
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0060c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00610
			 0x0e000003,                                                  // Trailing 1s:                                /// 00614
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00618
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0061c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00620
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00624
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00628
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0062c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00630
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00634
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00638
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0063c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00640
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00644
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00648
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0064c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00650
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00654
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0x3f028f5c,                                                  // 0.51                                        /// 00660
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00664
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00668
			 0x7fc00001,                                                  // signaling NaN                               /// 0066c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00670
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00678
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0067c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00680
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00684
			 0x55555555,                                                  // 4 random values                             /// 00688
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0068c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00694
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00698
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006a0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006ac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006b0
			 0x55555555,                                                  // 4 random values                             /// 006b4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006b8
			 0x0c400000,                                                  // Leading 1s:                                 /// 006bc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006c4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006c8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006cc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006d0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006d4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 006dc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006e0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006e8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006ec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006f0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006f4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006f8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006fc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0xbf028f5c,                                                  // -0.51                                       /// 00704
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00708
			 0x80011111,                                                  // -9.7958E-41                                 /// 0070c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0071c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00728
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0072c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00730
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00734
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00738
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0073c
			 0x7fc00001,                                                  // signaling NaN                               /// 00740
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00744
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00748
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00750
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00754
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00758
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0075c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00764
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00768
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0076c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00770
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0077c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00784
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0078c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00790
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00794
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00798
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007a4
			 0x00000000,                                                  // zero                                        /// 007a8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007b4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x0e000003,                                                  // Trailing 1s:                                /// 007bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007c0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007cc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007d8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007dc
			 0x00011111,                                                  // 9.7958E-41                                  /// 007e0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007e4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007f0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007f4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007f8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007fc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00800
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00808
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0080c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00810
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00814
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00818
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0081c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00820
			 0x33333333,                                                  // 4 random values                             /// 00824
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00828
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0082c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00830
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00834
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00838
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00840
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00844
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00848
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00850
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00854
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00858
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0085c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00860
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00864
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0086c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00870
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0087c
			 0x7fc00001,                                                  // signaling NaN                               /// 00880
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00884
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00888
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0088c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00890
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x0c700000,                                                  // Leading 1s:                                 /// 00898
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0089c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008a0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008a4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008a8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008ac
			 0x0c780000,                                                  // Leading 1s:                                 /// 008b0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008b4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 008c4
			 0x0c400000,                                                  // Leading 1s:                                 /// 008c8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x0e000001,                                                  // Trailing 1s:                                /// 008d0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008d4
			 0x0c780000,                                                  // Leading 1s:                                 /// 008d8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008e0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008e4
			 0x00000000,                                                  // zero                                        /// 008e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008f0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008f8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008fc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00900
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00908
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0090c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00910
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00914
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0091c
			 0x80000000,                                                  // -zero                                       /// 00920
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00924
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00928
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0092c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00930
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00934
			 0x0e000001,                                                  // Trailing 1s:                                /// 00938
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0093c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00940
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00944
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0094c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00954
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00958
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x0e000003,                                                  // Trailing 1s:                                /// 00964
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00968
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0096c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00970
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00974
			 0x0e000003,                                                  // Trailing 1s:                                /// 00978
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0097c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x0c600000,                                                  // Leading 1s:                                 /// 00988
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00990
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00998
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0099c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009a4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009a8
			 0x0c400000,                                                  // Leading 1s:                                 /// 009ac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009b0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009b4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009b8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009c0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009c4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009cc
			 0xffc00001,                                                  // -signaling NaN                              /// 009d0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009d4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009d8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009dc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009e4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009e8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009f0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009f4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009f8
			 0x0c700000,                                                  // Leading 1s:                                 /// 009fc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a00
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a04
			 0x7f800000,                                                  // inf                                         /// 00a08
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a0c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a14
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a18
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a20
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a28
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a2c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a34
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a38
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a3c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a40
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a48
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a4c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a50
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a54
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a58
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a5c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a60
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a64
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a68
			 0x80000000,                                                  // -zero                                       /// 00a6c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a70
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a74
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a78
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a7c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a80
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a84
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a88
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a8c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a9c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00aa0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00aa4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00aa8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ab4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ab8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ac4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ac8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00acc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ad0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ad8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0xff800000,                                                  // -inf                                        /// 00ae0
			 0xff800000,                                                  // -inf                                        /// 00ae4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ae8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00aec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00af0
			 0xcb000000,                                                  // -8388608.0                                  /// 00af4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00af8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00afc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b00
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b10
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b14
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b18
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b1c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b20
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b24
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b28
			 0x7f800000,                                                  // inf                                         /// 00b2c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b30
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b34
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b38
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b3c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b40
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b54
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b58
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b60
			 0xcb000000,                                                  // -8388608.0                                  /// 00b64
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0xff800000,                                                  // -inf                                        /// 00b6c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b70
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b74
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b78
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b84
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b88
			 0x33333333,                                                  // 4 random values                             /// 00b8c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b94
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b9c
			 0x55555555,                                                  // 4 random values                             /// 00ba0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ba8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bb4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00bbc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bc0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bc4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bc8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bcc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bd0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bd4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00bd8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bdc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00be4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00be8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bf0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bf4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bf8
			 0x3f028f5c,                                                  // 0.51                                        /// 00bfc
			 0x80000000,                                                  // -zero                                       /// 00c00
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c08
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c0c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c10
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c14
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c18
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c20
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c28
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c2c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c30
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c34
			 0xff800000,                                                  // -inf                                        /// 00c38
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c3c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c44
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c48
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c4c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c50
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c54
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c58
			 0x3f028f5c,                                                  // 0.51                                        /// 00c5c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c64
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c68
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c6c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c78
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c7c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c80
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c88
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c8c
			 0xcb000000,                                                  // -8388608.0                                  /// 00c90
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c94
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c98
			 0x4b000000,                                                  // 8388608.0                                   /// 00c9c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ca0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ca4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cb0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cb8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0xff800000,                                                  // -inf                                        /// 00cc0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cc8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ccc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cd0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cd4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cd8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ce0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ce4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ce8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cf0
			 0xffc00001,                                                  // -signaling NaN                              /// 00cf4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cf8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d08
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d14
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d18
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d1c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d20
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x3f028f5c,                                                  // 0.51                                        /// 00d2c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d30
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00d34
			 0x55555555,                                                  // 4 random values                             /// 00d38
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d3c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d40
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d48
			 0x7f800000,                                                  // inf                                         /// 00d4c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d50
			 0x55555555,                                                  // 4 random values                             /// 00d54
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d5c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d64
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d68
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d6c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d74
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d78
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d7c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d80
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d84
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d88
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d8c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d94
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d98
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d9c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00da0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00da4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00da8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dac
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00db0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00db4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00db8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00dbc
			 0x55555555,                                                  // 4 random values                             /// 00dc0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00dc4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00dc8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00dcc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0xff800000,                                                  // -inf                                        /// 00de0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00de4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00de8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00dec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00df0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00df4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00df8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00dfc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e08
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e0c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e14
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e18
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0xffc00001,                                                  // -signaling NaN                              /// 00e20
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e24
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e28
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e30
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e34
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e3c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e40
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e44
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e48
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00e4c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x80000000,                                                  // -zero                                       /// 00e54
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e58
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e5c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e60
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e70
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e78
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e80
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e88
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e8c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e98
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e9c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ea0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ea4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ea8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00eac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00eb4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00eb8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ebc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ec0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ec4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ec8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ed0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ed4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ed8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00edc
			 0x7fc00001,                                                  // signaling NaN                               /// 00ee0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ee4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ee8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ef0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ef4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ef8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f00
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f04
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f0c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f10
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f14
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f18
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f1c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f20
			 0xff800000,                                                  // -inf                                        /// 00f24
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f28
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f2c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f30
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f34
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f38
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f3c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f40
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f44
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f4c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f5c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f60
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f64
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f78
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f7c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f80
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f84
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f88
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f8c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f90
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f94
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f98
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f9c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fa4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fa8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fac
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fb0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fb4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fc0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fc4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fcc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fdc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fe0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fe4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fe8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ff0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ff4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ff8
			 0x80800001 // min norm + 1ulp                               // subnormals -ve                              /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00000
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00004
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00008
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00010
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00014
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00020
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00024
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00028
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00030
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00034
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00038
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0003c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00040
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00044
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0004c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00050
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00054
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00058
			 0xffc00001,                                                  // -signaling NaN                              /// 0005c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00060
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0006c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x00011111,                                                  // 9.7958E-41                                  /// 00074
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0007c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00080
			 0xffc00001,                                                  // -signaling NaN                              /// 00084
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00088
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0008c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0009c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000a0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000a8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 000b0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000b8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000bc
			 0xffc00001,                                                  // -signaling NaN                              /// 000c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 000c4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000c8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000d0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000d4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000d8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000dc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000e0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000e8
			 0x0c780000,                                                  // Leading 1s:                                 /// 000ec
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000f0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000f8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00104
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00108
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0010c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00110
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0011c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00120
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00124
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00128
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0012c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00130
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00134
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00138
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0013c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x0c400000,                                                  // Leading 1s:                                 /// 00144
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00148
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0014c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00150
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00154
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0015c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00160
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00164
			 0x0c400000,                                                  // Leading 1s:                                 /// 00168
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0016c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00170
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00174
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0017c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00180
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00184
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0018c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00190
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00194
			 0x4b000000,                                                  // 8388608.0                                   /// 00198
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0019c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001a0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001a8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001ac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001b0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001b8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001bc
			 0x00000000,                                                  // zero                                        /// 001c0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001c4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001c8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001d0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 001d8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001dc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001ec
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001f0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001f4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001fc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00200
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00204
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00208
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00210
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00214
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0021c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00220
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00224
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00228
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0022c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00230
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00238
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0023c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00244
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00248
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00250
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00254
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00258
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0025c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00260
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00264
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0026c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00270
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00274
			 0x3f028f5c,                                                  // 0.51                                        /// 00278
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0027c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00280
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00284
			 0x00011111,                                                  // 9.7958E-41                                  /// 00288
			 0x80000000,                                                  // -zero                                       /// 0028c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0029c
			 0x0e000003,                                                  // Trailing 1s:                                /// 002a0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002a4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002a8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002b0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x7f800000,                                                  // inf                                         /// 002bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002c4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002c8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002d0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002d8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002e0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002e8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002ec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002f8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x0e000003,                                                  // Trailing 1s:                                /// 00304
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00308
			 0xcb000000,                                                  // -8388608.0                                  /// 0030c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00310
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00314
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00318
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0031c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00324
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0032c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00330
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00338
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0xff800000,                                                  // -inf                                        /// 00344
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00348
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0034c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00350
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00354
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00358
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0035c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00360
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00364
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x33333333,                                                  // 4 random values                             /// 0036c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00370
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00374
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00378
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00384
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00388
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0038c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00390
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00394
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00398
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0039c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003b4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003bc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003c8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003cc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003d4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003d8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003dc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003e8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 003f4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003fc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00400
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00404
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00408
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0040c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00410
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00414
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00420
			 0x3f028f5c,                                                  // 0.51                                        /// 00424
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00428
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0042c
			 0xffc00001,                                                  // -signaling NaN                              /// 00430
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00434
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00438
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0043c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00448
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0044c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00450
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00454
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0045c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00460
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00464
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00470
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00474
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00478
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x00011111,                                                  // 9.7958E-41                                  /// 00484
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00488
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0048c
			 0x7f800000,                                                  // inf                                         /// 00490
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00494
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00498
			 0xff800000,                                                  // -inf                                        /// 0049c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004a4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004ac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004b0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004b4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004bc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 004c4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004c8
			 0x0e000001,                                                  // Trailing 1s:                                /// 004cc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x80000000,                                                  // -zero                                       /// 004d8
			 0x7f800000,                                                  // inf                                         /// 004dc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004e4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004e8
			 0x0e000003,                                                  // Trailing 1s:                                /// 004ec
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004f0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x7fc00001,                                                  // signaling NaN                               /// 004f8
			 0xbf028f5c,                                                  // -0.51                                       /// 004fc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00500
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00504
			 0x00011111,                                                  // 9.7958E-41                                  /// 00508
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00510
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00514
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0051c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x7f800000,                                                  // inf                                         /// 00528
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00534
			 0x80000000,                                                  // -zero                                       /// 00538
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0053c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00540
			 0xbf028f5c,                                                  // -0.51                                       /// 00544
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00548
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0054c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00550
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00554
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00558
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0055c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00560
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00564
			 0x0e000001,                                                  // Trailing 1s:                                /// 00568
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0056c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00570
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x80000000,                                                  // -zero                                       /// 00578
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0057c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00580
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00584
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00588
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0058c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00590
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00598
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0059c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005a0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005a4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005a8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005b4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005b8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005bc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005c0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005c4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005c8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005cc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005d0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005d4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005d8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005dc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 005e0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005e4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005e8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005f0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 005f4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005f8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00600
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00604
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00608
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0060c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00610
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00614
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00620
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00624
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00628
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0062c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00634
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00638
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0063c
			 0x00000000,                                                  // zero                                        /// 00640
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00644
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00648
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00650
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00654
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00658
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0065c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00664
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0066c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00670
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00674
			 0x3f028f5c,                                                  // 0.51                                        /// 00678
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0xbf028f5c,                                                  // -0.51                                       /// 00680
			 0x0c700000,                                                  // Leading 1s:                                 /// 00684
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00688
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0068c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00690
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00694
			 0x0e000001,                                                  // Trailing 1s:                                /// 00698
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0069c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006a0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006a4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006ac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006b0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006bc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006c0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006c4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006c8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006cc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006d0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006d4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006dc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006e4
			 0x4b000000,                                                  // 8388608.0                                   /// 006e8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006ec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006f0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006f8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00714
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00718
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00720
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00724
			 0x55555555,                                                  // 4 random values                             /// 00728
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00734
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00738
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0073c
			 0x80000000,                                                  // -zero                                       /// 00740
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00744
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00748
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0074c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x0e000007,                                                  // Trailing 1s:                                /// 00754
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0075c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x0e000003,                                                  // Trailing 1s:                                /// 00768
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0076c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00774
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00778
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0077c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00780
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00788
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0078c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00790
			 0x0e000003,                                                  // Trailing 1s:                                /// 00794
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0079c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007a0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007ac
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007b0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007b8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 007bc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007c0
			 0x7fc00001,                                                  // signaling NaN                               /// 007c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007c8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007cc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007d0
			 0x0e000001,                                                  // Trailing 1s:                                /// 007d4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007d8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007e0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007e8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007ec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007f0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007f4
			 0x33333333,                                                  // 4 random values                             /// 007f8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00808
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0080c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00810
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00814
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x0c780000,                                                  // Leading 1s:                                 /// 0081c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00820
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00828
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0082c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00830
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00834
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00838
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00844
			 0x00011111,                                                  // 9.7958E-41                                  /// 00848
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0084c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00850
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00854
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00858
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0085c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00864
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00868
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00870
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00878
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0087c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00880
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00884
			 0x0e000001,                                                  // Trailing 1s:                                /// 00888
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0088c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00898
			 0x00011111,                                                  // 9.7958E-41                                  /// 0089c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x7f800000,                                                  // inf                                         /// 008a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008ac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008b0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008b4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008b8
			 0x0c600000,                                                  // Leading 1s:                                 /// 008bc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008c0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008cc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008d0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008d4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008d8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008dc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 008e8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008ec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008f0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008f8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008fc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00904
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00908
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00910
			 0x0e000007,                                                  // Trailing 1s:                                /// 00914
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00918
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00920
			 0x80000000,                                                  // -zero                                       /// 00924
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00928
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0092c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00930
			 0x7f800000,                                                  // inf                                         /// 00934
			 0x80011111,                                                  // -9.7958E-41                                 /// 00938
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0093c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00940
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00944
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0xff800000,                                                  // -inf                                        /// 0094c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00950
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00954
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00958
			 0xcb000000,                                                  // -8388608.0                                  /// 0095c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00960
			 0x80000000,                                                  // -zero                                       /// 00964
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00968
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0096c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00970
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00974
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0097c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00984
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00988
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0098c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00990
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00994
			 0xcb000000,                                                  // -8388608.0                                  /// 00998
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0099c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009a0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009a8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009ac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009b0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009b4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009b8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009c8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009cc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009d0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009d4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009d8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009dc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009e0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009e4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009ec
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009f0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009f4
			 0xffc00001,                                                  // -signaling NaN                              /// 009f8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 009fc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a00
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a08
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a0c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a10
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a18
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a1c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0xff800000,                                                  // -inf                                        /// 00a24
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a28
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a2c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a30
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a34
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a38
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a3c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a40
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a44
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a48
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a4c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a50
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a60
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a6c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a70
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x33333333,                                                  // 4 random values                             /// 00a78
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a84
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a88
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a8c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a90
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a94
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a98
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a9c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00aa0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00aa4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ab0
			 0x33333333,                                                  // 4 random values                             /// 00ab4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x55555555,                                                  // 4 random values                             /// 00abc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ac0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ac8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ad0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ad4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ad8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00adc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ae4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00aec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00af0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00af8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b04
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b08
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b14
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b18
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b1c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b20
			 0xcb000000,                                                  // -8388608.0                                  /// 00b24
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b28
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b2c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b30
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x00000000,                                                  // zero                                        /// 00b38
			 0xff800000,                                                  // -inf                                        /// 00b3c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b40
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b4c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b50
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b54
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b58
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b5c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b64
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b6c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b70
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b74
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b78
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b7c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b84
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b88
			 0xff800000,                                                  // -inf                                        /// 00b8c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b90
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b94
			 0x80000000,                                                  // -zero                                       /// 00b98
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b9c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ba4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ba8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x4b000000,                                                  // 8388608.0                                   /// 00bb8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00bbc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bc0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bc4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0x55555555,                                                  // 4 random values                             /// 00bcc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bd0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bd4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00bd8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00be0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00be4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00be8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x7fc00001,                                                  // signaling NaN                               /// 00bf0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bf8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bfc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c08
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c0c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c10
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c14
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c18
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c1c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c20
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c28
			 0x3f028f5c,                                                  // 0.51                                        /// 00c2c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c30
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c34
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c38
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c3c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c44
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c48
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c4c
			 0x00000000,                                                  // zero                                        /// 00c50
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c54
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c5c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c60
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c64
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c68
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c6c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c70
			 0x3f028f5c,                                                  // 0.51                                        /// 00c74
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c78
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c84
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c88
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c8c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c98
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c9c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ca0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0xffc00001,                                                  // -signaling NaN                              /// 00cac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cb0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x7fc00001,                                                  // signaling NaN                               /// 00cb8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cbc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cc0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cc4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cc8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cd0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cd4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cd8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cdc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ce0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ce4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ce8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cf0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cf4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cf8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cfc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d00
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x55555555,                                                  // 4 random values                             /// 00d08
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d0c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d10
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d14
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d18
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d1c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d24
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d28
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d2c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d30
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d34
			 0x33333333,                                                  // 4 random values                             /// 00d38
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d3c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d40
			 0x7f800000,                                                  // inf                                         /// 00d44
			 0x33333333,                                                  // 4 random values                             /// 00d48
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d4c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d58
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d5c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d64
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d68
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d6c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d74
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d78
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d7c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d80
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d84
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d88
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d8c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d90
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d94
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d98
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d9c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00da0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00da4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00da8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00db4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x00000000,                                                  // zero                                        /// 00dbc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dc0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00dc8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dcc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00dd0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00dd4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00dd8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ddc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00de0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00de4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00de8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00dec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00df0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00df4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00df8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00dfc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e00
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e08
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e0c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e10
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e14
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e18
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e1c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e20
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e24
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e28
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e2c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e30
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e34
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e38
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e3c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e48
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e4c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e58
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e5c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e64
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e68
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e6c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e70
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e78
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e7c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e84
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e88
			 0x00000000,                                                  // zero                                        /// 00e8c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e90
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e94
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ea0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ea4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ea8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00eb0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00eb4
			 0xcb000000,                                                  // -8388608.0                                  /// 00eb8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ebc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ec0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ecc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ed4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ed8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00edc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ee0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ee4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ee8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ef0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ef8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00efc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f00
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f0c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f10
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f14
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f18
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f1c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f20
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f24
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f2c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f30
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f34
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f3c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f44
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f48
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f54
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f58
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f5c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f60
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f68
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f6c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f70
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f74
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f78
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f84
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f88
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f90
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f94
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f98
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fa4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fa8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fb0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fb4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fb8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fc4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fc8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fcc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fd0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fd4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fd8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fdc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fe0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ff0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ff4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ff8
			 0x0c7f0000                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x986ab3c2, /// 0x0
			 0x919cca79, /// 0x4
			 0xe35f092f, /// 0x8
			 0x79d108f0, /// 0xc
			 0x07f93128, /// 0x10
			 0x89ddec0b, /// 0x14
			 0x9f1ba0d0, /// 0x18
			 0x799e8162, /// 0x1c
			 0x46e3dfeb, /// 0x20
			 0x0c51dd86, /// 0x24
			 0xf6b28cd6, /// 0x28
			 0x75862b39, /// 0x2c
			 0x28909607, /// 0x30
			 0xf76ed61f, /// 0x34
			 0x44bf579b, /// 0x38
			 0x075b5de7, /// 0x3c
			 0xf45334d9, /// 0x40
			 0xb2871b8d, /// 0x44
			 0xde6386b8, /// 0x48
			 0xbe3ae7a0, /// 0x4c
			 0xe7bf0585, /// 0x50
			 0x4655b81f, /// 0x54
			 0x2c30c4c8, /// 0x58
			 0x8a496609, /// 0x5c
			 0x79031091, /// 0x60
			 0x1e65173a, /// 0x64
			 0xd7a9a1a7, /// 0x68
			 0xdb5bca0f, /// 0x6c
			 0x488fee76, /// 0x70
			 0x391663df, /// 0x74
			 0x8855ff82, /// 0x78
			 0xc80c9059, /// 0x7c
			 0xe48203a4, /// 0x80
			 0x5af1ea17, /// 0x84
			 0xfe8d24c8, /// 0x88
			 0xfc7788de, /// 0x8c
			 0x61255cb7, /// 0x90
			 0xead90daa, /// 0x94
			 0xe66591d0, /// 0x98
			 0xb8f5998b, /// 0x9c
			 0xa7dd581d, /// 0xa0
			 0x3e273f0a, /// 0xa4
			 0x14f98f1b, /// 0xa8
			 0xb43a0f10, /// 0xac
			 0xc44db4d8, /// 0xb0
			 0x3eaa74a1, /// 0xb4
			 0x00a85d5b, /// 0xb8
			 0x3b8d801a, /// 0xbc
			 0x977e79d1, /// 0xc0
			 0xa7766ba6, /// 0xc4
			 0xbfdc963b, /// 0xc8
			 0x61e780e3, /// 0xcc
			 0x2500f651, /// 0xd0
			 0x4053f6fe, /// 0xd4
			 0xaac779dd, /// 0xd8
			 0xc353865b, /// 0xdc
			 0x724a24c6, /// 0xe0
			 0x846aed77, /// 0xe4
			 0xa9ce95c4, /// 0xe8
			 0x22f6099d, /// 0xec
			 0x2ad0171e, /// 0xf0
			 0xd72a993d, /// 0xf4
			 0x2e0d3e9f, /// 0xf8
			 0x6ae996e0, /// 0xfc
			 0xd8886903, /// 0x100
			 0x00b62193, /// 0x104
			 0x2c418f91, /// 0x108
			 0xf8b742fa, /// 0x10c
			 0x22982877, /// 0x110
			 0x77273af3, /// 0x114
			 0xc63890d1, /// 0x118
			 0x155abfed, /// 0x11c
			 0x3eb4295e, /// 0x120
			 0x4dc2c622, /// 0x124
			 0x1f63846c, /// 0x128
			 0xff7e5cc6, /// 0x12c
			 0xfc19b2d4, /// 0x130
			 0xf929fc23, /// 0x134
			 0x36e0ad57, /// 0x138
			 0xb8fe2fa9, /// 0x13c
			 0x50949404, /// 0x140
			 0x272cb3f9, /// 0x144
			 0xe4a68d84, /// 0x148
			 0xc8567f80, /// 0x14c
			 0x661df71a, /// 0x150
			 0x59106ff0, /// 0x154
			 0x13588b6d, /// 0x158
			 0x69718b2c, /// 0x15c
			 0x6692a99a, /// 0x160
			 0x7b3187ee, /// 0x164
			 0x5bd9de38, /// 0x168
			 0xa4b9849b, /// 0x16c
			 0xccec8d2d, /// 0x170
			 0x504e82b6, /// 0x174
			 0x3764cca0, /// 0x178
			 0xb2fb95f4, /// 0x17c
			 0xab192447, /// 0x180
			 0x6a7885be, /// 0x184
			 0xc8e954d7, /// 0x188
			 0x23d26e0e, /// 0x18c
			 0x7e52ea51, /// 0x190
			 0x3e3a898e, /// 0x194
			 0xa7f982ab, /// 0x198
			 0x4dd67ff6, /// 0x19c
			 0xeb037cb4, /// 0x1a0
			 0xd0ae3c9a, /// 0x1a4
			 0x22ded3d5, /// 0x1a8
			 0x8d8bb8f1, /// 0x1ac
			 0xcc332650, /// 0x1b0
			 0xbc9f30f4, /// 0x1b4
			 0x6e3266c4, /// 0x1b8
			 0x6afb6f1b, /// 0x1bc
			 0x7dc3ee07, /// 0x1c0
			 0x304c91dd, /// 0x1c4
			 0x88467cf1, /// 0x1c8
			 0x76486575, /// 0x1cc
			 0x18fe985c, /// 0x1d0
			 0x510801f4, /// 0x1d4
			 0x453975e6, /// 0x1d8
			 0xe3279e12, /// 0x1dc
			 0x165c38bb, /// 0x1e0
			 0xe7ca880a, /// 0x1e4
			 0xaff4d59e, /// 0x1e8
			 0x75ce4322, /// 0x1ec
			 0x07156e4a, /// 0x1f0
			 0xcc877bc7, /// 0x1f4
			 0x1de20aac, /// 0x1f8
			 0x3a09b58f, /// 0x1fc
			 0xb4837199, /// 0x200
			 0xda8235cb, /// 0x204
			 0xb648aa32, /// 0x208
			 0xf5a48dd2, /// 0x20c
			 0x6328bdcc, /// 0x210
			 0x7be8b9ca, /// 0x214
			 0xe4de5a28, /// 0x218
			 0xfa422283, /// 0x21c
			 0x13bd9426, /// 0x220
			 0xc7ced18e, /// 0x224
			 0x83374452, /// 0x228
			 0x428c042b, /// 0x22c
			 0x14153238, /// 0x230
			 0x91e2b2a1, /// 0x234
			 0xe38fd3eb, /// 0x238
			 0x1eb82af2, /// 0x23c
			 0xca2023ba, /// 0x240
			 0xa483fb88, /// 0x244
			 0x38fc6183, /// 0x248
			 0x2aed925e, /// 0x24c
			 0x90907711, /// 0x250
			 0xe4ff016f, /// 0x254
			 0x008ad77e, /// 0x258
			 0x034a51b1, /// 0x25c
			 0xf5ea36dc, /// 0x260
			 0x450290f6, /// 0x264
			 0x62689a27, /// 0x268
			 0xbc2858a3, /// 0x26c
			 0x1b727dce, /// 0x270
			 0x68d01b05, /// 0x274
			 0x19c0c421, /// 0x278
			 0x6ab84de6, /// 0x27c
			 0x3a7b51f7, /// 0x280
			 0x345a2219, /// 0x284
			 0xbe4886ec, /// 0x288
			 0x29b311f1, /// 0x28c
			 0x241bc850, /// 0x290
			 0x4c7d4887, /// 0x294
			 0x26fe3698, /// 0x298
			 0xa34b15c9, /// 0x29c
			 0xdce3dc11, /// 0x2a0
			 0xef45c11e, /// 0x2a4
			 0x97b60554, /// 0x2a8
			 0xc6bb37ab, /// 0x2ac
			 0x26a436ae, /// 0x2b0
			 0xa5bbb028, /// 0x2b4
			 0x088cb14a, /// 0x2b8
			 0x13054e30, /// 0x2bc
			 0x6d360132, /// 0x2c0
			 0x00a36f99, /// 0x2c4
			 0x588a588a, /// 0x2c8
			 0xadd8eac6, /// 0x2cc
			 0x34c665a1, /// 0x2d0
			 0x011d1f60, /// 0x2d4
			 0x8b27bb3c, /// 0x2d8
			 0x0c4a78f8, /// 0x2dc
			 0xda4f16f9, /// 0x2e0
			 0x7f56ab3e, /// 0x2e4
			 0x2f0ebaa0, /// 0x2e8
			 0xd205c44e, /// 0x2ec
			 0xbd8e1644, /// 0x2f0
			 0x2739ab35, /// 0x2f4
			 0x6f5bf418, /// 0x2f8
			 0xd55e3a86, /// 0x2fc
			 0x60d9f7ae, /// 0x300
			 0x4150581b, /// 0x304
			 0xecc1cfe3, /// 0x308
			 0x986f3a98, /// 0x30c
			 0x0e0c3a4c, /// 0x310
			 0x05a2e82e, /// 0x314
			 0x42528c1f, /// 0x318
			 0xe9aa2ee0, /// 0x31c
			 0x5b7b4fe1, /// 0x320
			 0x0ff08dca, /// 0x324
			 0x6923dcf1, /// 0x328
			 0x0ffb093a, /// 0x32c
			 0xfcd526b6, /// 0x330
			 0xa69c4cb4, /// 0x334
			 0x16018f58, /// 0x338
			 0x50186994, /// 0x33c
			 0xde94f91f, /// 0x340
			 0x2691bb3c, /// 0x344
			 0xd60ada6f, /// 0x348
			 0xcd78e591, /// 0x34c
			 0xe29a2dcc, /// 0x350
			 0x5b0a242b, /// 0x354
			 0xc5706b8c, /// 0x358
			 0x1bdc9f5a, /// 0x35c
			 0xad3a2997, /// 0x360
			 0xd7d98c29, /// 0x364
			 0xd9a46d06, /// 0x368
			 0x74899556, /// 0x36c
			 0xb6e35585, /// 0x370
			 0x39140876, /// 0x374
			 0x6b2db70a, /// 0x378
			 0xd80ddd49, /// 0x37c
			 0xa31fcba5, /// 0x380
			 0x7acbbb50, /// 0x384
			 0x81f0d25f, /// 0x388
			 0x74f61f9a, /// 0x38c
			 0x8aeb1a8a, /// 0x390
			 0x97179ed0, /// 0x394
			 0x4e9876f3, /// 0x398
			 0x9bc0708a, /// 0x39c
			 0x2c5a51ed, /// 0x3a0
			 0x7d399721, /// 0x3a4
			 0x37035d8c, /// 0x3a8
			 0x7349334b, /// 0x3ac
			 0x042e0308, /// 0x3b0
			 0x70e9172e, /// 0x3b4
			 0xf9fcc4fb, /// 0x3b8
			 0x2ca32f4f, /// 0x3bc
			 0x6623eb5b, /// 0x3c0
			 0x73b485fa, /// 0x3c4
			 0x6e512303, /// 0x3c8
			 0xc9d3ecf5, /// 0x3cc
			 0xbe56ec1e, /// 0x3d0
			 0x22966b43, /// 0x3d4
			 0xb0316a82, /// 0x3d8
			 0x406c500e, /// 0x3dc
			 0xc7e15a49, /// 0x3e0
			 0x7a714cb2, /// 0x3e4
			 0x2ef6a692, /// 0x3e8
			 0xeb85d9eb, /// 0x3ec
			 0x42c7ecec, /// 0x3f0
			 0x4688550b, /// 0x3f4
			 0x4e106e5f, /// 0x3f8
			 0x4a6368c9, /// 0x3fc
			 0x0a8a68b0, /// 0x400
			 0x9b505bd8, /// 0x404
			 0x519c4c57, /// 0x408
			 0x8b3afe1a, /// 0x40c
			 0x262e77b7, /// 0x410
			 0xc7082dbd, /// 0x414
			 0xe8f793c6, /// 0x418
			 0xd7cd5711, /// 0x41c
			 0x6b1e5f46, /// 0x420
			 0x474052fd, /// 0x424
			 0x413132ae, /// 0x428
			 0xf4e11579, /// 0x42c
			 0x5f5aae30, /// 0x430
			 0x9acc7308, /// 0x434
			 0x9b4179d5, /// 0x438
			 0xf14aeb90, /// 0x43c
			 0xaf59f30b, /// 0x440
			 0x94b1a8d0, /// 0x444
			 0xc81c33e8, /// 0x448
			 0xef2df093, /// 0x44c
			 0x65ca00b0, /// 0x450
			 0x074e49ef, /// 0x454
			 0xc0a3d8da, /// 0x458
			 0x9733c89b, /// 0x45c
			 0x307523e7, /// 0x460
			 0xdd6d8647, /// 0x464
			 0x48bc588f, /// 0x468
			 0x3cd1a1ff, /// 0x46c
			 0x7b65b10f, /// 0x470
			 0xa12957ea, /// 0x474
			 0x2f36294a, /// 0x478
			 0xb4af6c5c, /// 0x47c
			 0x5aecd01a, /// 0x480
			 0xae924424, /// 0x484
			 0xb7a100d2, /// 0x488
			 0x581d0d8f, /// 0x48c
			 0x56bee9ed, /// 0x490
			 0x7e35b9fd, /// 0x494
			 0x135bdd0f, /// 0x498
			 0xf2b3f694, /// 0x49c
			 0xcccebeb5, /// 0x4a0
			 0x9a4efe8f, /// 0x4a4
			 0x58f805ad, /// 0x4a8
			 0xd3ff1fc1, /// 0x4ac
			 0xc009466f, /// 0x4b0
			 0xb5a22684, /// 0x4b4
			 0x094c6d6f, /// 0x4b8
			 0x22b216a0, /// 0x4bc
			 0x4ae95226, /// 0x4c0
			 0x08b42f5b, /// 0x4c4
			 0x8be692d7, /// 0x4c8
			 0xcd54e03c, /// 0x4cc
			 0xa1d3f590, /// 0x4d0
			 0x003af940, /// 0x4d4
			 0x968dffdf, /// 0x4d8
			 0xfbd51162, /// 0x4dc
			 0xae7cb7ee, /// 0x4e0
			 0x1e767be5, /// 0x4e4
			 0xde396cbc, /// 0x4e8
			 0x566fb7e3, /// 0x4ec
			 0x3cd040dc, /// 0x4f0
			 0x9e711f90, /// 0x4f4
			 0x725ab2be, /// 0x4f8
			 0x4a2b8770, /// 0x4fc
			 0x66230046, /// 0x500
			 0x1430688d, /// 0x504
			 0x77b9c895, /// 0x508
			 0x764deb08, /// 0x50c
			 0x9664fc05, /// 0x510
			 0x28442d3f, /// 0x514
			 0x80095643, /// 0x518
			 0x4216d7fb, /// 0x51c
			 0x734c324b, /// 0x520
			 0x463543ab, /// 0x524
			 0x4cb81984, /// 0x528
			 0x8f1de1ed, /// 0x52c
			 0x116c5b67, /// 0x530
			 0x3fe713df, /// 0x534
			 0x47ad94fe, /// 0x538
			 0xa4f110fb, /// 0x53c
			 0xa08da030, /// 0x540
			 0x941aa6e6, /// 0x544
			 0x32ed3354, /// 0x548
			 0x847228dc, /// 0x54c
			 0xd4b717e6, /// 0x550
			 0x397c4e6a, /// 0x554
			 0xce775618, /// 0x558
			 0xae761e64, /// 0x55c
			 0x574b15ca, /// 0x560
			 0x507d39a7, /// 0x564
			 0x26c8cfef, /// 0x568
			 0xcb87d0a7, /// 0x56c
			 0x64db1c52, /// 0x570
			 0xa897ffab, /// 0x574
			 0x80148ba6, /// 0x578
			 0x48e32d8f, /// 0x57c
			 0xa52df994, /// 0x580
			 0x2050365f, /// 0x584
			 0x14f3fb89, /// 0x588
			 0x7152148f, /// 0x58c
			 0x76241791, /// 0x590
			 0x0817bcef, /// 0x594
			 0xbca51746, /// 0x598
			 0x3ed16c15, /// 0x59c
			 0x21337a0b, /// 0x5a0
			 0x053051eb, /// 0x5a4
			 0xf0c8677b, /// 0x5a8
			 0xc0ce94a7, /// 0x5ac
			 0xaa7ed78d, /// 0x5b0
			 0xc948c81c, /// 0x5b4
			 0x0592755f, /// 0x5b8
			 0xd40f6623, /// 0x5bc
			 0x3acef0e4, /// 0x5c0
			 0x17c0f0d7, /// 0x5c4
			 0x2a39e5c2, /// 0x5c8
			 0xe05dc5c1, /// 0x5cc
			 0x3b26bd9e, /// 0x5d0
			 0xc93718cf, /// 0x5d4
			 0x82bba921, /// 0x5d8
			 0xaed324e4, /// 0x5dc
			 0x8f7f2da0, /// 0x5e0
			 0x787a72e2, /// 0x5e4
			 0xba466fb6, /// 0x5e8
			 0xe5c6e7a9, /// 0x5ec
			 0x02c79ce4, /// 0x5f0
			 0x96fc3195, /// 0x5f4
			 0x50d76530, /// 0x5f8
			 0xee31fcc7, /// 0x5fc
			 0x21514baa, /// 0x600
			 0x66bd1227, /// 0x604
			 0xe022de38, /// 0x608
			 0xc328bb96, /// 0x60c
			 0xf6288034, /// 0x610
			 0x696188e9, /// 0x614
			 0x862c7471, /// 0x618
			 0x29c874da, /// 0x61c
			 0x30927933, /// 0x620
			 0xa00b17aa, /// 0x624
			 0xd0922065, /// 0x628
			 0xc74772e8, /// 0x62c
			 0x1086aaae, /// 0x630
			 0x26b29098, /// 0x634
			 0xa8b3deb7, /// 0x638
			 0xffa12035, /// 0x63c
			 0xd5b91388, /// 0x640
			 0x242411b6, /// 0x644
			 0x924f0ca3, /// 0x648
			 0xf0cd0f22, /// 0x64c
			 0x827344c5, /// 0x650
			 0x3775ad90, /// 0x654
			 0xae2c35c2, /// 0x658
			 0xcbae80ad, /// 0x65c
			 0x6232c4a1, /// 0x660
			 0x11342287, /// 0x664
			 0xb62fe1fc, /// 0x668
			 0x8d7cb89c, /// 0x66c
			 0xbff704dd, /// 0x670
			 0x088b3b36, /// 0x674
			 0xa9d87d51, /// 0x678
			 0x4951591e, /// 0x67c
			 0xdbd72b27, /// 0x680
			 0x59426484, /// 0x684
			 0x3d458ea7, /// 0x688
			 0xee0495ea, /// 0x68c
			 0xff55d557, /// 0x690
			 0x6d1dca6d, /// 0x694
			 0x5f59ac43, /// 0x698
			 0x73df6b1a, /// 0x69c
			 0x37ae3a40, /// 0x6a0
			 0x1288b66a, /// 0x6a4
			 0x157efb12, /// 0x6a8
			 0x9c00606b, /// 0x6ac
			 0x12ff31b9, /// 0x6b0
			 0xb5f5e785, /// 0x6b4
			 0xc70671bc, /// 0x6b8
			 0xc33abef9, /// 0x6bc
			 0xb3f61bad, /// 0x6c0
			 0xb50384c4, /// 0x6c4
			 0x1d3988fc, /// 0x6c8
			 0x24344b1d, /// 0x6cc
			 0xed86f820, /// 0x6d0
			 0xbfc1cb61, /// 0x6d4
			 0x19b5d323, /// 0x6d8
			 0xfc66f3b4, /// 0x6dc
			 0x18794852, /// 0x6e0
			 0x7026f007, /// 0x6e4
			 0xeb64ff67, /// 0x6e8
			 0xb1209e29, /// 0x6ec
			 0xe42497a6, /// 0x6f0
			 0x583e8261, /// 0x6f4
			 0xcc6b5695, /// 0x6f8
			 0x1aa11f36, /// 0x6fc
			 0xabb39be8, /// 0x700
			 0x63415f6b, /// 0x704
			 0x39de6269, /// 0x708
			 0x9e5b65ab, /// 0x70c
			 0x740f162e, /// 0x710
			 0x9674290c, /// 0x714
			 0xd40d7784, /// 0x718
			 0x1a9df2d7, /// 0x71c
			 0xb8637e6e, /// 0x720
			 0x6ef7caf5, /// 0x724
			 0xcc565c42, /// 0x728
			 0x1203209d, /// 0x72c
			 0xcedde431, /// 0x730
			 0xe9bb96e9, /// 0x734
			 0x5a87fe26, /// 0x738
			 0x4aaaeed5, /// 0x73c
			 0xa0b0f1ef, /// 0x740
			 0xa6ebef73, /// 0x744
			 0xfb62900b, /// 0x748
			 0xdf059be0, /// 0x74c
			 0xe4d2d528, /// 0x750
			 0xe3473b3e, /// 0x754
			 0x5bdb5ac7, /// 0x758
			 0x16ffc555, /// 0x75c
			 0x876cb7c7, /// 0x760
			 0x64cd2ec6, /// 0x764
			 0x6263e8c2, /// 0x768
			 0x604d4c4b, /// 0x76c
			 0x694b7f76, /// 0x770
			 0x50f1837a, /// 0x774
			 0x98bd137d, /// 0x778
			 0x2332b40b, /// 0x77c
			 0xd3ae0832, /// 0x780
			 0x16aaadd9, /// 0x784
			 0x6f42e636, /// 0x788
			 0xa1fc2c07, /// 0x78c
			 0xc2edaacd, /// 0x790
			 0xde100d2c, /// 0x794
			 0x92ab4d19, /// 0x798
			 0x3fe56e1c, /// 0x79c
			 0x8060dd1e, /// 0x7a0
			 0x5c736fed, /// 0x7a4
			 0xd45d9188, /// 0x7a8
			 0x30e2df2d, /// 0x7ac
			 0x0856b803, /// 0x7b0
			 0x176f16e6, /// 0x7b4
			 0xd7205199, /// 0x7b8
			 0x61f1bf3b, /// 0x7bc
			 0x1c1ce4f7, /// 0x7c0
			 0xc6ae1c8a, /// 0x7c4
			 0x9d3511a2, /// 0x7c8
			 0xf1f77618, /// 0x7cc
			 0x2cc5c32b, /// 0x7d0
			 0x58056632, /// 0x7d4
			 0xaf49aba2, /// 0x7d8
			 0x5f40cea5, /// 0x7dc
			 0xfc3ae08e, /// 0x7e0
			 0x80dc6098, /// 0x7e4
			 0x6fb1e279, /// 0x7e8
			 0x9fc5f35c, /// 0x7ec
			 0xff8f5785, /// 0x7f0
			 0xad98f74e, /// 0x7f4
			 0x92e6481b, /// 0x7f8
			 0xf43f3645, /// 0x7fc
			 0xf22e947e, /// 0x800
			 0x3ba4234f, /// 0x804
			 0x2212740b, /// 0x808
			 0xf48f084c, /// 0x80c
			 0xc4d0f141, /// 0x810
			 0x1a6b0cb5, /// 0x814
			 0xfd37a950, /// 0x818
			 0x7894a4e9, /// 0x81c
			 0xc85f7e74, /// 0x820
			 0x4a76374b, /// 0x824
			 0x0ecb9056, /// 0x828
			 0x5ca0d3e4, /// 0x82c
			 0xadc6d37c, /// 0x830
			 0x4ad8ed8e, /// 0x834
			 0xcb428880, /// 0x838
			 0x5f9371a4, /// 0x83c
			 0xc6c69599, /// 0x840
			 0x40c9f037, /// 0x844
			 0x32ba2dad, /// 0x848
			 0x2eaa944a, /// 0x84c
			 0x250a334e, /// 0x850
			 0x48ff3afc, /// 0x854
			 0x0021861c, /// 0x858
			 0x9fa135fd, /// 0x85c
			 0xf4056abf, /// 0x860
			 0x42a0f568, /// 0x864
			 0x690bb458, /// 0x868
			 0x741d85b1, /// 0x86c
			 0xa02464f9, /// 0x870
			 0x4ee1b97e, /// 0x874
			 0x8e1cbe50, /// 0x878
			 0x84454061, /// 0x87c
			 0x1b03d713, /// 0x880
			 0xb874c3ac, /// 0x884
			 0x4c94cb17, /// 0x888
			 0x703b8c98, /// 0x88c
			 0x1b2dd532, /// 0x890
			 0x89e5bab0, /// 0x894
			 0xa9167a87, /// 0x898
			 0x233c63ad, /// 0x89c
			 0xd79b49fd, /// 0x8a0
			 0xe9414b79, /// 0x8a4
			 0xad8f5a05, /// 0x8a8
			 0x236ee7ba, /// 0x8ac
			 0x9e59e7e8, /// 0x8b0
			 0x8c0c588b, /// 0x8b4
			 0xd94b19df, /// 0x8b8
			 0x14deea58, /// 0x8bc
			 0x534460e6, /// 0x8c0
			 0x19023e15, /// 0x8c4
			 0x7d74ae8b, /// 0x8c8
			 0xe64bd393, /// 0x8cc
			 0x60d4330a, /// 0x8d0
			 0x07722959, /// 0x8d4
			 0x1a5599d9, /// 0x8d8
			 0x4fa8ad4d, /// 0x8dc
			 0x1b1c4031, /// 0x8e0
			 0xdd33d236, /// 0x8e4
			 0x40f71e7a, /// 0x8e8
			 0xb5a6202e, /// 0x8ec
			 0xa841bedc, /// 0x8f0
			 0x71de49d6, /// 0x8f4
			 0xcb854a47, /// 0x8f8
			 0x830eb5a1, /// 0x8fc
			 0xe9fcf7b7, /// 0x900
			 0xfd54e4ba, /// 0x904
			 0x80e08085, /// 0x908
			 0xc0697f8c, /// 0x90c
			 0xf72cb3bf, /// 0x910
			 0xc0e4034e, /// 0x914
			 0xc4e456c2, /// 0x918
			 0xc0fc7730, /// 0x91c
			 0x6fc9a5be, /// 0x920
			 0xfbfe6749, /// 0x924
			 0x898e9f14, /// 0x928
			 0xd3617a8d, /// 0x92c
			 0xbf9aff29, /// 0x930
			 0x765810f1, /// 0x934
			 0x52f2767c, /// 0x938
			 0x5a22c9f4, /// 0x93c
			 0x175ac9fe, /// 0x940
			 0x34fd306f, /// 0x944
			 0x070ea50e, /// 0x948
			 0x0208592c, /// 0x94c
			 0x816538f5, /// 0x950
			 0x66de7e59, /// 0x954
			 0xdc5eeef5, /// 0x958
			 0xba968e03, /// 0x95c
			 0x9133c56c, /// 0x960
			 0xdec4cbac, /// 0x964
			 0x72f4e270, /// 0x968
			 0xf2794ff9, /// 0x96c
			 0xd71fac90, /// 0x970
			 0x908aea92, /// 0x974
			 0xeab87a89, /// 0x978
			 0xba498fcc, /// 0x97c
			 0x06edc07f, /// 0x980
			 0x837a83c1, /// 0x984
			 0xf6faa577, /// 0x988
			 0xd63fdd64, /// 0x98c
			 0x9c5c5117, /// 0x990
			 0x0b8c6d92, /// 0x994
			 0x46eff2b3, /// 0x998
			 0x1674edd6, /// 0x99c
			 0x94ae329c, /// 0x9a0
			 0x969f94e7, /// 0x9a4
			 0xd3cb2aab, /// 0x9a8
			 0x5e7efcee, /// 0x9ac
			 0xc0d0ff72, /// 0x9b0
			 0x9a48da63, /// 0x9b4
			 0x6fb460e9, /// 0x9b8
			 0x1054cefe, /// 0x9bc
			 0x302dde61, /// 0x9c0
			 0xb13aaa9f, /// 0x9c4
			 0x1d845820, /// 0x9c8
			 0x278758b9, /// 0x9cc
			 0x9e2fe6ff, /// 0x9d0
			 0x4c7c6a05, /// 0x9d4
			 0x6e32cb04, /// 0x9d8
			 0x64e5c6f5, /// 0x9dc
			 0x02b5d718, /// 0x9e0
			 0x543e703a, /// 0x9e4
			 0x58479665, /// 0x9e8
			 0x8e53359f, /// 0x9ec
			 0x2b38f930, /// 0x9f0
			 0x8458e772, /// 0x9f4
			 0xcba36040, /// 0x9f8
			 0x1bb18962, /// 0x9fc
			 0xb7fd1c82, /// 0xa00
			 0x6ddc19cd, /// 0xa04
			 0xb4e66d56, /// 0xa08
			 0x9b84d014, /// 0xa0c
			 0xad904477, /// 0xa10
			 0x30e3e45b, /// 0xa14
			 0x80033d29, /// 0xa18
			 0xbdc580f1, /// 0xa1c
			 0x651fec01, /// 0xa20
			 0x395c1cb7, /// 0xa24
			 0xe4d4b997, /// 0xa28
			 0x6e68bda6, /// 0xa2c
			 0x8b499f1d, /// 0xa30
			 0xb0671971, /// 0xa34
			 0xb9dce436, /// 0xa38
			 0xabc35999, /// 0xa3c
			 0x9f3bf254, /// 0xa40
			 0x0259cb5a, /// 0xa44
			 0x8198e796, /// 0xa48
			 0xcb8aad90, /// 0xa4c
			 0x1acef033, /// 0xa50
			 0x96573e97, /// 0xa54
			 0x52f58745, /// 0xa58
			 0x51805db0, /// 0xa5c
			 0xe3cd9ad8, /// 0xa60
			 0xe8845d36, /// 0xa64
			 0xd86e4537, /// 0xa68
			 0x838759b6, /// 0xa6c
			 0x3b89907b, /// 0xa70
			 0x70a08239, /// 0xa74
			 0x11a1b31e, /// 0xa78
			 0xe4a84743, /// 0xa7c
			 0xbb1287b0, /// 0xa80
			 0xe53b70b7, /// 0xa84
			 0xfd80d40e, /// 0xa88
			 0x375827e3, /// 0xa8c
			 0x41571024, /// 0xa90
			 0x5be6ee18, /// 0xa94
			 0xa9ce9d3d, /// 0xa98
			 0xa5083a87, /// 0xa9c
			 0xbbd0f34a, /// 0xaa0
			 0x825f975a, /// 0xaa4
			 0x8955e8af, /// 0xaa8
			 0x71d86dff, /// 0xaac
			 0x4da8ce81, /// 0xab0
			 0x186baed5, /// 0xab4
			 0x5223deac, /// 0xab8
			 0xdc08ef2d, /// 0xabc
			 0x0e504078, /// 0xac0
			 0xe8f63f02, /// 0xac4
			 0x7dd1e6f1, /// 0xac8
			 0xb6bdbac9, /// 0xacc
			 0x4b31cf61, /// 0xad0
			 0x26158e39, /// 0xad4
			 0x33a9681f, /// 0xad8
			 0xecc2b8ca, /// 0xadc
			 0xe552da9b, /// 0xae0
			 0x0fdced37, /// 0xae4
			 0xcdf35068, /// 0xae8
			 0x349ba6c7, /// 0xaec
			 0x1a9d3b58, /// 0xaf0
			 0xd71c8e05, /// 0xaf4
			 0xd4e5f391, /// 0xaf8
			 0x5f32420f, /// 0xafc
			 0xdf979821, /// 0xb00
			 0x6337787a, /// 0xb04
			 0x2eacc840, /// 0xb08
			 0xab5daae6, /// 0xb0c
			 0x68850200, /// 0xb10
			 0x6dd81f8c, /// 0xb14
			 0x7605f4d2, /// 0xb18
			 0x8ea810c2, /// 0xb1c
			 0xf3d96405, /// 0xb20
			 0xf5544e19, /// 0xb24
			 0x5e380d37, /// 0xb28
			 0xdd58c5a4, /// 0xb2c
			 0xaa2b8f9b, /// 0xb30
			 0x4997c1a6, /// 0xb34
			 0xcf39b87e, /// 0xb38
			 0xd712909d, /// 0xb3c
			 0xf865a59d, /// 0xb40
			 0x5d0955d3, /// 0xb44
			 0xb9661546, /// 0xb48
			 0xadcc650c, /// 0xb4c
			 0x60239149, /// 0xb50
			 0x5e640398, /// 0xb54
			 0xd7ebfea2, /// 0xb58
			 0x79775985, /// 0xb5c
			 0x1642f3c2, /// 0xb60
			 0xbad8ccec, /// 0xb64
			 0xe95806df, /// 0xb68
			 0xa876fe20, /// 0xb6c
			 0xd007cc08, /// 0xb70
			 0x3a3fb207, /// 0xb74
			 0xb82b8f1a, /// 0xb78
			 0xa01652c0, /// 0xb7c
			 0x7ab6ca75, /// 0xb80
			 0xd5e1587a, /// 0xb84
			 0x11a33070, /// 0xb88
			 0xf8295e39, /// 0xb8c
			 0xc96d7b3e, /// 0xb90
			 0x0f7c3a7f, /// 0xb94
			 0x39a76a4c, /// 0xb98
			 0x6971883f, /// 0xb9c
			 0xd1e393c8, /// 0xba0
			 0x5f9c714c, /// 0xba4
			 0xd58a38a3, /// 0xba8
			 0xbdbf0fa6, /// 0xbac
			 0xc9e559d6, /// 0xbb0
			 0x852c6aff, /// 0xbb4
			 0xd257c480, /// 0xbb8
			 0x15a6b067, /// 0xbbc
			 0xf1761eab, /// 0xbc0
			 0xe952274b, /// 0xbc4
			 0x442c088a, /// 0xbc8
			 0x141269ca, /// 0xbcc
			 0xf95e89ce, /// 0xbd0
			 0x88645c2c, /// 0xbd4
			 0x41f572ba, /// 0xbd8
			 0xa242aa4c, /// 0xbdc
			 0x52eb5d19, /// 0xbe0
			 0xf8ff2fc4, /// 0xbe4
			 0x1ee202ce, /// 0xbe8
			 0x9a00244a, /// 0xbec
			 0x50d41819, /// 0xbf0
			 0x866df05b, /// 0xbf4
			 0x15f4e294, /// 0xbf8
			 0x3a49b2d0, /// 0xbfc
			 0x92c5ef40, /// 0xc00
			 0x5a948c5b, /// 0xc04
			 0xe9bda237, /// 0xc08
			 0x9eb5da26, /// 0xc0c
			 0x6b0e4ede, /// 0xc10
			 0x99aea63a, /// 0xc14
			 0x247a549d, /// 0xc18
			 0x41496d4b, /// 0xc1c
			 0x8e543f28, /// 0xc20
			 0x0125c5b7, /// 0xc24
			 0x82df4885, /// 0xc28
			 0x10f43104, /// 0xc2c
			 0x228e969f, /// 0xc30
			 0xce36cb4e, /// 0xc34
			 0xab84c407, /// 0xc38
			 0xf76b2ee8, /// 0xc3c
			 0x256a9a74, /// 0xc40
			 0xb30c565d, /// 0xc44
			 0xafa16537, /// 0xc48
			 0xe4c6d4c6, /// 0xc4c
			 0x2d6dcdd1, /// 0xc50
			 0xfc1648a8, /// 0xc54
			 0x8167cc47, /// 0xc58
			 0x6754b812, /// 0xc5c
			 0xaae1451a, /// 0xc60
			 0x030d0076, /// 0xc64
			 0xf8ca40e3, /// 0xc68
			 0xa768a857, /// 0xc6c
			 0x212d70c5, /// 0xc70
			 0xeddbf08c, /// 0xc74
			 0x18f13d39, /// 0xc78
			 0x374605ba, /// 0xc7c
			 0x35b9af6d, /// 0xc80
			 0xb7729705, /// 0xc84
			 0xb6e0ab72, /// 0xc88
			 0x7eee367b, /// 0xc8c
			 0x04cad800, /// 0xc90
			 0xc8b06050, /// 0xc94
			 0x487ca91c, /// 0xc98
			 0x03309bba, /// 0xc9c
			 0xdc7e489e, /// 0xca0
			 0x5a6c61a7, /// 0xca4
			 0x70b0c384, /// 0xca8
			 0xff2ad5c1, /// 0xcac
			 0x37327f52, /// 0xcb0
			 0xe27174a7, /// 0xcb4
			 0x35e52fd6, /// 0xcb8
			 0x662cdf14, /// 0xcbc
			 0x1a844e64, /// 0xcc0
			 0x444dab1c, /// 0xcc4
			 0x655b8e78, /// 0xcc8
			 0xdf268fce, /// 0xccc
			 0x54f598b7, /// 0xcd0
			 0xde101871, /// 0xcd4
			 0xb60aeab0, /// 0xcd8
			 0xe30e2001, /// 0xcdc
			 0x4aac93c2, /// 0xce0
			 0xaa896ad8, /// 0xce4
			 0x8a7e3d66, /// 0xce8
			 0xfd0bc9ac, /// 0xcec
			 0xaef3a50e, /// 0xcf0
			 0xf368026b, /// 0xcf4
			 0xbc0dffc8, /// 0xcf8
			 0x23a37d6b, /// 0xcfc
			 0xc11cac79, /// 0xd00
			 0x3d9e2a81, /// 0xd04
			 0xd582c23b, /// 0xd08
			 0x1dda1dfe, /// 0xd0c
			 0x7214d82f, /// 0xd10
			 0x63582a4f, /// 0xd14
			 0xf28f1f0e, /// 0xd18
			 0x358e2afb, /// 0xd1c
			 0x9cadad76, /// 0xd20
			 0xff2c0554, /// 0xd24
			 0x8c2caaf7, /// 0xd28
			 0x031839aa, /// 0xd2c
			 0xab5be3ff, /// 0xd30
			 0x133bc3bb, /// 0xd34
			 0x1c81fb17, /// 0xd38
			 0xcfef04e8, /// 0xd3c
			 0x86f2217b, /// 0xd40
			 0x897346be, /// 0xd44
			 0x49065288, /// 0xd48
			 0x05b57a28, /// 0xd4c
			 0x5b35c575, /// 0xd50
			 0x26862b54, /// 0xd54
			 0x821a8363, /// 0xd58
			 0xe690c74f, /// 0xd5c
			 0xe962a6af, /// 0xd60
			 0x7913f472, /// 0xd64
			 0x257341cf, /// 0xd68
			 0xb2981598, /// 0xd6c
			 0x995a16f8, /// 0xd70
			 0xe8d82888, /// 0xd74
			 0xb69cf487, /// 0xd78
			 0x7f37c81b, /// 0xd7c
			 0x53536cbc, /// 0xd80
			 0xfd25417d, /// 0xd84
			 0x3f24ccbe, /// 0xd88
			 0x70f31a62, /// 0xd8c
			 0xc87f6ce4, /// 0xd90
			 0x6af0967b, /// 0xd94
			 0xea432811, /// 0xd98
			 0xdb29ee4f, /// 0xd9c
			 0x410e9334, /// 0xda0
			 0x1b231995, /// 0xda4
			 0xdc2dff32, /// 0xda8
			 0xc4bf0436, /// 0xdac
			 0x2f6c7abf, /// 0xdb0
			 0x407750f5, /// 0xdb4
			 0x7fe92916, /// 0xdb8
			 0xe5734b9a, /// 0xdbc
			 0x71499a61, /// 0xdc0
			 0xafd9ab95, /// 0xdc4
			 0xae928db3, /// 0xdc8
			 0x01f0dcc4, /// 0xdcc
			 0x486dcd0c, /// 0xdd0
			 0xa057a8d2, /// 0xdd4
			 0x00cb8c03, /// 0xdd8
			 0xd07d9f77, /// 0xddc
			 0x1e02f573, /// 0xde0
			 0xe1552531, /// 0xde4
			 0xc710dcc2, /// 0xde8
			 0xc4e5180e, /// 0xdec
			 0x32e1d7bc, /// 0xdf0
			 0xe1bb873a, /// 0xdf4
			 0x9636656d, /// 0xdf8
			 0x79a69988, /// 0xdfc
			 0x281d2bb7, /// 0xe00
			 0x66e7eb0e, /// 0xe04
			 0x7c8b34a7, /// 0xe08
			 0x221244eb, /// 0xe0c
			 0x36b10513, /// 0xe10
			 0xbbcfac5b, /// 0xe14
			 0xe3e37204, /// 0xe18
			 0x05e77d7f, /// 0xe1c
			 0x7c190f26, /// 0xe20
			 0x5744c3c7, /// 0xe24
			 0x8e753f38, /// 0xe28
			 0x262e8ff8, /// 0xe2c
			 0xdcea7503, /// 0xe30
			 0x0aa48d69, /// 0xe34
			 0x74c5ee60, /// 0xe38
			 0xaca439d6, /// 0xe3c
			 0xeb603955, /// 0xe40
			 0xdafa14bf, /// 0xe44
			 0x246d6324, /// 0xe48
			 0xa5825aa7, /// 0xe4c
			 0x212edd93, /// 0xe50
			 0x790a7ec4, /// 0xe54
			 0x7cddf51d, /// 0xe58
			 0x1de48e02, /// 0xe5c
			 0x0c878567, /// 0xe60
			 0x572abc1b, /// 0xe64
			 0x467f5e00, /// 0xe68
			 0xfc75184c, /// 0xe6c
			 0x691e25ba, /// 0xe70
			 0x656e923a, /// 0xe74
			 0x38f9929f, /// 0xe78
			 0xdaf19e64, /// 0xe7c
			 0x236b71bc, /// 0xe80
			 0x70bfbba2, /// 0xe84
			 0xf7602847, /// 0xe88
			 0x1b802c63, /// 0xe8c
			 0xe97905b3, /// 0xe90
			 0x23f47479, /// 0xe94
			 0xc4ec840c, /// 0xe98
			 0x270e4530, /// 0xe9c
			 0x70016d50, /// 0xea0
			 0x361dfd25, /// 0xea4
			 0x1f1d7e51, /// 0xea8
			 0x2a9a61f3, /// 0xeac
			 0xadb2fe4c, /// 0xeb0
			 0x1bf80d89, /// 0xeb4
			 0x0ca3e5df, /// 0xeb8
			 0x17361b58, /// 0xebc
			 0xaa50c964, /// 0xec0
			 0x9002a245, /// 0xec4
			 0xfbd8ebdb, /// 0xec8
			 0xb64bb09d, /// 0xecc
			 0x83c15683, /// 0xed0
			 0x30616211, /// 0xed4
			 0xd056187e, /// 0xed8
			 0xc93e1a31, /// 0xedc
			 0x78af0abb, /// 0xee0
			 0x017d4316, /// 0xee4
			 0xf11e7869, /// 0xee8
			 0x25600685, /// 0xeec
			 0x28e633c0, /// 0xef0
			 0xa7b2f16a, /// 0xef4
			 0x7b373248, /// 0xef8
			 0xe10a7162, /// 0xefc
			 0xe64b8e97, /// 0xf00
			 0x5c0dc99f, /// 0xf04
			 0xb1b0bdaa, /// 0xf08
			 0xa10dbd76, /// 0xf0c
			 0x91b5900e, /// 0xf10
			 0x562c69a6, /// 0xf14
			 0x9e112be6, /// 0xf18
			 0x0305df2c, /// 0xf1c
			 0x5c750927, /// 0xf20
			 0x25b07ffc, /// 0xf24
			 0x7d14f73e, /// 0xf28
			 0x7edc3d81, /// 0xf2c
			 0x71f50f8a, /// 0xf30
			 0x2d6a0852, /// 0xf34
			 0x693e8a25, /// 0xf38
			 0x2e30d997, /// 0xf3c
			 0xdd96dbcd, /// 0xf40
			 0x8d32523f, /// 0xf44
			 0x3b9b2ec9, /// 0xf48
			 0xd73c8fd1, /// 0xf4c
			 0x4cff941e, /// 0xf50
			 0x4b929d80, /// 0xf54
			 0x0aa98146, /// 0xf58
			 0x95474cbc, /// 0xf5c
			 0x1e395c86, /// 0xf60
			 0x9db1dab1, /// 0xf64
			 0x26153aeb, /// 0xf68
			 0x0f9a5cd4, /// 0xf6c
			 0xa8bb3971, /// 0xf70
			 0x36822f7a, /// 0xf74
			 0xb6215a21, /// 0xf78
			 0x13d454c4, /// 0xf7c
			 0x302e2cd0, /// 0xf80
			 0x2fe48927, /// 0xf84
			 0x821c537d, /// 0xf88
			 0x10713210, /// 0xf8c
			 0xb95d45fc, /// 0xf90
			 0x8762e0f7, /// 0xf94
			 0xdad61ee6, /// 0xf98
			 0x46bc8710, /// 0xf9c
			 0xe9cf5147, /// 0xfa0
			 0x975e96d6, /// 0xfa4
			 0x0589de78, /// 0xfa8
			 0x693c22eb, /// 0xfac
			 0x3047445a, /// 0xfb0
			 0x764dda58, /// 0xfb4
			 0x8d55f920, /// 0xfb8
			 0x89fc8fae, /// 0xfbc
			 0xf76258be, /// 0xfc0
			 0xcb49e088, /// 0xfc4
			 0x12867ba8, /// 0xfc8
			 0x656ca01c, /// 0xfcc
			 0x012c332a, /// 0xfd0
			 0x408fd301, /// 0xfd4
			 0x927444db, /// 0xfd8
			 0x43f0cf81, /// 0xfdc
			 0x49096507, /// 0xfe0
			 0xe2206ffa, /// 0xfe4
			 0xbbaef055, /// 0xfe8
			 0x3a489484, /// 0xfec
			 0x46c71e54, /// 0xff0
			 0x29ed227f, /// 0xff4
			 0x9058b92c, /// 0xff8
			 0x4a41a31e /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00000
			 0x55555555,                                                  // 4 random values                             /// 00004
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00008
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0000c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00014
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0001c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00024
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00028
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0002c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00030
			 0xffc00001,                                                  // -signaling NaN                              /// 00034
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0003c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00040
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0004c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00050
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00054
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0005c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00060
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00068
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0006c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00070
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00078
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0007c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0008c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00090
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00094
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00098
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0009c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000a0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000a8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000ac
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000b0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000b4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000b8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000c8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000d0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000d8
			 0x3f028f5c,                                                  // 0.51                                        /// 000dc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000e0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000e4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000e8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000ec
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000f0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000f4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000f8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00100
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00104
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00108
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0010c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00114
			 0x0c600000,                                                  // Leading 1s:                                 /// 00118
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0011c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00120
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0012c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00134
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x0c600000,                                                  // Leading 1s:                                 /// 0013c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00140
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0014c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0xbf028f5c,                                                  // -0.51                                       /// 00154
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00158
			 0x0c700000,                                                  // Leading 1s:                                 /// 0015c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00160
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00164
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00168
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0016c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00174
			 0x0e000007,                                                  // Trailing 1s:                                /// 00178
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00180
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00184
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00188
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0018c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00190
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00194
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00198
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001a0
			 0x0e000001,                                                  // Trailing 1s:                                /// 001a4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001ac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001bc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001c0
			 0x0c400000,                                                  // Leading 1s:                                 /// 001c4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c8
			 0x00000000,                                                  // zero                                        /// 001cc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001d0
			 0xbf028f5c,                                                  // -0.51                                       /// 001d4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001d8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0xcb000000,                                                  // -8388608.0                                  /// 001e0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001e8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001f0
			 0x0c400000,                                                  // Leading 1s:                                 /// 001f4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001f8
			 0xbf028f5c,                                                  // -0.51                                       /// 001fc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00200
			 0x0e000003,                                                  // Trailing 1s:                                /// 00204
			 0xffc00001,                                                  // -signaling NaN                              /// 00208
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0020c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00210
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00214
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00218
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0021c
			 0xffc00001,                                                  // -signaling NaN                              /// 00220
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00224
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00228
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00230
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00240
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x0c600000,                                                  // Leading 1s:                                 /// 00248
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0024c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00250
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00254
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00258
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00260
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00264
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00268
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00278
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0027c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00280
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00284
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00288
			 0x3f028f5c,                                                  // 0.51                                        /// 0028c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0029c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002a0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002a4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002a8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002ac
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002b0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002b4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002b8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002bc
			 0x00011111,                                                  // 9.7958E-41                                  /// 002c0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002c4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002d0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x55555555,                                                  // 4 random values                             /// 002d8
			 0x0c600000,                                                  // Leading 1s:                                 /// 002dc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002e0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002e4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002e8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002ec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002f0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002f4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002f8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002fc
			 0xffc00001,                                                  // -signaling NaN                              /// 00300
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00304
			 0x0e000007,                                                  // Trailing 1s:                                /// 00308
			 0x7fc00001,                                                  // signaling NaN                               /// 0030c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00310
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00318
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0031c
			 0x33333333,                                                  // 4 random values                             /// 00320
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00324
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00328
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00334
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0033c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00344
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00348
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00350
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00354
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0035c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0036c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00370
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00374
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0037c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00380
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00384
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00388
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0038c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00398
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0039c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003a0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x00011111,                                                  // 9.7958E-41                                  /// 003ac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003b0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003b8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003c0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003c8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 003cc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003d0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003d4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003d8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003e8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003ec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003f0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003f8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003fc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00400
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00404
			 0x00011111,                                                  // 9.7958E-41                                  /// 00408
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0040c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00414
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00418
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0041c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00420
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00424
			 0x00011111,                                                  // 9.7958E-41                                  /// 00428
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00434
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00438
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0043c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00440
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00444
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00450
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00454
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00458
			 0x33333333,                                                  // 4 random values                             /// 0045c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00460
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00464
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0046c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00470
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00474
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00478
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0047c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0xbf028f5c,                                                  // -0.51                                       /// 00484
			 0x80000000,                                                  // -zero                                       /// 00488
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0048c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00490
			 0xbf028f5c,                                                  // -0.51                                       /// 00494
			 0x7f800000,                                                  // inf                                         /// 00498
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0049c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004a0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004a4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004a8
			 0x7fc00001,                                                  // signaling NaN                               /// 004ac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004b0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004b4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004b8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004bc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004c0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004c4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004c8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004cc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004d0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004d4
			 0x80000000,                                                  // -zero                                       /// 004d8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004e0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004e8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 004f8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004fc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x0c780000,                                                  // Leading 1s:                                 /// 00504
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x0c600000,                                                  // Leading 1s:                                 /// 0050c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00510
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00514
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0051c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00520
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00524
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0052c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00530
			 0xffc00001,                                                  // -signaling NaN                              /// 00534
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00538
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0053c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00548
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0054c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00558
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0055c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x0e000007,                                                  // Trailing 1s:                                /// 00568
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00570
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00574
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00578
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0057c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00580
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00584
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00588
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0058c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00590
			 0x0e000001,                                                  // Trailing 1s:                                /// 00594
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00598
			 0x3f028f5c,                                                  // 0.51                                        /// 0059c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005a0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005a8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005ac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005b0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005b4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005b8
			 0x0c700000,                                                  // Leading 1s:                                 /// 005bc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005c4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005c8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 005cc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005d0
			 0xbf028f5c,                                                  // -0.51                                       /// 005d4
			 0xbf028f5c,                                                  // -0.51                                       /// 005d8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005dc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005e4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005e8
			 0x0c700000,                                                  // Leading 1s:                                 /// 005ec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005f4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005f8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005fc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00600
			 0x0e000003,                                                  // Trailing 1s:                                /// 00604
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00608
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00610
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00614
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0061c
			 0x55555555,                                                  // 4 random values                             /// 00620
			 0x4b000000,                                                  // 8388608.0                                   /// 00624
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00628
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0062c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00630
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00638
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0063c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00640
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00644
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00648
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0064c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00650
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00654
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00658
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0065c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00660
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00664
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00668
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0066c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00670
			 0x0c700000,                                                  // Leading 1s:                                 /// 00674
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0067c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0068c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00690
			 0x0c780000,                                                  // Leading 1s:                                 /// 00694
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00698
			 0xff800000,                                                  // -inf                                        /// 0069c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006a4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006a8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006b0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006b4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006b8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006c0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006c4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006c8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006cc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006d0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006d8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006dc
			 0x7f800000,                                                  // inf                                         /// 006e0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006e4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006e8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006f0
			 0x55555555,                                                  // 4 random values                             /// 006f4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006f8
			 0xcb000000,                                                  // -8388608.0                                  /// 006fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00700
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00708
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0070c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00710
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00718
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0071c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00720
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00728
			 0x33333333,                                                  // 4 random values                             /// 0072c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00730
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00734
			 0x00000000,                                                  // zero                                        /// 00738
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0073c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00740
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00744
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00748
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0074c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00750
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00754
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00758
			 0x0c400000,                                                  // Leading 1s:                                 /// 0075c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00760
			 0x55555555,                                                  // 4 random values                             /// 00764
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0076c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00774
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00778
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0077c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00780
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00784
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00788
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0078c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x3f028f5c,                                                  // 0.51                                        /// 0079c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007a0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x7f800000,                                                  // inf                                         /// 007a8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007ac
			 0x0e000007,                                                  // Trailing 1s:                                /// 007b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007b4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007b8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007bc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007c0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007c8
			 0x80011111,                                                  // -9.7958E-41                                 /// 007cc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007d0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007d4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007d8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007dc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007e4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007ec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007f0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007f4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00800
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00804
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00808
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0080c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00814
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00818
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0081c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0082c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00830
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00838
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0083c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00840
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00848
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0084c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00850
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0085c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00860
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00864
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00868
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0086c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00870
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00874
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00880
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00884
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0088c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00890
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00894
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x3f028f5c,                                                  // 0.51                                        /// 0089c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008a0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008a4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008a8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008ac
			 0x00011111,                                                  // 9.7958E-41                                  /// 008b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008b8
			 0x00000000,                                                  // zero                                        /// 008bc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008c0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x55555555,                                                  // 4 random values                             /// 008c8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008cc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008d8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008e4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008e8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008f0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008f4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008fc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00900
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00908
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0090c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00910
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0091c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00928
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00930
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00934
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00938
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0093c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00940
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0094c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00954
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00958
			 0x0e000001,                                                  // Trailing 1s:                                /// 0095c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00960
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00964
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0096c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00978
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00980
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x0e000007,                                                  // Trailing 1s:                                /// 00988
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00990
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00994
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00998
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0099c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009a0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009a4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009a8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009b4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009b8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009bc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009c0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009c4
			 0x4b000000,                                                  // 8388608.0                                   /// 009c8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009cc
			 0x0e000007,                                                  // Trailing 1s:                                /// 009d0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009dc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x3f028f5c,                                                  // 0.51                                        /// 009e4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009e8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009f0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009f4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009f8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009fc
			 0x4b000000,                                                  // 8388608.0                                   /// 00a00
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a04
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a08
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0xcb000000,                                                  // -8388608.0                                  /// 00a18
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a1c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a20
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a24
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a2c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a34
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a3c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a44
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a48
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a4c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a50
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a5c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a60
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a64
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a68
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a70
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a78
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a7c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a80
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a84
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a8c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a90
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a94
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a98
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a9c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00aa0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00aa4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00aa8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ab0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ab4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x0e000001,                                                  // Trailing 1s:                                /// 00abc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ac0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ac4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00acc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ad0
			 0x33333333,                                                  // 4 random values                             /// 00ad4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ad8
			 0x7fc00001,                                                  // signaling NaN                               /// 00adc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ae8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00aec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00af0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00af4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00afc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b00
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b04
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b08
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b10
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b14
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b18
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b1c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b24
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b28
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b2c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b30
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b34
			 0x33333333,                                                  // 4 random values                             /// 00b38
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b3c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b40
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b44
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b4c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b50
			 0x4b000000,                                                  // 8388608.0                                   /// 00b54
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b5c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b64
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b70
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b74
			 0xffc00001,                                                  // -signaling NaN                              /// 00b78
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b80
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b88
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00b8c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b90
			 0xcb000000,                                                  // -8388608.0                                  /// 00b94
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b9c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ba4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ba8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bb0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bb4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bb8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bbc
			 0x7f800000,                                                  // inf                                         /// 00bc0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bc4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00bc8
			 0xcb000000,                                                  // -8388608.0                                  /// 00bcc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bd0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00bd4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bd8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bdc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00be0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00be4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00be8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bf0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bf4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bf8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bfc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c00
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c04
			 0x7fc00001,                                                  // signaling NaN                               /// 00c08
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c0c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c10
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c20
			 0x33333333,                                                  // 4 random values                             /// 00c24
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c28
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c30
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c34
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c38
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c3c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c44
			 0xcb000000,                                                  // -8388608.0                                  /// 00c48
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c4c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c50
			 0x7f800000,                                                  // inf                                         /// 00c54
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c58
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c5c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c60
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c64
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c74
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c78
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c7c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c88
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c8c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c90
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c94
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c98
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ca0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ca4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cb0
			 0x7f800000,                                                  // inf                                         /// 00cb4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00cb8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cbc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cc0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cc8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ccc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cd0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00cd4
			 0x3f028f5c,                                                  // 0.51                                        /// 00cd8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cdc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ce0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cf0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cf8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cfc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d00
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0xcb000000,                                                  // -8388608.0                                  /// 00d08
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d10
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d14
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d18
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d20
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d28
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d2c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d30
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d34
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d38
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d3c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d40
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d44
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d48
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d4c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d5c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x7f800000,                                                  // inf                                         /// 00d70
			 0x3f028f5c,                                                  // 0.51                                        /// 00d74
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d78
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d7c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d80
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d84
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d88
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d8c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d90
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d94
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d98
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d9c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00da0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00dac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00db0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00db8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dc0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dc4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dcc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dd0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00dd4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00dd8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ddc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00de4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00de8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00dec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00df0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00df4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00df8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00dfc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e00
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e04
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e08
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e0c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e10
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x7f800000,                                                  // inf                                         /// 00e18
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e1c
			 0x33333333,                                                  // 4 random values                             /// 00e20
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e24
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e28
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e2c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e34
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e38
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e48
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e50
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e54
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e58
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e5c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e60
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e64
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e68
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e6c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e74
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e78
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e80
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e88
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e8c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e90
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e94
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ea0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ea8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00eac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00eb0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00eb4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ebc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ec0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ec8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ecc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ed0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00edc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ee0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ee4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ee8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00eec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ef4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x33333333,                                                  // 4 random values                             /// 00efc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f00
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f04
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f08
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f0c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f14
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f18
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f1c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f20
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f24
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f28
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f2c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f30
			 0x00000000,                                                  // zero                                        /// 00f34
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f38
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f40
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f50
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f54
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f5c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f64
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f68
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f70
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f74
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f78
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f7c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f80
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f84
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f88
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f8c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f94
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f9c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fa0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fa8
			 0x3f028f5c,                                                  // 0.51                                        /// 00fac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fb0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fb8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fbc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fc0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fc8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fd0
			 0xbf028f5c,                                                  // -0.51                                       /// 00fd4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fd8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fdc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fe0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fe4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fe8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ff0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ff4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ff8
			 0x0c7ffff8                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x1f71c721, /// 0x0
			 0x26891ec8, /// 0x4
			 0x44481c15, /// 0x8
			 0x136e244e, /// 0xc
			 0x2a9d14ef, /// 0x10
			 0x94c47e68, /// 0x14
			 0x4ce29cce, /// 0x18
			 0x3d167ac8, /// 0x1c
			 0xef0b4baf, /// 0x20
			 0x6d2c31b9, /// 0x24
			 0x7012573b, /// 0x28
			 0x31b5451b, /// 0x2c
			 0x45376727, /// 0x30
			 0xf43971d3, /// 0x34
			 0x0264e36a, /// 0x38
			 0x2a181802, /// 0x3c
			 0x39b84335, /// 0x40
			 0x56c25b5e, /// 0x44
			 0x4ee827f3, /// 0x48
			 0x8cacb909, /// 0x4c
			 0xd6bbb685, /// 0x50
			 0x933729a0, /// 0x54
			 0x26a1280e, /// 0x58
			 0x9673b1f4, /// 0x5c
			 0xc921e51c, /// 0x60
			 0xc2aba97c, /// 0x64
			 0x4bc7802e, /// 0x68
			 0xe8a08c97, /// 0x6c
			 0x12244300, /// 0x70
			 0x702de961, /// 0x74
			 0xee1bc8cc, /// 0x78
			 0xbadd96aa, /// 0x7c
			 0x62103641, /// 0x80
			 0x084aed52, /// 0x84
			 0x7edcf88a, /// 0x88
			 0x30ef754e, /// 0x8c
			 0xeffb9b55, /// 0x90
			 0xffef27e9, /// 0x94
			 0xbadef92c, /// 0x98
			 0x0f5d7d74, /// 0x9c
			 0x222df55b, /// 0xa0
			 0x8d260c6b, /// 0xa4
			 0x0080f13a, /// 0xa8
			 0xb1a106b1, /// 0xac
			 0x3f9ba24b, /// 0xb0
			 0xad890f64, /// 0xb4
			 0xd8f7ce37, /// 0xb8
			 0xceff2257, /// 0xbc
			 0x837c159f, /// 0xc0
			 0x7f498cd9, /// 0xc4
			 0xeaedfc9f, /// 0xc8
			 0x7900a975, /// 0xcc
			 0x999c3b74, /// 0xd0
			 0x27a66784, /// 0xd4
			 0x5acee774, /// 0xd8
			 0x2173c99a, /// 0xdc
			 0x4540d000, /// 0xe0
			 0xcf8e7bbe, /// 0xe4
			 0x9fab7247, /// 0xe8
			 0x3aa0fec3, /// 0xec
			 0x0b8d8588, /// 0xf0
			 0xa53b02af, /// 0xf4
			 0x5cefd228, /// 0xf8
			 0x3bff4000, /// 0xfc
			 0x7b10660d, /// 0x100
			 0x3e48e1e0, /// 0x104
			 0x37963e2c, /// 0x108
			 0x9252014c, /// 0x10c
			 0x39b3fdab, /// 0x110
			 0xf099b908, /// 0x114
			 0xfb8d04b0, /// 0x118
			 0xaf8c4a76, /// 0x11c
			 0x6f9742b2, /// 0x120
			 0x5babb41e, /// 0x124
			 0xfe41cfed, /// 0x128
			 0xed776fd4, /// 0x12c
			 0xb4f57061, /// 0x130
			 0xb360e0b8, /// 0x134
			 0x22189f17, /// 0x138
			 0x5c390338, /// 0x13c
			 0x3014065f, /// 0x140
			 0xee759664, /// 0x144
			 0xaead14da, /// 0x148
			 0x75e857f0, /// 0x14c
			 0x4bd8ec00, /// 0x150
			 0xe11a2c21, /// 0x154
			 0xac8b3d83, /// 0x158
			 0x3db67523, /// 0x15c
			 0x0d787fa2, /// 0x160
			 0x66353724, /// 0x164
			 0xcd4a5f14, /// 0x168
			 0xec690cbe, /// 0x16c
			 0xa8f6efd1, /// 0x170
			 0x3492580c, /// 0x174
			 0x9bb89a28, /// 0x178
			 0x91a2499f, /// 0x17c
			 0xd28d8810, /// 0x180
			 0x91726132, /// 0x184
			 0x577f51e1, /// 0x188
			 0xfa3792ac, /// 0x18c
			 0x831e9327, /// 0x190
			 0x601c6faf, /// 0x194
			 0x8d2f5ea1, /// 0x198
			 0xdcc288c8, /// 0x19c
			 0xfc4515e2, /// 0x1a0
			 0xdf5dfcbc, /// 0x1a4
			 0xe41efad7, /// 0x1a8
			 0x7a854b7c, /// 0x1ac
			 0x470c0565, /// 0x1b0
			 0xf18c97ae, /// 0x1b4
			 0x057d0cf7, /// 0x1b8
			 0xe56a97cb, /// 0x1bc
			 0x6d0ab872, /// 0x1c0
			 0xbc28795f, /// 0x1c4
			 0xdaf70d5d, /// 0x1c8
			 0x50271117, /// 0x1cc
			 0x63721c76, /// 0x1d0
			 0xc1d27473, /// 0x1d4
			 0xc66ef94b, /// 0x1d8
			 0xbd557833, /// 0x1dc
			 0x0777cb9d, /// 0x1e0
			 0xb987bad9, /// 0x1e4
			 0x15c335dc, /// 0x1e8
			 0x1c644cec, /// 0x1ec
			 0xa43ab0b3, /// 0x1f0
			 0x1a50f2ae, /// 0x1f4
			 0xd198568f, /// 0x1f8
			 0x0dc15c64, /// 0x1fc
			 0xa701dbfa, /// 0x200
			 0xaece1ceb, /// 0x204
			 0xb02d03db, /// 0x208
			 0x7405b946, /// 0x20c
			 0xd9de3fb2, /// 0x210
			 0xb2a8301d, /// 0x214
			 0x2ca8889c, /// 0x218
			 0x507dc078, /// 0x21c
			 0x3bd0913b, /// 0x220
			 0xcf8eb532, /// 0x224
			 0x966faa2b, /// 0x228
			 0x017ad619, /// 0x22c
			 0x63bfe3aa, /// 0x230
			 0xc23952e0, /// 0x234
			 0x7140e563, /// 0x238
			 0xe5fc9836, /// 0x23c
			 0x9585883e, /// 0x240
			 0xb65f8c85, /// 0x244
			 0x0179f15e, /// 0x248
			 0xb0dc39a9, /// 0x24c
			 0x31859d25, /// 0x250
			 0x0707b021, /// 0x254
			 0x33be104b, /// 0x258
			 0x49d9cc73, /// 0x25c
			 0x8d727df1, /// 0x260
			 0x2466707e, /// 0x264
			 0x593b1e61, /// 0x268
			 0xabbc26a5, /// 0x26c
			 0x35bfaf44, /// 0x270
			 0xba234438, /// 0x274
			 0xad0e970f, /// 0x278
			 0xde4ca73d, /// 0x27c
			 0x09190703, /// 0x280
			 0xa5c15a8a, /// 0x284
			 0xee841c55, /// 0x288
			 0x9c8c30ba, /// 0x28c
			 0xe69ae1b2, /// 0x290
			 0x67135d7d, /// 0x294
			 0x8d14c1cd, /// 0x298
			 0xf9f64254, /// 0x29c
			 0x2391acca, /// 0x2a0
			 0x53929ed1, /// 0x2a4
			 0x1155548b, /// 0x2a8
			 0x7db9633e, /// 0x2ac
			 0x2080aa0f, /// 0x2b0
			 0xc84d618a, /// 0x2b4
			 0x8e682d42, /// 0x2b8
			 0x185979ce, /// 0x2bc
			 0xa6812497, /// 0x2c0
			 0x6b1442ab, /// 0x2c4
			 0xf7b5e8c6, /// 0x2c8
			 0x3d66cc04, /// 0x2cc
			 0x9a16d30d, /// 0x2d0
			 0x29740a60, /// 0x2d4
			 0x2fc30787, /// 0x2d8
			 0x69371fe4, /// 0x2dc
			 0xebf4588d, /// 0x2e0
			 0xe5e17ee3, /// 0x2e4
			 0x37af2dc6, /// 0x2e8
			 0x1643838c, /// 0x2ec
			 0x0a68f71e, /// 0x2f0
			 0x0362d81e, /// 0x2f4
			 0x33b29324, /// 0x2f8
			 0xd77dc844, /// 0x2fc
			 0xb74de70a, /// 0x300
			 0x2f7891d7, /// 0x304
			 0x96e3ebe7, /// 0x308
			 0x1fc5fbca, /// 0x30c
			 0xba505006, /// 0x310
			 0xbaba6e59, /// 0x314
			 0xa6869499, /// 0x318
			 0x95c297bf, /// 0x31c
			 0x64e2b2a0, /// 0x320
			 0xcd4454f2, /// 0x324
			 0xd98acb2a, /// 0x328
			 0x855d2f8a, /// 0x32c
			 0x5556e53c, /// 0x330
			 0x9bf46a9a, /// 0x334
			 0xa21c7e29, /// 0x338
			 0x5bebf423, /// 0x33c
			 0xdd2fe610, /// 0x340
			 0x8542f9fa, /// 0x344
			 0x0b1f8f54, /// 0x348
			 0xd7132d57, /// 0x34c
			 0x618afc0f, /// 0x350
			 0xacf95813, /// 0x354
			 0x806b3e77, /// 0x358
			 0xe91a8c11, /// 0x35c
			 0x593d40aa, /// 0x360
			 0xfd45f821, /// 0x364
			 0x92f65828, /// 0x368
			 0x750b68ef, /// 0x36c
			 0x6f49fca4, /// 0x370
			 0x6d453557, /// 0x374
			 0x26782a5b, /// 0x378
			 0xb18fc4a5, /// 0x37c
			 0xa292f21a, /// 0x380
			 0x3fa67cce, /// 0x384
			 0x92adbf5b, /// 0x388
			 0xde3f1fe4, /// 0x38c
			 0x7518eb3f, /// 0x390
			 0xe3a968db, /// 0x394
			 0xd10dd104, /// 0x398
			 0x81646337, /// 0x39c
			 0x93a6db01, /// 0x3a0
			 0x0e6f1a54, /// 0x3a4
			 0x69f9be63, /// 0x3a8
			 0xf4389152, /// 0x3ac
			 0x2b3b880c, /// 0x3b0
			 0x7242fe2b, /// 0x3b4
			 0x1263bf54, /// 0x3b8
			 0xd84bd7ae, /// 0x3bc
			 0x0f811243, /// 0x3c0
			 0xb83904a4, /// 0x3c4
			 0x6d375d28, /// 0x3c8
			 0xec942849, /// 0x3cc
			 0xc77eb8f2, /// 0x3d0
			 0x4b4078fd, /// 0x3d4
			 0x94f6cb6a, /// 0x3d8
			 0x1860e528, /// 0x3dc
			 0x3e086154, /// 0x3e0
			 0x129bb9e0, /// 0x3e4
			 0x0487b07d, /// 0x3e8
			 0x6456c148, /// 0x3ec
			 0xfafd196a, /// 0x3f0
			 0x0103955d, /// 0x3f4
			 0x7ac8846f, /// 0x3f8
			 0x3cbca607, /// 0x3fc
			 0x0dbe8b0d, /// 0x400
			 0x80930eb2, /// 0x404
			 0x1d99040a, /// 0x408
			 0x93eb587d, /// 0x40c
			 0x00ee0bab, /// 0x410
			 0x43c42ca2, /// 0x414
			 0x7d47107b, /// 0x418
			 0xead7495e, /// 0x41c
			 0x041b5d4c, /// 0x420
			 0x33e8f6d5, /// 0x424
			 0xbba08567, /// 0x428
			 0xbe129986, /// 0x42c
			 0xe69be52d, /// 0x430
			 0xa620bcfe, /// 0x434
			 0xa305a074, /// 0x438
			 0x8283f64f, /// 0x43c
			 0xf1a83174, /// 0x440
			 0x314269a3, /// 0x444
			 0x912f24c5, /// 0x448
			 0x514cf846, /// 0x44c
			 0xf187dc42, /// 0x450
			 0x32cf27d2, /// 0x454
			 0x5f0caa3d, /// 0x458
			 0xb7b9bc29, /// 0x45c
			 0x2f73d455, /// 0x460
			 0x8f6f389b, /// 0x464
			 0x78ec6bf8, /// 0x468
			 0x73f9a30b, /// 0x46c
			 0xf69d936c, /// 0x470
			 0x8d3b399d, /// 0x474
			 0x465a9604, /// 0x478
			 0x21ba0769, /// 0x47c
			 0x710b2825, /// 0x480
			 0x486c2cb4, /// 0x484
			 0xc26cb511, /// 0x488
			 0x597c4cb9, /// 0x48c
			 0x4d61980c, /// 0x490
			 0xff8ae488, /// 0x494
			 0x69d04dbf, /// 0x498
			 0x7b381215, /// 0x49c
			 0x951b6173, /// 0x4a0
			 0x43aa9443, /// 0x4a4
			 0xfe0ffce3, /// 0x4a8
			 0xed03a625, /// 0x4ac
			 0x1512b64c, /// 0x4b0
			 0x1fc81616, /// 0x4b4
			 0x4e0c780b, /// 0x4b8
			 0x4b634d25, /// 0x4bc
			 0x386caa6b, /// 0x4c0
			 0x9995e7de, /// 0x4c4
			 0xe1e42f92, /// 0x4c8
			 0x13296639, /// 0x4cc
			 0xe5decf16, /// 0x4d0
			 0xd92b6ea9, /// 0x4d4
			 0x1c2b3342, /// 0x4d8
			 0x8df3215f, /// 0x4dc
			 0x540cafa4, /// 0x4e0
			 0x749b5e07, /// 0x4e4
			 0xf87e4d9d, /// 0x4e8
			 0x6ca36986, /// 0x4ec
			 0x39a33b27, /// 0x4f0
			 0x6c574467, /// 0x4f4
			 0xf4bd3a65, /// 0x4f8
			 0x6098b113, /// 0x4fc
			 0xca75ba3c, /// 0x500
			 0x9e6cd193, /// 0x504
			 0x0b7f4635, /// 0x508
			 0xa4958935, /// 0x50c
			 0x82eb1df1, /// 0x510
			 0x5a65f2b2, /// 0x514
			 0x5d4f47e1, /// 0x518
			 0x35180bc1, /// 0x51c
			 0xd7dd625f, /// 0x520
			 0xeea364b1, /// 0x524
			 0x33a22e2f, /// 0x528
			 0xfe7720ea, /// 0x52c
			 0xbb6043e5, /// 0x530
			 0xf455452c, /// 0x534
			 0x09487816, /// 0x538
			 0xa3d8589b, /// 0x53c
			 0x89f9c3e1, /// 0x540
			 0x06cc4802, /// 0x544
			 0x8c881bdd, /// 0x548
			 0x6a0ab7ab, /// 0x54c
			 0xa2292b22, /// 0x550
			 0xe0d51989, /// 0x554
			 0x40216090, /// 0x558
			 0xdbe297ca, /// 0x55c
			 0xb5bf0eee, /// 0x560
			 0x0b033a9f, /// 0x564
			 0xab7e5759, /// 0x568
			 0x0d68113f, /// 0x56c
			 0x828d7a69, /// 0x570
			 0xea34dc7c, /// 0x574
			 0xf83f902a, /// 0x578
			 0x9034d6b7, /// 0x57c
			 0xdac8d8a2, /// 0x580
			 0xdf07a929, /// 0x584
			 0x3248e1ec, /// 0x588
			 0x02edcf98, /// 0x58c
			 0x6aa89c73, /// 0x590
			 0x387b5a1d, /// 0x594
			 0x0f31c577, /// 0x598
			 0x56f2bc1a, /// 0x59c
			 0x4a036549, /// 0x5a0
			 0x26e82b5d, /// 0x5a4
			 0xcd251c40, /// 0x5a8
			 0x1d72169d, /// 0x5ac
			 0x5a1f4873, /// 0x5b0
			 0xc5d05e85, /// 0x5b4
			 0x35dc87a2, /// 0x5b8
			 0xdeb3e959, /// 0x5bc
			 0x99278bb0, /// 0x5c0
			 0xbe5cdab8, /// 0x5c4
			 0x595605d8, /// 0x5c8
			 0x35d8d8eb, /// 0x5cc
			 0x8cb9eea3, /// 0x5d0
			 0x8aa9c88a, /// 0x5d4
			 0xd70f77ce, /// 0x5d8
			 0xd009a372, /// 0x5dc
			 0x0d46cef7, /// 0x5e0
			 0x2909e226, /// 0x5e4
			 0x26561f49, /// 0x5e8
			 0x730706e5, /// 0x5ec
			 0x405fdf58, /// 0x5f0
			 0xccf86209, /// 0x5f4
			 0x98d74455, /// 0x5f8
			 0x7d47903c, /// 0x5fc
			 0xcb2a462e, /// 0x600
			 0xa36bd661, /// 0x604
			 0xcfd5f006, /// 0x608
			 0xad7511fc, /// 0x60c
			 0x0ed8f646, /// 0x610
			 0x4e43a70f, /// 0x614
			 0x643b1733, /// 0x618
			 0xe1f10ac6, /// 0x61c
			 0x51e1e2da, /// 0x620
			 0x1afa3bb4, /// 0x624
			 0x43dc7c92, /// 0x628
			 0x3237ef14, /// 0x62c
			 0xfd5d9c5f, /// 0x630
			 0x7d2a4a54, /// 0x634
			 0x527d9904, /// 0x638
			 0xc48da6c2, /// 0x63c
			 0xcd7a919e, /// 0x640
			 0xcd696760, /// 0x644
			 0x63dd21c8, /// 0x648
			 0xf6f2e6bb, /// 0x64c
			 0x663c4676, /// 0x650
			 0x25e2ebe5, /// 0x654
			 0x25112205, /// 0x658
			 0x49ec127b, /// 0x65c
			 0xe6891984, /// 0x660
			 0x3b9f105d, /// 0x664
			 0x34cf996f, /// 0x668
			 0x923135b4, /// 0x66c
			 0xb9b13646, /// 0x670
			 0xdfb35590, /// 0x674
			 0x9da2b981, /// 0x678
			 0x11cb2e27, /// 0x67c
			 0x0bcdfe05, /// 0x680
			 0x09a8260f, /// 0x684
			 0x4056a922, /// 0x688
			 0xc9969a31, /// 0x68c
			 0x5c92ef8e, /// 0x690
			 0xdbf05bda, /// 0x694
			 0xebe134c2, /// 0x698
			 0x3bfe315c, /// 0x69c
			 0x37d77e73, /// 0x6a0
			 0x7ce70264, /// 0x6a4
			 0x30dd4e61, /// 0x6a8
			 0xdef3087a, /// 0x6ac
			 0xe48dfa49, /// 0x6b0
			 0xae441f53, /// 0x6b4
			 0x9c0ca808, /// 0x6b8
			 0x13d197b4, /// 0x6bc
			 0x4c05dfa8, /// 0x6c0
			 0x95b577e4, /// 0x6c4
			 0xe559b9ad, /// 0x6c8
			 0x0d606d04, /// 0x6cc
			 0xab29312b, /// 0x6d0
			 0xd8dfda63, /// 0x6d4
			 0xb4e51208, /// 0x6d8
			 0x57bfc2bc, /// 0x6dc
			 0x47966373, /// 0x6e0
			 0x3ea9be57, /// 0x6e4
			 0x954c8ad9, /// 0x6e8
			 0xce47a44b, /// 0x6ec
			 0x49dd86fd, /// 0x6f0
			 0x740a9382, /// 0x6f4
			 0xce44390f, /// 0x6f8
			 0xf8f0539a, /// 0x6fc
			 0xfffc93ab, /// 0x700
			 0xc3e0d5fe, /// 0x704
			 0x9ddf0ecd, /// 0x708
			 0x12b69f1c, /// 0x70c
			 0xcfa37124, /// 0x710
			 0xbd1a7cbc, /// 0x714
			 0x0a9949f1, /// 0x718
			 0xceb59b64, /// 0x71c
			 0x74cfd2dc, /// 0x720
			 0xafca1d6d, /// 0x724
			 0x5510bc7e, /// 0x728
			 0xdd08d534, /// 0x72c
			 0xfbb80ee6, /// 0x730
			 0x09348f69, /// 0x734
			 0x2d3e696e, /// 0x738
			 0xf3726842, /// 0x73c
			 0x56d8be92, /// 0x740
			 0xab855a31, /// 0x744
			 0x3eb9150e, /// 0x748
			 0x1d7917d7, /// 0x74c
			 0x7d05f35c, /// 0x750
			 0xd64e56cb, /// 0x754
			 0x13e78389, /// 0x758
			 0xe1eb7e14, /// 0x75c
			 0xa7de36ba, /// 0x760
			 0x4ed3ad3e, /// 0x764
			 0x27ac4479, /// 0x768
			 0x15c696f3, /// 0x76c
			 0xdbac7132, /// 0x770
			 0x53504678, /// 0x774
			 0xb888ee28, /// 0x778
			 0x6c3270ec, /// 0x77c
			 0x74c2ed42, /// 0x780
			 0xd5398cd5, /// 0x784
			 0x1d22ce8a, /// 0x788
			 0xa6ec1071, /// 0x78c
			 0xdfb55222, /// 0x790
			 0xefc74a9c, /// 0x794
			 0xaa972231, /// 0x798
			 0x5a35d75d, /// 0x79c
			 0xd3cc57fa, /// 0x7a0
			 0x9d4fcfa2, /// 0x7a4
			 0x279095e4, /// 0x7a8
			 0x821705f3, /// 0x7ac
			 0x61f0d088, /// 0x7b0
			 0xfa7553ae, /// 0x7b4
			 0x21fc3360, /// 0x7b8
			 0xeb9c84c9, /// 0x7bc
			 0xfe5f66c4, /// 0x7c0
			 0xa5d10c57, /// 0x7c4
			 0x39df6c78, /// 0x7c8
			 0xab2ff166, /// 0x7cc
			 0x06ebe015, /// 0x7d0
			 0x397bbe0d, /// 0x7d4
			 0xc8b9c498, /// 0x7d8
			 0xb1982e76, /// 0x7dc
			 0x75ac25fc, /// 0x7e0
			 0x2946e01f, /// 0x7e4
			 0x75a81964, /// 0x7e8
			 0x05929b2a, /// 0x7ec
			 0x5c00c079, /// 0x7f0
			 0xe65d7b38, /// 0x7f4
			 0x8c7233a8, /// 0x7f8
			 0x443057eb, /// 0x7fc
			 0xa43a5b3e, /// 0x800
			 0x5d8511ca, /// 0x804
			 0xbfcda5eb, /// 0x808
			 0x96d05f18, /// 0x80c
			 0xa4de405c, /// 0x810
			 0x5589cfda, /// 0x814
			 0x354e31eb, /// 0x818
			 0x0ba9a31a, /// 0x81c
			 0x3fa4e65e, /// 0x820
			 0x8b547490, /// 0x824
			 0x441398e0, /// 0x828
			 0xcef5c90f, /// 0x82c
			 0x7d211830, /// 0x830
			 0x04e7562c, /// 0x834
			 0x1b06822b, /// 0x838
			 0x9ff37df2, /// 0x83c
			 0xb9714bb0, /// 0x840
			 0x8eafeca8, /// 0x844
			 0x44349a9c, /// 0x848
			 0x7e3d7f07, /// 0x84c
			 0x2c46eab2, /// 0x850
			 0xe0ba1ece, /// 0x854
			 0x73d0979a, /// 0x858
			 0xb9430322, /// 0x85c
			 0x87d3fe75, /// 0x860
			 0x655648a8, /// 0x864
			 0xf857a8c3, /// 0x868
			 0xf8094a15, /// 0x86c
			 0x103e2328, /// 0x870
			 0xeee762b6, /// 0x874
			 0x10cc1944, /// 0x878
			 0x81e9075c, /// 0x87c
			 0x201ea867, /// 0x880
			 0x449e09c3, /// 0x884
			 0x067634fd, /// 0x888
			 0xec1cfd9e, /// 0x88c
			 0x01614d7f, /// 0x890
			 0x9f4cb830, /// 0x894
			 0x83984030, /// 0x898
			 0xa1018b3c, /// 0x89c
			 0xe7e29802, /// 0x8a0
			 0x85f9caf5, /// 0x8a4
			 0xc60ea69a, /// 0x8a8
			 0x01d2ce8e, /// 0x8ac
			 0xe9ffe876, /// 0x8b0
			 0xccc2f720, /// 0x8b4
			 0xaef7c04c, /// 0x8b8
			 0x2983d282, /// 0x8bc
			 0x0ef11363, /// 0x8c0
			 0x262653f7, /// 0x8c4
			 0xc9997c78, /// 0x8c8
			 0x30999f55, /// 0x8cc
			 0x10dd8809, /// 0x8d0
			 0x8c35c75c, /// 0x8d4
			 0x77eca35d, /// 0x8d8
			 0xb56046ec, /// 0x8dc
			 0xf63e4142, /// 0x8e0
			 0xb7d2b3be, /// 0x8e4
			 0x17f95d77, /// 0x8e8
			 0xbcba50c3, /// 0x8ec
			 0x3ddd43cc, /// 0x8f0
			 0xf21b8d7d, /// 0x8f4
			 0x4719c6ba, /// 0x8f8
			 0xcf5d0ea6, /// 0x8fc
			 0x10e742d1, /// 0x900
			 0x4ebb0c70, /// 0x904
			 0xdbc69f21, /// 0x908
			 0xf4edd18d, /// 0x90c
			 0xd0155837, /// 0x910
			 0x17308d77, /// 0x914
			 0x5335e71b, /// 0x918
			 0x46278a05, /// 0x91c
			 0x44ec3749, /// 0x920
			 0x3ccc223e, /// 0x924
			 0x4e5b5307, /// 0x928
			 0x14737fa4, /// 0x92c
			 0x7cebf65d, /// 0x930
			 0xc9d16310, /// 0x934
			 0x68fb4f54, /// 0x938
			 0x5ca49f87, /// 0x93c
			 0x7d258fcc, /// 0x940
			 0xd7629778, /// 0x944
			 0x6b63aaa0, /// 0x948
			 0x44e78777, /// 0x94c
			 0x4d46ece6, /// 0x950
			 0x627194fb, /// 0x954
			 0x64c1aa57, /// 0x958
			 0x9de5b310, /// 0x95c
			 0x2984153d, /// 0x960
			 0x71a9c726, /// 0x964
			 0x197e0ce6, /// 0x968
			 0x37fb707b, /// 0x96c
			 0xcd5e3736, /// 0x970
			 0xb221ef97, /// 0x974
			 0x17f8fef0, /// 0x978
			 0x1acf5337, /// 0x97c
			 0xfb8bd5f2, /// 0x980
			 0x244aac23, /// 0x984
			 0x8110a51e, /// 0x988
			 0x955a81fe, /// 0x98c
			 0x72f89663, /// 0x990
			 0x44193c3d, /// 0x994
			 0xed385f17, /// 0x998
			 0xc0ad2fb0, /// 0x99c
			 0x7d0f8635, /// 0x9a0
			 0x57532115, /// 0x9a4
			 0x3ba5b03a, /// 0x9a8
			 0xf623d972, /// 0x9ac
			 0x201ce5ea, /// 0x9b0
			 0x46b64d35, /// 0x9b4
			 0x0fc11f62, /// 0x9b8
			 0xcb7753bc, /// 0x9bc
			 0x10d47bff, /// 0x9c0
			 0xa627b5b2, /// 0x9c4
			 0x697ddf99, /// 0x9c8
			 0xf4b42c7c, /// 0x9cc
			 0x903f48ec, /// 0x9d0
			 0xc1aa0652, /// 0x9d4
			 0xce3176d1, /// 0x9d8
			 0x221bbdf2, /// 0x9dc
			 0x147681de, /// 0x9e0
			 0x6ab0d646, /// 0x9e4
			 0x2be7f250, /// 0x9e8
			 0x0c424460, /// 0x9ec
			 0xb057c248, /// 0x9f0
			 0x8281fb42, /// 0x9f4
			 0x91565b42, /// 0x9f8
			 0x1c191e42, /// 0x9fc
			 0x16c5d4b4, /// 0xa00
			 0xab5e7582, /// 0xa04
			 0x6ed6895c, /// 0xa08
			 0x05366e95, /// 0xa0c
			 0x57f85c8d, /// 0xa10
			 0xe9e3ca5d, /// 0xa14
			 0x0e627bec, /// 0xa18
			 0x716769f3, /// 0xa1c
			 0x768c38ed, /// 0xa20
			 0xf995bddc, /// 0xa24
			 0x137e0754, /// 0xa28
			 0x519aa996, /// 0xa2c
			 0x5ad2c79d, /// 0xa30
			 0x2b1d6a5a, /// 0xa34
			 0xa1e24dd2, /// 0xa38
			 0xb6d3cd63, /// 0xa3c
			 0xcb39fc8c, /// 0xa40
			 0xe24bf4b9, /// 0xa44
			 0x122a7aac, /// 0xa48
			 0x55e74fa4, /// 0xa4c
			 0x222aa0ca, /// 0xa50
			 0xb0a85516, /// 0xa54
			 0x99fae435, /// 0xa58
			 0xfab58404, /// 0xa5c
			 0x9eecdfd3, /// 0xa60
			 0x9d8d4fa9, /// 0xa64
			 0x0569116d, /// 0xa68
			 0xe12f78e9, /// 0xa6c
			 0xeb85363d, /// 0xa70
			 0xcc9bba1f, /// 0xa74
			 0x8d183263, /// 0xa78
			 0x8758633a, /// 0xa7c
			 0x055ca588, /// 0xa80
			 0x53ca627a, /// 0xa84
			 0xa943ba52, /// 0xa88
			 0x53b7b64c, /// 0xa8c
			 0x4beb4c7c, /// 0xa90
			 0x40a3978e, /// 0xa94
			 0x824e1edb, /// 0xa98
			 0x64c1b612, /// 0xa9c
			 0xa672ff50, /// 0xaa0
			 0x9db8b3c9, /// 0xaa4
			 0xfca02c26, /// 0xaa8
			 0x51e304e1, /// 0xaac
			 0xa220e8a4, /// 0xab0
			 0xdb17b419, /// 0xab4
			 0xeb47e30a, /// 0xab8
			 0xc6e69937, /// 0xabc
			 0x5c566e15, /// 0xac0
			 0x8d0dc261, /// 0xac4
			 0x344f5723, /// 0xac8
			 0xb9670518, /// 0xacc
			 0x871e75b2, /// 0xad0
			 0x0575bafb, /// 0xad4
			 0xc4491765, /// 0xad8
			 0xfbd05926, /// 0xadc
			 0xa19dac82, /// 0xae0
			 0xa9751cc1, /// 0xae4
			 0x27f168ef, /// 0xae8
			 0x0e448e75, /// 0xaec
			 0x72aaa851, /// 0xaf0
			 0x2c39aa85, /// 0xaf4
			 0x603b1434, /// 0xaf8
			 0x36e95807, /// 0xafc
			 0x676f3757, /// 0xb00
			 0xe02c91e1, /// 0xb04
			 0x738a28eb, /// 0xb08
			 0xaeac24db, /// 0xb0c
			 0xe7dd49ff, /// 0xb10
			 0xbfd532eb, /// 0xb14
			 0x2c9f7d78, /// 0xb18
			 0x53790a88, /// 0xb1c
			 0x268c1ab8, /// 0xb20
			 0x2545b7fa, /// 0xb24
			 0xb1099f36, /// 0xb28
			 0xf3977372, /// 0xb2c
			 0xcaf11dc2, /// 0xb30
			 0x6f65408f, /// 0xb34
			 0x960f425a, /// 0xb38
			 0xe770d558, /// 0xb3c
			 0x9d482ba9, /// 0xb40
			 0xcc4ad946, /// 0xb44
			 0xc2e960e6, /// 0xb48
			 0x969be796, /// 0xb4c
			 0x602cdec6, /// 0xb50
			 0xcdc60223, /// 0xb54
			 0x8304dbf4, /// 0xb58
			 0xc22d9602, /// 0xb5c
			 0xa5d26b3c, /// 0xb60
			 0x62fe2cf3, /// 0xb64
			 0xb7a7b6c1, /// 0xb68
			 0xd8d3424d, /// 0xb6c
			 0xa77990ce, /// 0xb70
			 0xc8df7315, /// 0xb74
			 0x75139d51, /// 0xb78
			 0xc5935216, /// 0xb7c
			 0xad18b3f6, /// 0xb80
			 0xd15c0114, /// 0xb84
			 0xcdf311b3, /// 0xb88
			 0xf71aec92, /// 0xb8c
			 0xfe75c820, /// 0xb90
			 0x8ea2eb24, /// 0xb94
			 0x4ed3dddb, /// 0xb98
			 0x506bf6ec, /// 0xb9c
			 0x2ae221f2, /// 0xba0
			 0x4956fe20, /// 0xba4
			 0x7118d0ac, /// 0xba8
			 0x8fb25c1d, /// 0xbac
			 0x1d378d34, /// 0xbb0
			 0x6cbad482, /// 0xbb4
			 0xd77e73a4, /// 0xbb8
			 0xcd958482, /// 0xbbc
			 0x2ed89b47, /// 0xbc0
			 0x67078e7e, /// 0xbc4
			 0xfb4bec57, /// 0xbc8
			 0xb79b807d, /// 0xbcc
			 0x5b8a4203, /// 0xbd0
			 0xfdf82a7c, /// 0xbd4
			 0x4c1448bf, /// 0xbd8
			 0x6ba201d0, /// 0xbdc
			 0xcd48625f, /// 0xbe0
			 0x0c6777f2, /// 0xbe4
			 0xaf21ef39, /// 0xbe8
			 0xc4bc4f71, /// 0xbec
			 0xafbbac28, /// 0xbf0
			 0x24673a8e, /// 0xbf4
			 0x2e741a08, /// 0xbf8
			 0x113b51f9, /// 0xbfc
			 0xa1fa11f1, /// 0xc00
			 0xc9bbdd83, /// 0xc04
			 0xaba2964f, /// 0xc08
			 0x6197a370, /// 0xc0c
			 0xea1f53c2, /// 0xc10
			 0x7626a63c, /// 0xc14
			 0x7bd8f258, /// 0xc18
			 0x1f54116c, /// 0xc1c
			 0x667f8a32, /// 0xc20
			 0x85d12111, /// 0xc24
			 0x1006b108, /// 0xc28
			 0x18395fd4, /// 0xc2c
			 0xfd8f47e0, /// 0xc30
			 0xdd676998, /// 0xc34
			 0x9c5e0b4e, /// 0xc38
			 0x3da6b90a, /// 0xc3c
			 0xf1be7dab, /// 0xc40
			 0x75c0782d, /// 0xc44
			 0x337e0ff1, /// 0xc48
			 0x46776bb6, /// 0xc4c
			 0x67734823, /// 0xc50
			 0x8a2de9ea, /// 0xc54
			 0xd812a729, /// 0xc58
			 0x1ff9d9af, /// 0xc5c
			 0xeb5564d6, /// 0xc60
			 0xdda0ca82, /// 0xc64
			 0x334b6a56, /// 0xc68
			 0x6d0ffe5c, /// 0xc6c
			 0x8ec4d0a3, /// 0xc70
			 0x23668bc8, /// 0xc74
			 0xa1bd9785, /// 0xc78
			 0x986dc8eb, /// 0xc7c
			 0x689a590d, /// 0xc80
			 0x2855543b, /// 0xc84
			 0xe6ff0df3, /// 0xc88
			 0x97af6a83, /// 0xc8c
			 0x8602d32b, /// 0xc90
			 0x44107f91, /// 0xc94
			 0xa09b0930, /// 0xc98
			 0x3a7853d9, /// 0xc9c
			 0xbad47bce, /// 0xca0
			 0xfd265768, /// 0xca4
			 0xc71c94aa, /// 0xca8
			 0x41fa3a3b, /// 0xcac
			 0x596f3930, /// 0xcb0
			 0x34f01509, /// 0xcb4
			 0x538627a8, /// 0xcb8
			 0x483137e4, /// 0xcbc
			 0xaf7b809c, /// 0xcc0
			 0x80eecbae, /// 0xcc4
			 0xb256477f, /// 0xcc8
			 0x05269638, /// 0xccc
			 0xe6cfe69d, /// 0xcd0
			 0xb794720b, /// 0xcd4
			 0xe46ff825, /// 0xcd8
			 0xf1e2362a, /// 0xcdc
			 0x0a513c83, /// 0xce0
			 0xd409fe46, /// 0xce4
			 0x32ae16fb, /// 0xce8
			 0xd308a537, /// 0xcec
			 0xdc5ac6fa, /// 0xcf0
			 0xaebec7a3, /// 0xcf4
			 0x319bdd6a, /// 0xcf8
			 0x417f0f05, /// 0xcfc
			 0x965e7a4b, /// 0xd00
			 0x23aeb923, /// 0xd04
			 0xf904bc6e, /// 0xd08
			 0x2c9f5dfa, /// 0xd0c
			 0x3133c405, /// 0xd10
			 0xb36af8e1, /// 0xd14
			 0xc49c5fd3, /// 0xd18
			 0x43888023, /// 0xd1c
			 0xea3dfdfa, /// 0xd20
			 0x97cfb286, /// 0xd24
			 0x4dd43b59, /// 0xd28
			 0x15e1f55e, /// 0xd2c
			 0x615067fc, /// 0xd30
			 0x96580d9f, /// 0xd34
			 0x129456ac, /// 0xd38
			 0x96bfca2d, /// 0xd3c
			 0x0bddd077, /// 0xd40
			 0x34bd143b, /// 0xd44
			 0x63bfd2d4, /// 0xd48
			 0xd37b3995, /// 0xd4c
			 0xaabf32de, /// 0xd50
			 0xbe6df690, /// 0xd54
			 0x41301808, /// 0xd58
			 0x49bbe8d1, /// 0xd5c
			 0x86282aba, /// 0xd60
			 0xa4758238, /// 0xd64
			 0xbb54f46f, /// 0xd68
			 0xad069c86, /// 0xd6c
			 0x58204782, /// 0xd70
			 0xe3f39c70, /// 0xd74
			 0x69704586, /// 0xd78
			 0x2a152b41, /// 0xd7c
			 0xd030e675, /// 0xd80
			 0x9bb15b9c, /// 0xd84
			 0x4b43fc14, /// 0xd88
			 0x1580e09b, /// 0xd8c
			 0xc69cfb67, /// 0xd90
			 0x2df71d43, /// 0xd94
			 0xd1c17a32, /// 0xd98
			 0xd8c111ab, /// 0xd9c
			 0xbff0d87b, /// 0xda0
			 0x30c4dede, /// 0xda4
			 0x08cd1b05, /// 0xda8
			 0x8ead64bc, /// 0xdac
			 0x93c8631f, /// 0xdb0
			 0x35324025, /// 0xdb4
			 0x7cee0bfd, /// 0xdb8
			 0xd88a0838, /// 0xdbc
			 0x1e262228, /// 0xdc0
			 0xf9b5d2b2, /// 0xdc4
			 0x985dbbd6, /// 0xdc8
			 0x71b3a6dd, /// 0xdcc
			 0x54c5e93c, /// 0xdd0
			 0xc2895b1e, /// 0xdd4
			 0xe38f9ad2, /// 0xdd8
			 0xa2721df0, /// 0xddc
			 0x92b9aaf3, /// 0xde0
			 0xfaae2949, /// 0xde4
			 0xf98bcb3c, /// 0xde8
			 0x6770dfef, /// 0xdec
			 0x00499584, /// 0xdf0
			 0x0cc90687, /// 0xdf4
			 0x61cc76b2, /// 0xdf8
			 0xea9e9ab7, /// 0xdfc
			 0xa2650a49, /// 0xe00
			 0x1dd705f9, /// 0xe04
			 0x8b4ef239, /// 0xe08
			 0x81fa6df5, /// 0xe0c
			 0x2031d18c, /// 0xe10
			 0x47b7d2de, /// 0xe14
			 0xf3f727a2, /// 0xe18
			 0x22bebcc7, /// 0xe1c
			 0x78ee94ed, /// 0xe20
			 0x8cea1f2c, /// 0xe24
			 0x9a46353e, /// 0xe28
			 0x33c68000, /// 0xe2c
			 0xc2b72755, /// 0xe30
			 0x2353c8c3, /// 0xe34
			 0x06dd31fc, /// 0xe38
			 0x9e9ea8f5, /// 0xe3c
			 0xf921a465, /// 0xe40
			 0xcad86486, /// 0xe44
			 0xd5fc8f20, /// 0xe48
			 0xdc0f42e0, /// 0xe4c
			 0x5b21901f, /// 0xe50
			 0xcca9d94a, /// 0xe54
			 0xa577f2a3, /// 0xe58
			 0x868390a2, /// 0xe5c
			 0x3c5d31b6, /// 0xe60
			 0xdabeea43, /// 0xe64
			 0x0b85412a, /// 0xe68
			 0x4e59d70c, /// 0xe6c
			 0xbc9ce77d, /// 0xe70
			 0xbed300dd, /// 0xe74
			 0x86d714d6, /// 0xe78
			 0x0cffa455, /// 0xe7c
			 0x094a8cd5, /// 0xe80
			 0xc1fb7d6c, /// 0xe84
			 0xd68d973e, /// 0xe88
			 0xb72f6cba, /// 0xe8c
			 0xee9cf25a, /// 0xe90
			 0x8540123f, /// 0xe94
			 0x5760af0e, /// 0xe98
			 0xb97dd88b, /// 0xe9c
			 0x79261ff8, /// 0xea0
			 0xe4733a2d, /// 0xea4
			 0xcb977502, /// 0xea8
			 0x77d34f30, /// 0xeac
			 0x4176ae82, /// 0xeb0
			 0x39b0dbed, /// 0xeb4
			 0x556d9c58, /// 0xeb8
			 0x8b8363b3, /// 0xebc
			 0x987d3413, /// 0xec0
			 0x7ece88f8, /// 0xec4
			 0xa820e5b6, /// 0xec8
			 0x6d0e9792, /// 0xecc
			 0x4cc08a35, /// 0xed0
			 0xf594a75f, /// 0xed4
			 0x4306ff77, /// 0xed8
			 0xc42842fc, /// 0xedc
			 0x3eeac117, /// 0xee0
			 0xd7c6fd64, /// 0xee4
			 0xd8e5a457, /// 0xee8
			 0x633d4147, /// 0xeec
			 0xdf87581e, /// 0xef0
			 0xec59c81f, /// 0xef4
			 0x1140ebd3, /// 0xef8
			 0x7210060e, /// 0xefc
			 0x4fe78f59, /// 0xf00
			 0xfea401a3, /// 0xf04
			 0x0c3f959d, /// 0xf08
			 0xec659fc9, /// 0xf0c
			 0x6e34ceb1, /// 0xf10
			 0x4a64c4ac, /// 0xf14
			 0x736c66a2, /// 0xf18
			 0x7686aca9, /// 0xf1c
			 0xbeb45b89, /// 0xf20
			 0x874b003c, /// 0xf24
			 0x3060058e, /// 0xf28
			 0x7cb6a28e, /// 0xf2c
			 0x7fb5a5dc, /// 0xf30
			 0x962d9e39, /// 0xf34
			 0xe52cb509, /// 0xf38
			 0x02980727, /// 0xf3c
			 0xc5e96ecf, /// 0xf40
			 0x682301ee, /// 0xf44
			 0x67295931, /// 0xf48
			 0x285ad8f4, /// 0xf4c
			 0x437e83a5, /// 0xf50
			 0x4fb1eb39, /// 0xf54
			 0xb7766c86, /// 0xf58
			 0xec5f0dde, /// 0xf5c
			 0x69695f59, /// 0xf60
			 0x6a121d91, /// 0xf64
			 0x4694ac96, /// 0xf68
			 0x31414ba9, /// 0xf6c
			 0x9ab7487c, /// 0xf70
			 0x07d416e3, /// 0xf74
			 0x695a5be0, /// 0xf78
			 0x8251d1fa, /// 0xf7c
			 0x74118ad9, /// 0xf80
			 0x60a04aca, /// 0xf84
			 0x8bbe51d2, /// 0xf88
			 0xe910af21, /// 0xf8c
			 0x76c086ef, /// 0xf90
			 0x3922ffc2, /// 0xf94
			 0x25cdbfdb, /// 0xf98
			 0x1f1718b8, /// 0xf9c
			 0x73a6c0c2, /// 0xfa0
			 0x32f89800, /// 0xfa4
			 0xd4b6df10, /// 0xfa8
			 0xef7ffdea, /// 0xfac
			 0x8ff3f13e, /// 0xfb0
			 0x1a9f8e91, /// 0xfb4
			 0x4154699b, /// 0xfb8
			 0xde10072a, /// 0xfbc
			 0x7927f24a, /// 0xfc0
			 0xd778efc6, /// 0xfc4
			 0x12c99600, /// 0xfc8
			 0x545daffb, /// 0xfcc
			 0x930acb1b, /// 0xfd0
			 0x5640659c, /// 0xfd4
			 0x79cc9a9c, /// 0xfd8
			 0x45f4f48a, /// 0xfdc
			 0x7157fde6, /// 0xfe0
			 0x66f2dfbe, /// 0xfe4
			 0xa8101ab2, /// 0xfe8
			 0x82df22f6, /// 0xfec
			 0x38be467c, /// 0xff0
			 0xebe15e6e, /// 0xff4
			 0xb505e439, /// 0xff8
			 0x3807c652 /// last
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
			 0x000000f4,
			 0x0000002c,
			 0x00000394,
			 0x000001e8,
			 0x000004ec,
			 0x00000074,
			 0x0000024c,
			 0x000001cc,

			 /// vpu register f2
			 0x41000000,
			 0x40800000,
			 0x40400000,
			 0x40800000,
			 0x41b80000,
			 0x41000000,
			 0x41a00000,
			 0x41b00000,

			 /// vpu register f3
			 0x41500000,
			 0x40e00000,
			 0x41700000,
			 0x41600000,
			 0x41f00000,
			 0x41c80000,
			 0x41980000,
			 0x41000000,

			 /// vpu register f4
			 0x41700000,
			 0x41f00000,
			 0x41c80000,
			 0x41300000,
			 0x41880000,
			 0x41e00000,
			 0x41d80000,
			 0x40c00000,

			 /// vpu register f5
			 0x41300000,
			 0x41a00000,
			 0x41b80000,
			 0x41a00000,
			 0x41100000,
			 0x41e80000,
			 0x41800000,
			 0x41b00000,

			 /// vpu register f6
			 0x41500000,
			 0x41a80000,
			 0x41300000,
			 0x41100000,
			 0x41f00000,
			 0x40400000,
			 0x40a00000,
			 0x41c00000,

			 /// vpu register f7
			 0x41d80000,
			 0x41700000,
			 0x41f80000,
			 0x41c80000,
			 0x41000000,
			 0x40c00000,
			 0x40800000,
			 0x41d00000,

			 /// vpu register f8
			 0x40000000,
			 0x3f800000,
			 0x41000000,
			 0x41100000,
			 0x41b80000,
			 0x41f00000,
			 0x40e00000,
			 0x40e00000,

			 /// vpu register f9
			 0x40c00000,
			 0x41980000,
			 0x41980000,
			 0x41c00000,
			 0x41f00000,
			 0x41f00000,
			 0x41400000,
			 0x41980000,

			 /// vpu register f10
			 0x41a80000,
			 0x40a00000,
			 0x40e00000,
			 0x41600000,
			 0x41600000,
			 0x41980000,
			 0x41880000,
			 0x41700000,

			 /// vpu register f11
			 0x42000000,
			 0x40000000,
			 0x41700000,
			 0x40400000,
			 0x41980000,
			 0x41c80000,
			 0x41c80000,
			 0x41e80000,

			 /// vpu register f12
			 0x41300000,
			 0x41f80000,
			 0x41900000,
			 0x3f800000,
			 0x41300000,
			 0x41c80000,
			 0x41500000,
			 0x41400000,

			 /// vpu register f13
			 0x40e00000,
			 0x41700000,
			 0x3f800000,
			 0x3f800000,
			 0x41400000,
			 0x41980000,
			 0x41f00000,
			 0x41800000,

			 /// vpu register f14
			 0x41a80000,
			 0x41000000,
			 0x3f800000,
			 0x40c00000,
			 0x41c80000,
			 0x41000000,
			 0x41f80000,
			 0x41d80000,

			 /// vpu register f15
			 0x41e00000,
			 0x3f800000,
			 0x41b80000,
			 0x41d00000,
			 0x41e80000,
			 0x41600000,
			 0x40c00000,
			 0x41e00000,

			 /// vpu register f16
			 0x40400000,
			 0x41b00000,
			 0x40800000,
			 0x40e00000,
			 0x41700000,
			 0x41d80000,
			 0x40400000,
			 0x41300000,

			 /// vpu register f17
			 0x41000000,
			 0x41a00000,
			 0x41c80000,
			 0x41a80000,
			 0x41200000,
			 0x41000000,
			 0x41700000,
			 0x41f00000,

			 /// vpu register f18
			 0x41b00000,
			 0x41500000,
			 0x41000000,
			 0x41200000,
			 0x41300000,
			 0x41b00000,
			 0x41d80000,
			 0x41a00000,

			 /// vpu register f19
			 0x41f00000,
			 0x41400000,
			 0x41a00000,
			 0x41880000,
			 0x41b00000,
			 0x41f00000,
			 0x41d80000,
			 0x40c00000,

			 /// vpu register f20
			 0x41e80000,
			 0x41a80000,
			 0x41c00000,
			 0x40800000,
			 0x40e00000,
			 0x41f80000,
			 0x41400000,
			 0x3f800000,

			 /// vpu register f21
			 0x41b00000,
			 0x40800000,
			 0x41900000,
			 0x41700000,
			 0x41d80000,
			 0x41a80000,
			 0x41300000,
			 0x41a80000,

			 /// vpu register f22
			 0x41c80000,
			 0x41b80000,
			 0x41000000,
			 0x41b00000,
			 0x40400000,
			 0x41a00000,
			 0x41700000,
			 0x40e00000,

			 /// vpu register f23
			 0x41200000,
			 0x41000000,
			 0x41300000,
			 0x41600000,
			 0x41c80000,
			 0x41f00000,
			 0x41b00000,
			 0x41900000,

			 /// vpu register f24
			 0x41300000,
			 0x40c00000,
			 0x41f80000,
			 0x42000000,
			 0x41100000,
			 0x41e00000,
			 0x41b00000,
			 0x41400000,

			 /// vpu register f25
			 0x41a00000,
			 0x41c80000,
			 0x3f800000,
			 0x41300000,
			 0x41980000,
			 0x41a80000,
			 0x41f00000,
			 0x40400000,

			 /// vpu register f26
			 0x41e80000,
			 0x41a00000,
			 0x41200000,
			 0x41300000,
			 0x41300000,
			 0x41b00000,
			 0x41b80000,
			 0x41600000,

			 /// vpu register f27
			 0x3f800000,
			 0x41c00000,
			 0x40000000,
			 0x41b80000,
			 0x41f80000,
			 0x40400000,
			 0x41880000,
			 0x41700000,

			 /// vpu register f28
			 0x41d80000,
			 0x41300000,
			 0x41f00000,
			 0x41c80000,
			 0x41600000,
			 0x41c80000,
			 0x42000000,
			 0x40a00000,

			 /// vpu register f29
			 0x41b80000,
			 0x41800000,
			 0x41c00000,
			 0x41100000,
			 0x41b00000,
			 0x41980000,
			 0x42000000,
			 0x41100000,

			 /// vpu register f30
			 0x40e00000,
			 0x40a00000,
			 0x41200000,
			 0x41700000,
			 0x41800000,
			 0x40800000,
			 0x41a80000,
			 0x41900000,

			 /// vpu register f31
			 0x41100000,
			 0x41f00000,
			 0x41a00000,
			 0x41700000,
			 0x40a00000,
			 0x41000000,
			 0x41600000,
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
		"fpackreph.pi f12, f3\n"                              ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f12, f27\n"                             ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f27, f12\n"                             ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f6, f1\n"                               ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f5, f6\n"                               ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f8, f23\n"                              ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f19, f13\n"                             ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f21, f26\n"                             ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f15, f20\n"                             ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f8, f1\n"                               ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f10, f10\n"                             ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f23, f22\n"                             ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f9, f16\n"                              ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f9, f10\n"                              ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f23, f15\n"                             ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f5, f27\n"                              ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f16, f21\n"                             ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f17, f28\n"                             ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f24, f12\n"                             ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f2, f8\n"                               ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f2, f16\n"                              ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f16, f29\n"                             ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f4, f27\n"                              ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f7, f16\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f28, f20\n"                             ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f6, f10\n"                              ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f25, f26\n"                             ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f24, f5\n"                              ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f27, f13\n"                             ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f29, f10\n"                             ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f27, f10\n"                             ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f21, f9\n"                              ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f16, f17\n"                             ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f26, f14\n"                             ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f9, f8\n"                               ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f23, f12\n"                             ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f23, f23\n"                             ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f17, f21\n"                             ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f3, f0\n"                               ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f5, f7\n"                               ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f6, f22\n"                              ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f28, f8\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f8, f22\n"                              ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f29, f18\n"                             ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f24, f24\n"                             ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f12, f18\n"                             ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f30, f1\n"                              ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f8, f12\n"                              ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f10, f8\n"                              ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f16, f1\n"                              ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f12, f22\n"                             ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f21, f2\n"                              ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f9, f16\n"                              ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f25, f28\n"                             ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f4, f26\n"                              ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f17, f16\n"                             ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f9, f18\n"                              ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f18, f10\n"                             ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f21, f15\n"                             ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f9, f26\n"                              ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f5, f24\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f10, f13\n"                             ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f10, f11\n"                             ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f20, f21\n"                             ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f19, f17\n"                             ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f9, f29\n"                              ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f8, f14\n"                              ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f28, f21\n"                             ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f30, f6\n"                              ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f12, f28\n"                             ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f19, f22\n"                             ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f11, f12\n"                             ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f14, f16\n"                             ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f18, f14\n"                             ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f8, f14\n"                              ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f8, f28\n"                              ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f18, f3\n"                              ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f11, f21\n"                             ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f14, f21\n"                             ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f10, f23\n"                             ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f24, f1\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f3, f29\n"                              ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f8, f7\n"                               ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f29, f9\n"                              ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f11, f21\n"                             ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f17, f25\n"                             ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f30, f0\n"                              ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f26, f21\n"                             ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f23, f1\n"                              ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f21, f21\n"                             ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f4, f26\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f19, f21\n"                             ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f9, f23\n"                              ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f3, f13\n"                              ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f27, f9\n"                              ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f14, f23\n"                             ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f18, f12\n"                             ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f26, f23\n"                             ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fpackreph.pi f25, f26\n"                             ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
