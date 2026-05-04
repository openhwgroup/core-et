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
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00000
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00004
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00008
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0000c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00010
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00014
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00018
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0001c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00020
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00024
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0002c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00030
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00038
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0003c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00040
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00044
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00048
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0004c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00054
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00058
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0005c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00064
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00074
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00080
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00094
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00098
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000a4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000a8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000ac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000bc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000c4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000c8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000cc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000d0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000d4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000d8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000dc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000e4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000ec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000f0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000f4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000f8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000fc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00100
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00104
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00108
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0010c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00110
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00114
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00118
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0011c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00120
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00124
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x0c400000,                                                  // Leading 1s:                                 /// 0012c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00130
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00134
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00138
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0013c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00140
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00144
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00150
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x33333333,                                                  // 4 random values                             /// 00158
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0015c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00160
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00164
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00168
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0016c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00174
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00178
			 0x33333333,                                                  // 4 random values                             /// 0017c
			 0x55555555,                                                  // 4 random values                             /// 00180
			 0x0e000007,                                                  // Trailing 1s:                                /// 00184
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00188
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00190
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00194
			 0xbf028f5c,                                                  // -0.51                                       /// 00198
			 0x7fc00001,                                                  // signaling NaN                               /// 0019c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001a4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001a8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001b0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001c4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001cc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001d0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001d8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001dc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001e4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001e8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001f0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001fc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00200
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00208
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00210
			 0x0e000001,                                                  // Trailing 1s:                                /// 00214
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00218
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00228
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00230
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00238
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0023c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00240
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00244
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0024c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00254
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00258
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0025c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00264
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x0c400000,                                                  // Leading 1s:                                 /// 0026c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00270
			 0x0c700000,                                                  // Leading 1s:                                 /// 00274
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00278
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0027c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00280
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0028c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00298
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0029c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002a0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002a4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002a8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002ac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002b0
			 0xbf028f5c,                                                  // -0.51                                       /// 002b4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002b8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002bc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002c4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002c8
			 0x0c400000,                                                  // Leading 1s:                                 /// 002cc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002d4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002d8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002dc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002e0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002e8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002f0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002f4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00300
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00308
			 0xbf028f5c,                                                  // -0.51                                       /// 0030c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00310
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00314
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00318
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00320
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00328
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0032c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00330
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00338
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0033c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00340
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00344
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00348
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00354
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00358
			 0xbf028f5c,                                                  // -0.51                                       /// 0035c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00360
			 0x80000000,                                                  // -zero                                       /// 00364
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x33333333,                                                  // 4 random values                             /// 0036c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00370
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00378
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00380
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00384
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00388
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0038c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00390
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00394
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00398
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x80011111,                                                  // -9.7958E-41                                 /// 003a4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003b0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003b4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003b8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003bc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003c0
			 0x0e000001,                                                  // Trailing 1s:                                /// 003c4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003c8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003cc
			 0x80011111,                                                  // -9.7958E-41                                 /// 003d0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003d4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003dc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003e4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003e8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003ec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003f0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003f4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003f8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00400
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00404
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0040c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00410
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00414
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00418
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x4b000000,                                                  // 8388608.0                                   /// 00428
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0042c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x33333333,                                                  // 4 random values                             /// 00434
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00438
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0043c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00440
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00444
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00448
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0044c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x00000000,                                                  // zero                                        /// 00454
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00458
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0045c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00460
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00464
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00468
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0046c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00470
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00474
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0047c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00484
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00488
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0048c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00490
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00494
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00498
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0049c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004a0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004a4
			 0x33333333,                                                  // 4 random values                             /// 004a8
			 0x80000000,                                                  // -zero                                       /// 004ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004b0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004b4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004b8
			 0x7f800000,                                                  // inf                                         /// 004bc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004c0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0xcb000000,                                                  // -8388608.0                                  /// 004cc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004d0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004dc
			 0xcb000000,                                                  // -8388608.0                                  /// 004e0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004e4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 004e8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004ec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004f0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004f4
			 0x0c600000,                                                  // Leading 1s:                                 /// 004f8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004fc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00500
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00504
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x80000000,                                                  // -zero                                       /// 00510
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00514
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00518
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00520
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00528
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0052c
			 0xff800000,                                                  // -inf                                        /// 00530
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00538
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0053c
			 0x33333333,                                                  // 4 random values                             /// 00540
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00548
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0054c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00554
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00558
			 0x55555555,                                                  // 4 random values                             /// 0055c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00560
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00564
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00568
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0056c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00570
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00574
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00578
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0057c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00580
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00584
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0058c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00590
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0059c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005ac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005b0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005b4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005b8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005bc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005c0
			 0x80000000,                                                  // -zero                                       /// 005c4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005c8
			 0x80000000,                                                  // -zero                                       /// 005cc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005d4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005dc
			 0xcb000000,                                                  // -8388608.0                                  /// 005e0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005e4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005e8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x00011111,                                                  // 9.7958E-41                                  /// 005f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005f4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005f8
			 0x80000000,                                                  // -zero                                       /// 005fc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00600
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00608
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00610
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00618
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00620
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00628
			 0xff800000,                                                  // -inf                                        /// 0062c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00634
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00638
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0063c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00640
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00644
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00648
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00650
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00654
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00658
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0065c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00660
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00664
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00668
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0066c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00670
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00674
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00678
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00684
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00688
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0068c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00690
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00694
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0069c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006a0
			 0x0c780000,                                                  // Leading 1s:                                 /// 006a4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006a8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006ac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006b4
			 0xcb000000,                                                  // -8388608.0                                  /// 006b8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006c0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006c8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006cc
			 0x4b000000,                                                  // 8388608.0                                   /// 006d0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006d4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x80011111,                                                  // -9.7958E-41                                 /// 006dc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006e0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 006e8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006ec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006f0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006f8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00708
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0070c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00710
			 0x55555555,                                                  // 4 random values                             /// 00714
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0071c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00720
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00724
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00728
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0073c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00740
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00744
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00748
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0074c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00758
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0075c
			 0x55555555,                                                  // 4 random values                             /// 00760
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0076c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00770
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00778
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0077c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00784
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0078c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00790
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00794
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0079c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007a0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007a4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007a8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007ac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007b4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007c0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007c8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007d0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007d4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007d8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007dc
			 0x0c700000,                                                  // Leading 1s:                                 /// 007e0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007e8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007ec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007f0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007f4
			 0x0c780000,                                                  // Leading 1s:                                 /// 007f8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007fc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00800
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00804
			 0xbf028f5c,                                                  // -0.51                                       /// 00808
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0080c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00810
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00814
			 0x00000000,                                                  // zero                                        /// 00818
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0081c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00820
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00824
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00828
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0082c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00834
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0083c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00840
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0084c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00850
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00854
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0085c
			 0xff800000,                                                  // -inf                                        /// 00860
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00868
			 0x80011111,                                                  // -9.7958E-41                                 /// 0086c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x0c780000,                                                  // Leading 1s:                                 /// 00874
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00878
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0087c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00888
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008a8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008ac
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008b0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008b4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008c4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008c8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008cc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008d0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008d8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008e0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008e4
			 0x0c400000,                                                  // Leading 1s:                                 /// 008e8
			 0x0c700000,                                                  // Leading 1s:                                 /// 008ec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 008f8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008fc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00900
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0xff800000,                                                  // -inf                                        /// 00908
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0090c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00910
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00914
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00918
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0091c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00920
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00924
			 0x00011111,                                                  // 9.7958E-41                                  /// 00928
			 0x0c600000,                                                  // Leading 1s:                                 /// 0092c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00930
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0093c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00940
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00944
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00948
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0094c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00950
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00954
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0095c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00964
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00968
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0096c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00974
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00978
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0097c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00988
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0098c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00990
			 0x4b000000,                                                  // 8388608.0                                   /// 00994
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009a0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009a4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009a8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009ac
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009b0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009b8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x00000000,                                                  // zero                                        /// 009c0
			 0x55555555,                                                  // 4 random values                             /// 009c4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009cc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009d0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009d4
			 0xff800000,                                                  // -inf                                        /// 009d8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009dc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009e0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009e8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009ec
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x7fc00001,                                                  // signaling NaN                               /// 009f4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009fc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a00
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a04
			 0x7fc00001,                                                  // signaling NaN                               /// 00a08
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a0c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a10
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a14
			 0xbf028f5c,                                                  // -0.51                                       /// 00a18
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a1c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a20
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a24
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a28
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a2c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a30
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a34
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a38
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a3c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0xcb000000,                                                  // -8388608.0                                  /// 00a44
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a50
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a54
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a58
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a5c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a64
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a68
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a6c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a78
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a7c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a80
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a84
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x00000000,                                                  // zero                                        /// 00a8c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a98
			 0x3f028f5c,                                                  // 0.51                                        /// 00a9c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00aa8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00aac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ab0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ac0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ac4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ac8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ad4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ae0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ae4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00af4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x4b000000,                                                  // 8388608.0                                   /// 00afc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b00
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b04
			 0x7f800000,                                                  // inf                                         /// 00b08
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b10
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b14
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b20
			 0xffc00001,                                                  // -signaling NaN                              /// 00b24
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b28
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b2c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b30
			 0x55555555,                                                  // 4 random values                             /// 00b34
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b3c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b40
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b44
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b48
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b4c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b50
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b54
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b58
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b5c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b64
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b68
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b6c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b70
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b74
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b78
			 0x3f028f5c,                                                  // 0.51                                        /// 00b7c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b84
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b88
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b8c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b90
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b98
			 0xbf028f5c,                                                  // -0.51                                       /// 00b9c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ba4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00bb0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bb8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bbc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bc8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bd0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bd4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00be0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00be4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00be8
			 0xffc00001,                                                  // -signaling NaN                              /// 00bec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bf0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bf4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bfc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c00
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c04
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c08
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c0c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c14
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c1c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c24
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c28
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c34
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c38
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c40
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c44
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c48
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c4c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c50
			 0x33333333,                                                  // 4 random values                             /// 00c54
			 0xffc00001,                                                  // -signaling NaN                              /// 00c58
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c60
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c64
			 0xff800000,                                                  // -inf                                        /// 00c68
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c6c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c70
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c78
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c7c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c84
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c8c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c90
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c94
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c98
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c9c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ca0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ca4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cb0
			 0x55555555,                                                  // 4 random values                             /// 00cb4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cb8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cc0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00cc4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cc8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ccc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cd0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cd4
			 0x3f028f5c,                                                  // 0.51                                        /// 00cd8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cdc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ce8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cf0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00cf8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d04
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d08
			 0xffc00001,                                                  // -signaling NaN                              /// 00d0c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d18
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d1c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d20
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d24
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d28
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d2c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d30
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d38
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d3c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d40
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d44
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d4c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d58
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d60
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d64
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d68
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d70
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d74
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d78
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d7c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d80
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d84
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0xcb000000,                                                  // -8388608.0                                  /// 00d8c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d90
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d94
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00da4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00da8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00dac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00db0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00db4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00db8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dbc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00dc0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00dc4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00dcc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dd4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00dd8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ddc
			 0x4b000000,                                                  // 8388608.0                                   /// 00de0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00de4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00de8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00dec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00df0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00df8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00dfc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e00
			 0x55555555,                                                  // 4 random values                             /// 00e04
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e08
			 0xcb000000,                                                  // -8388608.0                                  /// 00e0c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e1c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e20
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e24
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e2c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e38
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e3c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e40
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e44
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e54
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e58
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e68
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e6c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e70
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e74
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e80
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e84
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e88
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e8c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e90
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e94
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e98
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e9c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ea0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x4b000000,                                                  // 8388608.0                                   /// 00ea8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00eb0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00eb4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00eb8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ec8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ecc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ed0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ed4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00edc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ee0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ee8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00eec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ef0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ef4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ef8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f00
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f04
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f0c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f10
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f14
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f18
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f20
			 0xcb000000,                                                  // -8388608.0                                  /// 00f24
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f28
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f2c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f30
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f34
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f3c
			 0x00000000,                                                  // zero                                        /// 00f40
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f44
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f48
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f4c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f54
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f58
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f60
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f64
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f68
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f6c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f70
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f78
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f80
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f84
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f90
			 0x4b000000,                                                  // 8388608.0                                   /// 00f94
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f9c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fa0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fa4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fa8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fac
			 0xff800000,                                                  // -inf                                        /// 00fb0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fb8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fbc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fc0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fc4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fcc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x7fc00001,                                                  // signaling NaN                               /// 00fd4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fdc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fe0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fe8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ff0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ff4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ff8
			 0x00800001                                                  // none of the mantissa set to +3ulp           /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xc608a826, /// 0x0
			 0x2ba2652a, /// 0x4
			 0x175e6bfd, /// 0x8
			 0xc1c64198, /// 0xc
			 0x2ccf5598, /// 0x10
			 0x6f5b1428, /// 0x14
			 0xc89ebb91, /// 0x18
			 0xf198d3d3, /// 0x1c
			 0x54a80fc6, /// 0x20
			 0x1bf8b380, /// 0x24
			 0x566eff5c, /// 0x28
			 0xbb39c1e6, /// 0x2c
			 0x5819468f, /// 0x30
			 0x5143dd42, /// 0x34
			 0xe35afa97, /// 0x38
			 0x3f82709a, /// 0x3c
			 0x3b996e88, /// 0x40
			 0xf31426d2, /// 0x44
			 0xe59a52a6, /// 0x48
			 0x79dd017e, /// 0x4c
			 0xf14ec550, /// 0x50
			 0x7ab38996, /// 0x54
			 0x10450a6f, /// 0x58
			 0x818bffe2, /// 0x5c
			 0x51995a0b, /// 0x60
			 0xc46c35f5, /// 0x64
			 0xf25f25d4, /// 0x68
			 0x4d37a3f8, /// 0x6c
			 0x1c819e7e, /// 0x70
			 0x8c3e2c49, /// 0x74
			 0xe78f5dac, /// 0x78
			 0xb76bffb4, /// 0x7c
			 0xbb4462dc, /// 0x80
			 0x1cc0ae81, /// 0x84
			 0xfeef531b, /// 0x88
			 0xbfe0d7fb, /// 0x8c
			 0x70ced447, /// 0x90
			 0x9805263d, /// 0x94
			 0x41a57358, /// 0x98
			 0x10c52c9e, /// 0x9c
			 0xa3bb28a2, /// 0xa0
			 0xdba4e413, /// 0xa4
			 0xedbe0db9, /// 0xa8
			 0x7070b449, /// 0xac
			 0x6040bba0, /// 0xb0
			 0x722b001f, /// 0xb4
			 0xc954b7bc, /// 0xb8
			 0x7e788e0c, /// 0xbc
			 0x0c6ff091, /// 0xc0
			 0x9e7a7089, /// 0xc4
			 0xcd418080, /// 0xc8
			 0xb072f5cc, /// 0xcc
			 0x4266881b, /// 0xd0
			 0x02bd9360, /// 0xd4
			 0x3b606b4a, /// 0xd8
			 0x8859b5de, /// 0xdc
			 0x94f2f0ab, /// 0xe0
			 0xea9235be, /// 0xe4
			 0x01478156, /// 0xe8
			 0xcffae33e, /// 0xec
			 0x79c7b6ad, /// 0xf0
			 0x5ca0f4ed, /// 0xf4
			 0xa2252ed2, /// 0xf8
			 0x5ca33aa0, /// 0xfc
			 0x308c8cf2, /// 0x100
			 0xd0dc95e2, /// 0x104
			 0x1450270d, /// 0x108
			 0xa151a36c, /// 0x10c
			 0xab6dc53f, /// 0x110
			 0x459437ae, /// 0x114
			 0xd7fb04e7, /// 0x118
			 0xfcf4d160, /// 0x11c
			 0x74a44b92, /// 0x120
			 0xc3a78bf8, /// 0x124
			 0x27d646ed, /// 0x128
			 0x482c3945, /// 0x12c
			 0xc1f157ac, /// 0x130
			 0x4ac01857, /// 0x134
			 0x268de8c5, /// 0x138
			 0x962b0112, /// 0x13c
			 0xc514f51e, /// 0x140
			 0xaa468eca, /// 0x144
			 0xff07747a, /// 0x148
			 0x2789bb1a, /// 0x14c
			 0xb31d2da1, /// 0x150
			 0xc220fefe, /// 0x154
			 0x13b86bb3, /// 0x158
			 0x92146699, /// 0x15c
			 0xb0921c73, /// 0x160
			 0x28fd1c7f, /// 0x164
			 0x8ff56595, /// 0x168
			 0xadc7a9ed, /// 0x16c
			 0x202aa976, /// 0x170
			 0xa6ae9b16, /// 0x174
			 0xf2fa40fd, /// 0x178
			 0x3b4656d4, /// 0x17c
			 0x543e73d2, /// 0x180
			 0x475957da, /// 0x184
			 0xde12fd55, /// 0x188
			 0xd6225791, /// 0x18c
			 0x5dfc1d76, /// 0x190
			 0x46c65c98, /// 0x194
			 0xa0a902fb, /// 0x198
			 0x76672a0f, /// 0x19c
			 0x3ba8da9d, /// 0x1a0
			 0xb249bc3c, /// 0x1a4
			 0x6a818c9c, /// 0x1a8
			 0xf5466b44, /// 0x1ac
			 0x8288b7e4, /// 0x1b0
			 0x80500eb8, /// 0x1b4
			 0xa1ac980d, /// 0x1b8
			 0xfcd4bb54, /// 0x1bc
			 0x6f8a942b, /// 0x1c0
			 0x432bd42d, /// 0x1c4
			 0xa1a27558, /// 0x1c8
			 0x0efb53a7, /// 0x1cc
			 0x2c811d67, /// 0x1d0
			 0x42e3ff9a, /// 0x1d4
			 0x1389f0ed, /// 0x1d8
			 0x9b1b5ccf, /// 0x1dc
			 0x9213adaf, /// 0x1e0
			 0x00886c60, /// 0x1e4
			 0x8ce5c62a, /// 0x1e8
			 0x024bb117, /// 0x1ec
			 0x337179f6, /// 0x1f0
			 0xa02e0fe0, /// 0x1f4
			 0x47b14080, /// 0x1f8
			 0xc4aaea5e, /// 0x1fc
			 0x865805ff, /// 0x200
			 0x19aebdc0, /// 0x204
			 0xce673c48, /// 0x208
			 0xf92b1120, /// 0x20c
			 0x9bf91850, /// 0x210
			 0xc7797c6c, /// 0x214
			 0x36723618, /// 0x218
			 0x375da6d9, /// 0x21c
			 0xa9ee307c, /// 0x220
			 0xe1623858, /// 0x224
			 0x9edada83, /// 0x228
			 0xe535f11b, /// 0x22c
			 0x66ae75f6, /// 0x230
			 0x51cbeb38, /// 0x234
			 0xbb53b8bd, /// 0x238
			 0x40d0556a, /// 0x23c
			 0xba769d70, /// 0x240
			 0x2a2e008e, /// 0x244
			 0x55abc1d3, /// 0x248
			 0x75f43e79, /// 0x24c
			 0x2896cd97, /// 0x250
			 0x8757d5a0, /// 0x254
			 0xdefd75fd, /// 0x258
			 0xc4bee22a, /// 0x25c
			 0x96d9ccdc, /// 0x260
			 0x152b2ac8, /// 0x264
			 0x73ff38ff, /// 0x268
			 0x6bb935c0, /// 0x26c
			 0x55290c2a, /// 0x270
			 0x3c4e4a7a, /// 0x274
			 0xc69e877f, /// 0x278
			 0xab051fdb, /// 0x27c
			 0x8f4f0e88, /// 0x280
			 0x9d90b15c, /// 0x284
			 0x3d4762e8, /// 0x288
			 0xab6a8a98, /// 0x28c
			 0x7c73a811, /// 0x290
			 0x632ff67e, /// 0x294
			 0xd5e67680, /// 0x298
			 0x5f9ea8db, /// 0x29c
			 0x9d953007, /// 0x2a0
			 0x51a99d90, /// 0x2a4
			 0xfa22fc48, /// 0x2a8
			 0xba7515ad, /// 0x2ac
			 0xc6f66a1f, /// 0x2b0
			 0xfa8a73bc, /// 0x2b4
			 0x3a1d2f3d, /// 0x2b8
			 0xf6cf678a, /// 0x2bc
			 0xe32a0c62, /// 0x2c0
			 0x046fa879, /// 0x2c4
			 0xa62d4553, /// 0x2c8
			 0xdcb12bcd, /// 0x2cc
			 0xdfc5b55d, /// 0x2d0
			 0xbf5cf94e, /// 0x2d4
			 0xcade8726, /// 0x2d8
			 0xcb55a62b, /// 0x2dc
			 0x5d3e5123, /// 0x2e0
			 0x72e85be5, /// 0x2e4
			 0xb0da34d7, /// 0x2e8
			 0xe9bd8f36, /// 0x2ec
			 0x3220e37e, /// 0x2f0
			 0xda32e0c1, /// 0x2f4
			 0x9de6465b, /// 0x2f8
			 0xbfd608b2, /// 0x2fc
			 0x68cf1123, /// 0x300
			 0x9b6764d2, /// 0x304
			 0x89ff0333, /// 0x308
			 0x06b8c57a, /// 0x30c
			 0x8a7cd02e, /// 0x310
			 0x7104e96c, /// 0x314
			 0x390c8ba4, /// 0x318
			 0x25a611e6, /// 0x31c
			 0x777a59ce, /// 0x320
			 0xb17f7199, /// 0x324
			 0x6f080684, /// 0x328
			 0xcd9cc49d, /// 0x32c
			 0x831cfc62, /// 0x330
			 0x55f1eedd, /// 0x334
			 0xfb355e99, /// 0x338
			 0xd0a27af9, /// 0x33c
			 0xfe742a1f, /// 0x340
			 0x4e88332d, /// 0x344
			 0xa5652fa8, /// 0x348
			 0xaccbc51c, /// 0x34c
			 0x39b7c0ba, /// 0x350
			 0x13622fb1, /// 0x354
			 0x2f8570d0, /// 0x358
			 0x6973947b, /// 0x35c
			 0xd96bd832, /// 0x360
			 0x48f7e026, /// 0x364
			 0x558f611a, /// 0x368
			 0xbe5d3fd9, /// 0x36c
			 0xa866c5ac, /// 0x370
			 0x242218d8, /// 0x374
			 0xeb5cee53, /// 0x378
			 0x1d4bb6ce, /// 0x37c
			 0x4285d315, /// 0x380
			 0x55aefe3c, /// 0x384
			 0x51b27099, /// 0x388
			 0xfa891624, /// 0x38c
			 0x8b34db3a, /// 0x390
			 0xd13004ec, /// 0x394
			 0x5b76972e, /// 0x398
			 0xa7aa22f1, /// 0x39c
			 0xc5283f96, /// 0x3a0
			 0xcd8b2dcc, /// 0x3a4
			 0x35344cf7, /// 0x3a8
			 0x68aaf3e3, /// 0x3ac
			 0x5e5ef494, /// 0x3b0
			 0x1babdbbd, /// 0x3b4
			 0xd4150d49, /// 0x3b8
			 0xf6035dfe, /// 0x3bc
			 0xa207f53f, /// 0x3c0
			 0x176552da, /// 0x3c4
			 0x6011d244, /// 0x3c8
			 0x51d745be, /// 0x3cc
			 0x7a246d66, /// 0x3d0
			 0x493ce8a5, /// 0x3d4
			 0x54d4d25b, /// 0x3d8
			 0x7288829e, /// 0x3dc
			 0xc3c32044, /// 0x3e0
			 0x02dea311, /// 0x3e4
			 0x2a733071, /// 0x3e8
			 0xefdbb699, /// 0x3ec
			 0x802a240b, /// 0x3f0
			 0x0c9c1baa, /// 0x3f4
			 0xe5abf77f, /// 0x3f8
			 0x158fb256, /// 0x3fc
			 0xb5fb5fec, /// 0x400
			 0xd559515c, /// 0x404
			 0x6fad315e, /// 0x408
			 0x48957661, /// 0x40c
			 0xb74b08d0, /// 0x410
			 0x13e4277e, /// 0x414
			 0x645c4635, /// 0x418
			 0x3fdbc6fd, /// 0x41c
			 0x0fcb8a11, /// 0x420
			 0x19219ef2, /// 0x424
			 0x2a5f89a9, /// 0x428
			 0xf8ad19bc, /// 0x42c
			 0x792a3c17, /// 0x430
			 0x46371c4c, /// 0x434
			 0x95e55838, /// 0x438
			 0xc4ba66ad, /// 0x43c
			 0xc05a66ea, /// 0x440
			 0x1a59707e, /// 0x444
			 0x6a33b4a2, /// 0x448
			 0xa09d6d4f, /// 0x44c
			 0x45fef894, /// 0x450
			 0x44a2b399, /// 0x454
			 0x09d5ca51, /// 0x458
			 0x23ae6a91, /// 0x45c
			 0x9c06c247, /// 0x460
			 0xbd71a15f, /// 0x464
			 0x8cff1d8d, /// 0x468
			 0x25eaf979, /// 0x46c
			 0x73f4728a, /// 0x470
			 0x9126d1be, /// 0x474
			 0xef885d72, /// 0x478
			 0x10ec5045, /// 0x47c
			 0x9d434814, /// 0x480
			 0xb88db25b, /// 0x484
			 0xf5e57baa, /// 0x488
			 0x622d17a6, /// 0x48c
			 0x05499d2f, /// 0x490
			 0x77bcd7aa, /// 0x494
			 0x05d3b539, /// 0x498
			 0xe8e85234, /// 0x49c
			 0x2032ec0c, /// 0x4a0
			 0x24fa9406, /// 0x4a4
			 0x8c79d98c, /// 0x4a8
			 0x367c96a0, /// 0x4ac
			 0xf524efd7, /// 0x4b0
			 0x582daae5, /// 0x4b4
			 0xc77b4508, /// 0x4b8
			 0xf905e3de, /// 0x4bc
			 0xfb416208, /// 0x4c0
			 0xf9ec34fa, /// 0x4c4
			 0x4422e40a, /// 0x4c8
			 0xe27b19fd, /// 0x4cc
			 0x9491547f, /// 0x4d0
			 0xbba803e0, /// 0x4d4
			 0x8884d04f, /// 0x4d8
			 0x1c2bbfb7, /// 0x4dc
			 0x4a1c9cf7, /// 0x4e0
			 0x287985e4, /// 0x4e4
			 0x76fd5687, /// 0x4e8
			 0x28a30a56, /// 0x4ec
			 0x63f58a49, /// 0x4f0
			 0x4398cdd5, /// 0x4f4
			 0x812c660a, /// 0x4f8
			 0x4eda452c, /// 0x4fc
			 0x36fb6e02, /// 0x500
			 0x0456e25b, /// 0x504
			 0xacf5601d, /// 0x508
			 0x68d57514, /// 0x50c
			 0x6e184777, /// 0x510
			 0xc144baab, /// 0x514
			 0x62cce5d4, /// 0x518
			 0x18555d7b, /// 0x51c
			 0xb9273afc, /// 0x520
			 0x040bc4ed, /// 0x524
			 0x85c55bff, /// 0x528
			 0xf6d2e666, /// 0x52c
			 0x2a5201f8, /// 0x530
			 0x9ddedfb9, /// 0x534
			 0xad6db884, /// 0x538
			 0x7c6741c4, /// 0x53c
			 0x2183012e, /// 0x540
			 0x9daa0cfc, /// 0x544
			 0x581525c2, /// 0x548
			 0xc1019696, /// 0x54c
			 0x210ff83b, /// 0x550
			 0x61403ad8, /// 0x554
			 0x4895a081, /// 0x558
			 0x8b74627d, /// 0x55c
			 0x06da7960, /// 0x560
			 0x49eb30a1, /// 0x564
			 0x334bc4ed, /// 0x568
			 0x569572d7, /// 0x56c
			 0x763dffbd, /// 0x570
			 0x93e237a6, /// 0x574
			 0xcb6ae255, /// 0x578
			 0x221e2ec7, /// 0x57c
			 0x13756a3e, /// 0x580
			 0x5ed8e85a, /// 0x584
			 0xfdeed476, /// 0x588
			 0x0768ee28, /// 0x58c
			 0xaee05c2d, /// 0x590
			 0x3af40fb9, /// 0x594
			 0x11c8ab74, /// 0x598
			 0x107c5aa3, /// 0x59c
			 0xd8f76b32, /// 0x5a0
			 0xd9343736, /// 0x5a4
			 0xbeda62f2, /// 0x5a8
			 0x991c235d, /// 0x5ac
			 0x0636a5b1, /// 0x5b0
			 0xf234bf92, /// 0x5b4
			 0x17c60262, /// 0x5b8
			 0xe24033ea, /// 0x5bc
			 0xc575759f, /// 0x5c0
			 0xb314db44, /// 0x5c4
			 0x53105bdb, /// 0x5c8
			 0x4c484b89, /// 0x5cc
			 0xa3ad5b48, /// 0x5d0
			 0x557addc3, /// 0x5d4
			 0xbf1a09a6, /// 0x5d8
			 0xdea9c05f, /// 0x5dc
			 0x71ddb745, /// 0x5e0
			 0x91d9a8fe, /// 0x5e4
			 0xc7727907, /// 0x5e8
			 0xc2eb438e, /// 0x5ec
			 0x39d2c764, /// 0x5f0
			 0xa3ce44cc, /// 0x5f4
			 0x10c8ac2a, /// 0x5f8
			 0xf172168e, /// 0x5fc
			 0xc8df3b83, /// 0x600
			 0x236293a8, /// 0x604
			 0xa9afd637, /// 0x608
			 0x711e663b, /// 0x60c
			 0xf2052062, /// 0x610
			 0x16ee6f1a, /// 0x614
			 0x5544557a, /// 0x618
			 0x9f1cd025, /// 0x61c
			 0x8b7e06f3, /// 0x620
			 0x289b5a19, /// 0x624
			 0xb4fea920, /// 0x628
			 0xae6fd008, /// 0x62c
			 0x8e7f193c, /// 0x630
			 0xedcb3902, /// 0x634
			 0x7247638f, /// 0x638
			 0x3cd141a4, /// 0x63c
			 0x1303eb23, /// 0x640
			 0x2dc8ae9f, /// 0x644
			 0x1f28e680, /// 0x648
			 0x0e59a8b9, /// 0x64c
			 0x8710cdaf, /// 0x650
			 0xf1c629b0, /// 0x654
			 0x88d8de43, /// 0x658
			 0xaddf0290, /// 0x65c
			 0x4bdb581b, /// 0x660
			 0x6397fe12, /// 0x664
			 0x8ab5db6f, /// 0x668
			 0xfd85acf7, /// 0x66c
			 0x032b81bb, /// 0x670
			 0xcf19c289, /// 0x674
			 0x65781be5, /// 0x678
			 0xb476f479, /// 0x67c
			 0xb4f8f3ea, /// 0x680
			 0x37202f6e, /// 0x684
			 0xb961097e, /// 0x688
			 0xe270e3dc, /// 0x68c
			 0xe7953feb, /// 0x690
			 0x1731e045, /// 0x694
			 0x9bd5c87e, /// 0x698
			 0xb0ed5629, /// 0x69c
			 0x61eacdaa, /// 0x6a0
			 0xe11feab6, /// 0x6a4
			 0x71c3fac1, /// 0x6a8
			 0xb8998b53, /// 0x6ac
			 0x5f34700b, /// 0x6b0
			 0x4bbf3f37, /// 0x6b4
			 0x68fd0fbf, /// 0x6b8
			 0xca2b1990, /// 0x6bc
			 0x776b874b, /// 0x6c0
			 0x626d7eb0, /// 0x6c4
			 0x39ad4aca, /// 0x6c8
			 0x7cb33175, /// 0x6cc
			 0xd21de2d3, /// 0x6d0
			 0xc5658bfa, /// 0x6d4
			 0xaca93321, /// 0x6d8
			 0x4fea6109, /// 0x6dc
			 0xe8761c72, /// 0x6e0
			 0xd199a46d, /// 0x6e4
			 0x4e4b8ea6, /// 0x6e8
			 0xc25188bd, /// 0x6ec
			 0xaf88533a, /// 0x6f0
			 0x48fda565, /// 0x6f4
			 0xdc26fafe, /// 0x6f8
			 0x3090b2b8, /// 0x6fc
			 0x5f5bc073, /// 0x700
			 0xbf936477, /// 0x704
			 0x073963ab, /// 0x708
			 0x9a0ce4d6, /// 0x70c
			 0xadf3707e, /// 0x710
			 0xc80ecf80, /// 0x714
			 0x21031677, /// 0x718
			 0xcbe44fc3, /// 0x71c
			 0x49923ddd, /// 0x720
			 0xaf3862be, /// 0x724
			 0xe889b3b3, /// 0x728
			 0x6a6690bc, /// 0x72c
			 0xc62b4ab6, /// 0x730
			 0xafccf8aa, /// 0x734
			 0x4bc74597, /// 0x738
			 0x72a56f60, /// 0x73c
			 0xd6373551, /// 0x740
			 0xe2966251, /// 0x744
			 0x46f567ae, /// 0x748
			 0xae32e373, /// 0x74c
			 0x38638c4a, /// 0x750
			 0x3d2ca2de, /// 0x754
			 0x8749d4fb, /// 0x758
			 0x0cf63642, /// 0x75c
			 0x1e720567, /// 0x760
			 0x304747c1, /// 0x764
			 0xc3362888, /// 0x768
			 0x0eccdb76, /// 0x76c
			 0x31e3bfa9, /// 0x770
			 0xbd1b3718, /// 0x774
			 0xa342dc13, /// 0x778
			 0x79b72b9c, /// 0x77c
			 0xa0fb0c50, /// 0x780
			 0x205c8a8c, /// 0x784
			 0x9ca38fa1, /// 0x788
			 0xb11a7482, /// 0x78c
			 0x19273276, /// 0x790
			 0x6fe44949, /// 0x794
			 0xa0693738, /// 0x798
			 0x63a67461, /// 0x79c
			 0x388b6cd7, /// 0x7a0
			 0xd2436b7e, /// 0x7a4
			 0x6147acc7, /// 0x7a8
			 0xea1f9448, /// 0x7ac
			 0x3a65ea2d, /// 0x7b0
			 0x8f7ee187, /// 0x7b4
			 0xc6bc9b76, /// 0x7b8
			 0x2d024183, /// 0x7bc
			 0xc787721a, /// 0x7c0
			 0x7e606838, /// 0x7c4
			 0xea104ec6, /// 0x7c8
			 0x593afa9a, /// 0x7cc
			 0xf84dcd91, /// 0x7d0
			 0x9cd6d3a4, /// 0x7d4
			 0x12321d2b, /// 0x7d8
			 0x6735f49e, /// 0x7dc
			 0x608b8c5c, /// 0x7e0
			 0x33c6286d, /// 0x7e4
			 0xa10d7962, /// 0x7e8
			 0xa942c397, /// 0x7ec
			 0x7a6f6adf, /// 0x7f0
			 0x09e1d497, /// 0x7f4
			 0xc7b8b587, /// 0x7f8
			 0xc490fdfd, /// 0x7fc
			 0xd079c9fa, /// 0x800
			 0x75113e5c, /// 0x804
			 0x787d4859, /// 0x808
			 0x093519b8, /// 0x80c
			 0xe8e34a5a, /// 0x810
			 0x5f5099f8, /// 0x814
			 0x5cc1a08e, /// 0x818
			 0x40c0b8b4, /// 0x81c
			 0x5c1ee9b7, /// 0x820
			 0x6088f350, /// 0x824
			 0x0d4e489c, /// 0x828
			 0xaa31364c, /// 0x82c
			 0xe19008e1, /// 0x830
			 0x695df7ce, /// 0x834
			 0x54809063, /// 0x838
			 0x7d4c99a5, /// 0x83c
			 0x2698c0ef, /// 0x840
			 0x42ccd19d, /// 0x844
			 0x6e2bc711, /// 0x848
			 0x6514bfa0, /// 0x84c
			 0x6f10153f, /// 0x850
			 0x53d2c90d, /// 0x854
			 0xa6a4ce77, /// 0x858
			 0xcd4d5fab, /// 0x85c
			 0xa3cd4720, /// 0x860
			 0xbe4c6ea6, /// 0x864
			 0x30067f6a, /// 0x868
			 0x0d9cfb68, /// 0x86c
			 0x85386cd8, /// 0x870
			 0xca3297ba, /// 0x874
			 0x58e86ece, /// 0x878
			 0xfe90f8b3, /// 0x87c
			 0x7275c9e6, /// 0x880
			 0xfe774fec, /// 0x884
			 0xb3188beb, /// 0x888
			 0x026919fe, /// 0x88c
			 0xb6b8b6fe, /// 0x890
			 0x2a114c93, /// 0x894
			 0x8fbd38e0, /// 0x898
			 0xc9c5e439, /// 0x89c
			 0xf345d6d3, /// 0x8a0
			 0x002e10c7, /// 0x8a4
			 0x2479f134, /// 0x8a8
			 0x0fa2e9da, /// 0x8ac
			 0x03933060, /// 0x8b0
			 0xf79e4ca0, /// 0x8b4
			 0x1d5965df, /// 0x8b8
			 0x1334d2a5, /// 0x8bc
			 0xa45daba7, /// 0x8c0
			 0x5a1defc9, /// 0x8c4
			 0xaaca9352, /// 0x8c8
			 0x9d85ae02, /// 0x8cc
			 0xb8868ec5, /// 0x8d0
			 0xcafec411, /// 0x8d4
			 0x99111dd2, /// 0x8d8
			 0xae7b4375, /// 0x8dc
			 0x3d45d5f8, /// 0x8e0
			 0xaf670ff1, /// 0x8e4
			 0xe5367590, /// 0x8e8
			 0x394cac9f, /// 0x8ec
			 0xb9115897, /// 0x8f0
			 0x9a122222, /// 0x8f4
			 0x5c08c9af, /// 0x8f8
			 0x4b68fc05, /// 0x8fc
			 0x3b568e47, /// 0x900
			 0xa078c9e0, /// 0x904
			 0x2dff473e, /// 0x908
			 0x2e80282f, /// 0x90c
			 0x80c1404e, /// 0x910
			 0x8af1659e, /// 0x914
			 0xf44ba210, /// 0x918
			 0xd7a5e989, /// 0x91c
			 0x2ccdaa4c, /// 0x920
			 0x1df693b6, /// 0x924
			 0x631faf81, /// 0x928
			 0xd4026303, /// 0x92c
			 0xf25b8ca8, /// 0x930
			 0x73ca7db3, /// 0x934
			 0x71d59eee, /// 0x938
			 0x3f324660, /// 0x93c
			 0x0b3086d8, /// 0x940
			 0x3c019d31, /// 0x944
			 0xa8e4f85a, /// 0x948
			 0x3d92089e, /// 0x94c
			 0x5ca43ff5, /// 0x950
			 0x5ee2379e, /// 0x954
			 0x17ed458c, /// 0x958
			 0xe2921107, /// 0x95c
			 0xa78b0b8a, /// 0x960
			 0x9bb58c39, /// 0x964
			 0x9ea7a5cc, /// 0x968
			 0xbab7c281, /// 0x96c
			 0x4f3dbf3c, /// 0x970
			 0xa4f1538d, /// 0x974
			 0xb3b48dfb, /// 0x978
			 0x1754247f, /// 0x97c
			 0x818a5a16, /// 0x980
			 0x0c3c0c7e, /// 0x984
			 0xea72c770, /// 0x988
			 0x02df7d53, /// 0x98c
			 0x6ea7b847, /// 0x990
			 0xda7d4cce, /// 0x994
			 0xa844455b, /// 0x998
			 0x9027848a, /// 0x99c
			 0x70f245dd, /// 0x9a0
			 0x808f8d21, /// 0x9a4
			 0x6b122452, /// 0x9a8
			 0x226a27f4, /// 0x9ac
			 0xc5497b7a, /// 0x9b0
			 0x18e975da, /// 0x9b4
			 0x1883d1cc, /// 0x9b8
			 0xab561e2f, /// 0x9bc
			 0x03e1e2a3, /// 0x9c0
			 0x45b653be, /// 0x9c4
			 0xc0779abf, /// 0x9c8
			 0xf93ae816, /// 0x9cc
			 0x547238ac, /// 0x9d0
			 0x5f87c6cb, /// 0x9d4
			 0x9dec8cf3, /// 0x9d8
			 0xced19b70, /// 0x9dc
			 0x0e5f95fa, /// 0x9e0
			 0x70cb017c, /// 0x9e4
			 0xb80f439e, /// 0x9e8
			 0xfd501309, /// 0x9ec
			 0xf983af92, /// 0x9f0
			 0x5be8cbfd, /// 0x9f4
			 0xab8cb18c, /// 0x9f8
			 0x223a8c65, /// 0x9fc
			 0x237457e4, /// 0xa00
			 0x691b6c8f, /// 0xa04
			 0x453d64ab, /// 0xa08
			 0xc52b499e, /// 0xa0c
			 0xfdd5ddd6, /// 0xa10
			 0x49b5f7b6, /// 0xa14
			 0xb24a9187, /// 0xa18
			 0x4db4bf07, /// 0xa1c
			 0xfdf900a9, /// 0xa20
			 0xe8f34dae, /// 0xa24
			 0xf28a2ff0, /// 0xa28
			 0x4c89e756, /// 0xa2c
			 0xf3894c9f, /// 0xa30
			 0x7ddbfb76, /// 0xa34
			 0xf3c70df8, /// 0xa38
			 0x1238c836, /// 0xa3c
			 0x0a951dc4, /// 0xa40
			 0xc693c966, /// 0xa44
			 0x28ad1931, /// 0xa48
			 0xb6b258c3, /// 0xa4c
			 0xee8ff520, /// 0xa50
			 0xe0ce34d9, /// 0xa54
			 0x2b2b277d, /// 0xa58
			 0x355f2fae, /// 0xa5c
			 0xe57a447e, /// 0xa60
			 0x83e9a140, /// 0xa64
			 0x6edbd0ec, /// 0xa68
			 0x233796f7, /// 0xa6c
			 0x439b3be8, /// 0xa70
			 0xd7bed08a, /// 0xa74
			 0xd93e416c, /// 0xa78
			 0xc47edc4e, /// 0xa7c
			 0xd82ddaad, /// 0xa80
			 0xfb1eec39, /// 0xa84
			 0xa70d3d3c, /// 0xa88
			 0xa32cc458, /// 0xa8c
			 0x5d526524, /// 0xa90
			 0x3e5dec71, /// 0xa94
			 0x0a80a3ef, /// 0xa98
			 0x6ad383da, /// 0xa9c
			 0x6020d770, /// 0xaa0
			 0x27a0faf0, /// 0xaa4
			 0x6d129822, /// 0xaa8
			 0x4a670ae1, /// 0xaac
			 0xf252742b, /// 0xab0
			 0x92f8368c, /// 0xab4
			 0x2b9e8c76, /// 0xab8
			 0xe1c15187, /// 0xabc
			 0xac6f725b, /// 0xac0
			 0xa52b9396, /// 0xac4
			 0x920f46ce, /// 0xac8
			 0x50060f90, /// 0xacc
			 0x2b42c8b5, /// 0xad0
			 0x7ca28c5f, /// 0xad4
			 0x48b498d5, /// 0xad8
			 0x214731be, /// 0xadc
			 0x7613655c, /// 0xae0
			 0xc548aef6, /// 0xae4
			 0xe82e7486, /// 0xae8
			 0x4e142c19, /// 0xaec
			 0x7e58f50c, /// 0xaf0
			 0x1491ac74, /// 0xaf4
			 0xda55ace4, /// 0xaf8
			 0x82cfce5c, /// 0xafc
			 0x18b65514, /// 0xb00
			 0x3d484ea7, /// 0xb04
			 0x0587a935, /// 0xb08
			 0x6194846d, /// 0xb0c
			 0xfa91e3d7, /// 0xb10
			 0x773c3db6, /// 0xb14
			 0x726e897b, /// 0xb18
			 0xc8d5122d, /// 0xb1c
			 0x366a6076, /// 0xb20
			 0x61dd7931, /// 0xb24
			 0xffc3782f, /// 0xb28
			 0x7c3147e2, /// 0xb2c
			 0x83b19009, /// 0xb30
			 0x9aa3bd95, /// 0xb34
			 0x086d0064, /// 0xb38
			 0x025524e0, /// 0xb3c
			 0x3a23ea87, /// 0xb40
			 0x4a74d7bb, /// 0xb44
			 0x89916e9e, /// 0xb48
			 0xc3ca7041, /// 0xb4c
			 0x640afc9d, /// 0xb50
			 0x733e9e7c, /// 0xb54
			 0xee00ddf9, /// 0xb58
			 0x4d395c61, /// 0xb5c
			 0x144b8e49, /// 0xb60
			 0x5722d206, /// 0xb64
			 0x522960f5, /// 0xb68
			 0x8f7b4a07, /// 0xb6c
			 0x97193423, /// 0xb70
			 0x4b6268d7, /// 0xb74
			 0x6d1a7896, /// 0xb78
			 0x0ae40d85, /// 0xb7c
			 0x6fd1a756, /// 0xb80
			 0x3ae83f83, /// 0xb84
			 0xfe630cd5, /// 0xb88
			 0xabf97e6c, /// 0xb8c
			 0xe3a9a45f, /// 0xb90
			 0x9780ee65, /// 0xb94
			 0xba4e18a8, /// 0xb98
			 0xf040b60f, /// 0xb9c
			 0x24a95493, /// 0xba0
			 0xdca77c08, /// 0xba4
			 0x3269e639, /// 0xba8
			 0xaaf75d6e, /// 0xbac
			 0xf873714a, /// 0xbb0
			 0x21241c53, /// 0xbb4
			 0xf0dcf73c, /// 0xbb8
			 0x54ba39b8, /// 0xbbc
			 0xbd37d84f, /// 0xbc0
			 0xe6cea927, /// 0xbc4
			 0x632ed86d, /// 0xbc8
			 0x091cde7c, /// 0xbcc
			 0x5fb6e0a3, /// 0xbd0
			 0x8c082748, /// 0xbd4
			 0x67babb88, /// 0xbd8
			 0x70b5e70f, /// 0xbdc
			 0xeb36c08d, /// 0xbe0
			 0xf5c50795, /// 0xbe4
			 0xd9115273, /// 0xbe8
			 0xdad0d0f9, /// 0xbec
			 0xf739bbd3, /// 0xbf0
			 0x955f4835, /// 0xbf4
			 0x5d3acb86, /// 0xbf8
			 0x024de9a5, /// 0xbfc
			 0xc144b3c6, /// 0xc00
			 0x16f58045, /// 0xc04
			 0xcf28f65e, /// 0xc08
			 0x4c08ae3f, /// 0xc0c
			 0xa7734956, /// 0xc10
			 0x98ab7d15, /// 0xc14
			 0xf7e1f997, /// 0xc18
			 0x6f8116ae, /// 0xc1c
			 0x9d483a13, /// 0xc20
			 0x4806e670, /// 0xc24
			 0x86a746c8, /// 0xc28
			 0x95f1204e, /// 0xc2c
			 0xf6707ee5, /// 0xc30
			 0xbd3877a7, /// 0xc34
			 0x1a8e5fd4, /// 0xc38
			 0x4fda75a7, /// 0xc3c
			 0xfcdd2f7f, /// 0xc40
			 0x79d749dc, /// 0xc44
			 0xeda8b27a, /// 0xc48
			 0xe8a69bd5, /// 0xc4c
			 0x5f2c6d4b, /// 0xc50
			 0xb1f5faed, /// 0xc54
			 0x2b73fb72, /// 0xc58
			 0x1a4c6802, /// 0xc5c
			 0x71a123fe, /// 0xc60
			 0xd2a49d32, /// 0xc64
			 0x2c064f46, /// 0xc68
			 0x593c6933, /// 0xc6c
			 0x44ecc0e0, /// 0xc70
			 0x12df574f, /// 0xc74
			 0xdc50f350, /// 0xc78
			 0x08db4578, /// 0xc7c
			 0xd7cb05db, /// 0xc80
			 0xe5dc7dc2, /// 0xc84
			 0xe1a9e0bb, /// 0xc88
			 0xfb66262e, /// 0xc8c
			 0x1f520606, /// 0xc90
			 0x6d7d3d4a, /// 0xc94
			 0x76e4a2db, /// 0xc98
			 0x812d3616, /// 0xc9c
			 0x285a639d, /// 0xca0
			 0xb0f1b707, /// 0xca4
			 0xb485517d, /// 0xca8
			 0x84f2442a, /// 0xcac
			 0x8175715f, /// 0xcb0
			 0x24096037, /// 0xcb4
			 0xf790cee5, /// 0xcb8
			 0x2895c954, /// 0xcbc
			 0xaaf07222, /// 0xcc0
			 0xab9816f2, /// 0xcc4
			 0x52926028, /// 0xcc8
			 0x95d8db3b, /// 0xccc
			 0x5f2dbad2, /// 0xcd0
			 0x08dde9d4, /// 0xcd4
			 0x7b44a8b1, /// 0xcd8
			 0x75565da4, /// 0xcdc
			 0xe185a555, /// 0xce0
			 0xf4a5cae1, /// 0xce4
			 0x0edc3106, /// 0xce8
			 0x60a312d2, /// 0xcec
			 0xc5b52f19, /// 0xcf0
			 0x0bd8bc26, /// 0xcf4
			 0xbd3256df, /// 0xcf8
			 0xf1f0485e, /// 0xcfc
			 0x45e52dfb, /// 0xd00
			 0xf2a812fb, /// 0xd04
			 0xb3033328, /// 0xd08
			 0xabc0bd0c, /// 0xd0c
			 0x986650e9, /// 0xd10
			 0x1fe86dc3, /// 0xd14
			 0x9dfbc08a, /// 0xd18
			 0xac9fe5f9, /// 0xd1c
			 0x3f92bf0f, /// 0xd20
			 0x227f469e, /// 0xd24
			 0x52b5003e, /// 0xd28
			 0xafa83398, /// 0xd2c
			 0x93564f75, /// 0xd30
			 0xfe01a9ef, /// 0xd34
			 0x7e39bda1, /// 0xd38
			 0xdf71a5a7, /// 0xd3c
			 0x36298bd6, /// 0xd40
			 0x84e59792, /// 0xd44
			 0x65b02247, /// 0xd48
			 0x5609cb35, /// 0xd4c
			 0x6390db41, /// 0xd50
			 0x0e1994e0, /// 0xd54
			 0xf3db5595, /// 0xd58
			 0x6205ff05, /// 0xd5c
			 0x294582ba, /// 0xd60
			 0xdb1fb242, /// 0xd64
			 0x011ba3c1, /// 0xd68
			 0xb9126c5c, /// 0xd6c
			 0x33a34b05, /// 0xd70
			 0xe9addf9a, /// 0xd74
			 0x27f23890, /// 0xd78
			 0x836ff270, /// 0xd7c
			 0xc61729c8, /// 0xd80
			 0xf31aca58, /// 0xd84
			 0x647d5313, /// 0xd88
			 0xae0d00ae, /// 0xd8c
			 0xd30e1874, /// 0xd90
			 0xb69940b9, /// 0xd94
			 0x8a11e237, /// 0xd98
			 0x9cd97166, /// 0xd9c
			 0x1e6d7cd1, /// 0xda0
			 0x22881f39, /// 0xda4
			 0x444ebb1b, /// 0xda8
			 0x9248aceb, /// 0xdac
			 0x114fe1f6, /// 0xdb0
			 0x217e7f2c, /// 0xdb4
			 0xf75d1db6, /// 0xdb8
			 0x80fed109, /// 0xdbc
			 0xaec3e5dc, /// 0xdc0
			 0x84afc697, /// 0xdc4
			 0xc5f67e0d, /// 0xdc8
			 0xc0361f08, /// 0xdcc
			 0x6e37d401, /// 0xdd0
			 0x49c537f5, /// 0xdd4
			 0x589709a6, /// 0xdd8
			 0x03485dc8, /// 0xddc
			 0x4b07bd5c, /// 0xde0
			 0x89058d9f, /// 0xde4
			 0x6ead3b52, /// 0xde8
			 0xf5c57134, /// 0xdec
			 0xf23c4a61, /// 0xdf0
			 0xddb6e65e, /// 0xdf4
			 0x7674b7dc, /// 0xdf8
			 0xd62b9afc, /// 0xdfc
			 0xa8f2ae10, /// 0xe00
			 0xa7e22fb8, /// 0xe04
			 0x2a9803ac, /// 0xe08
			 0x4ba139b0, /// 0xe0c
			 0xb832c642, /// 0xe10
			 0xb42abf5b, /// 0xe14
			 0x8a1cbbc9, /// 0xe18
			 0x15054198, /// 0xe1c
			 0x8e5927e9, /// 0xe20
			 0x6642c614, /// 0xe24
			 0xf341cbc8, /// 0xe28
			 0x687586f5, /// 0xe2c
			 0x4baa89aa, /// 0xe30
			 0x98b3acbe, /// 0xe34
			 0xc922fe2e, /// 0xe38
			 0x97f513f0, /// 0xe3c
			 0x17b2afdf, /// 0xe40
			 0xd01630c5, /// 0xe44
			 0x0a43beca, /// 0xe48
			 0x2e8befce, /// 0xe4c
			 0x9d8eaaf8, /// 0xe50
			 0x2c29b942, /// 0xe54
			 0xd4d98b85, /// 0xe58
			 0x17f72535, /// 0xe5c
			 0x507ca62d, /// 0xe60
			 0x7163e4e0, /// 0xe64
			 0x8371c578, /// 0xe68
			 0x6618957f, /// 0xe6c
			 0x8970e02c, /// 0xe70
			 0x46684923, /// 0xe74
			 0x36cd795c, /// 0xe78
			 0x0d9222e4, /// 0xe7c
			 0xd4f79b8c, /// 0xe80
			 0x7f21a8a1, /// 0xe84
			 0x855fbcc5, /// 0xe88
			 0x1655fcb3, /// 0xe8c
			 0x690591a0, /// 0xe90
			 0xb73d7a33, /// 0xe94
			 0x7ae4d96a, /// 0xe98
			 0x646f6dbb, /// 0xe9c
			 0xc0794c50, /// 0xea0
			 0xf4be07b6, /// 0xea4
			 0x5411a333, /// 0xea8
			 0xc6e34c66, /// 0xeac
			 0x963a32d0, /// 0xeb0
			 0x4daa22a0, /// 0xeb4
			 0x429f463a, /// 0xeb8
			 0x2c2075b9, /// 0xebc
			 0x4db0fbfb, /// 0xec0
			 0xdf50b5eb, /// 0xec4
			 0xb691bd6c, /// 0xec8
			 0xe0698d2f, /// 0xecc
			 0xa8886129, /// 0xed0
			 0x0db802ff, /// 0xed4
			 0x71b91fb1, /// 0xed8
			 0x73ed67dc, /// 0xedc
			 0x62e5b6fe, /// 0xee0
			 0x639b6545, /// 0xee4
			 0xcf984392, /// 0xee8
			 0x51058365, /// 0xeec
			 0x36bd25bb, /// 0xef0
			 0xbd07b5ee, /// 0xef4
			 0xaa00829e, /// 0xef8
			 0x8f2c9dcb, /// 0xefc
			 0x07d57e0e, /// 0xf00
			 0x1f2f665c, /// 0xf04
			 0xd3edaed9, /// 0xf08
			 0x538d5238, /// 0xf0c
			 0x1cd2cf58, /// 0xf10
			 0x92191f76, /// 0xf14
			 0x252e81e1, /// 0xf18
			 0x37eac280, /// 0xf1c
			 0x9a719af2, /// 0xf20
			 0x0ed83fc9, /// 0xf24
			 0x39543519, /// 0xf28
			 0x3cd26936, /// 0xf2c
			 0x7eb838c7, /// 0xf30
			 0x67b2a76a, /// 0xf34
			 0x4b486c93, /// 0xf38
			 0xb0135538, /// 0xf3c
			 0x9f7f1e97, /// 0xf40
			 0x932bca64, /// 0xf44
			 0xb02c456b, /// 0xf48
			 0xb1b22631, /// 0xf4c
			 0x80500b76, /// 0xf50
			 0x2b1cee62, /// 0xf54
			 0xeab87f7a, /// 0xf58
			 0x43f1cd33, /// 0xf5c
			 0x6592b05a, /// 0xf60
			 0x827a8d74, /// 0xf64
			 0xaae1dadf, /// 0xf68
			 0x1216ab89, /// 0xf6c
			 0x50a79192, /// 0xf70
			 0x4cc999bc, /// 0xf74
			 0xa3b23dd7, /// 0xf78
			 0x975a015c, /// 0xf7c
			 0xe8e84224, /// 0xf80
			 0x43f5eee4, /// 0xf84
			 0x0f29a269, /// 0xf88
			 0x6e176560, /// 0xf8c
			 0xc5ba412c, /// 0xf90
			 0x5580e9d7, /// 0xf94
			 0xc2ee71f6, /// 0xf98
			 0xf50db850, /// 0xf9c
			 0xa81b4c6c, /// 0xfa0
			 0xb67a7f14, /// 0xfa4
			 0xbde6ebb3, /// 0xfa8
			 0x85ee98eb, /// 0xfac
			 0x19c413ec, /// 0xfb0
			 0x8b85e8a9, /// 0xfb4
			 0x0c050249, /// 0xfb8
			 0x63a324d6, /// 0xfbc
			 0x8a216ee1, /// 0xfc0
			 0xd4d1c5dd, /// 0xfc4
			 0xeb02c94b, /// 0xfc8
			 0xce31d891, /// 0xfcc
			 0x477af3d6, /// 0xfd0
			 0xd51f5d37, /// 0xfd4
			 0xf496504d, /// 0xfd8
			 0xc1b9308c, /// 0xfdc
			 0x026dd24f, /// 0xfe0
			 0x55e2b071, /// 0xfe4
			 0x7332beb4, /// 0xfe8
			 0x9c11df25, /// 0xfec
			 0x7a7ca93a, /// 0xff0
			 0xacea4489, /// 0xff4
			 0xab6a83f5, /// 0xff8
			 0xf594e1e1 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0e000003,                                                  // Trailing 1s:                                /// 00000
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00008
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0000c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00010
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00014
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00018
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0001c
			 0x4b000000,                                                  // 8388608.0                                   /// 00020
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00024
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00028
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0002c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00030
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00034
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x33333333,                                                  // 4 random values                             /// 0003c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00040
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00044
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00048
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0004c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00054
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00058
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0005c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00064
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00068
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0006c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00070
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00078
			 0x55555555,                                                  // 4 random values                             /// 0007c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00080
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00090
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00098
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0009c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x33333333,                                                  // 4 random values                             /// 000a8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000b4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000b8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000bc
			 0x00011111,                                                  // 9.7958E-41                                  /// 000c0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000c4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000c8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000cc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000d0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000d4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000dc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000e0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000e4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000e8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000ec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000f4
			 0x0e000003,                                                  // Trailing 1s:                                /// 000f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000fc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00100
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00104
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00108
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0010c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00110
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00114
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00118
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0011c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00124
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0012c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00130
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00134
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00140
			 0x0e000001,                                                  // Trailing 1s:                                /// 00144
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00148
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0014c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00150
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00154
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00158
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00160
			 0x0c700000,                                                  // Leading 1s:                                 /// 00164
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00168
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0016c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00170
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00174
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00178
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0017c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00180
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00184
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00188
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0018c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00194
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0019c
			 0xbf028f5c,                                                  // -0.51                                       /// 001a0
			 0x0c780000,                                                  // Leading 1s:                                 /// 001a4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001a8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001b4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001b8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001c0
			 0x00000000,                                                  // zero                                        /// 001c4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001c8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001cc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 001d8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001e4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001ec
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001f0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001f4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001f8
			 0x7fc00001,                                                  // signaling NaN                               /// 001fc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00204
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00208
			 0xffc00001,                                                  // -signaling NaN                              /// 0020c
			 0xcb000000,                                                  // -8388608.0                                  /// 00210
			 0xcb000000,                                                  // -8388608.0                                  /// 00214
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00220
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00224
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x55555555,                                                  // 4 random values                             /// 0022c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00230
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00234
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00238
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0023c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00240
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00248
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00254
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00258
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0025c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00260
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00268
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0026c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00270
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00278
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x00011111,                                                  // 9.7958E-41                                  /// 00284
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00288
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0028c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00290
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00294
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0029c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002b0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002b8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002c0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002c4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002c8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002cc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002d4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002d8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002e0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002e8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002ec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002f0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002f4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002f8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00300
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00304
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00308
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0030c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00314
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00318
			 0x4b000000,                                                  // 8388608.0                                   /// 0031c
			 0xbf028f5c,                                                  // -0.51                                       /// 00320
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00324
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0032c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00330
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00338
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00340
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00344
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00348
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00354
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0035c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00360
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00364
			 0x0c700000,                                                  // Leading 1s:                                 /// 00368
			 0x0e000007,                                                  // Trailing 1s:                                /// 0036c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00374
			 0x00011111,                                                  // 9.7958E-41                                  /// 00378
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0037c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00384
			 0x0c400000,                                                  // Leading 1s:                                 /// 00388
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00390
			 0x80000000,                                                  // -zero                                       /// 00394
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00398
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003a0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003a4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003a8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003b0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003bc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003c4
			 0xffc00001,                                                  // -signaling NaN                              /// 003c8
			 0x00000000,                                                  // zero                                        /// 003cc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003d4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003d8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003dc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003e0
			 0x33333333,                                                  // 4 random values                             /// 003e4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003e8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003ec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003f0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003f4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003f8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003fc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00404
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00408
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00410
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00414
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00418
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00420
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00428
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00430
			 0x7fc00001,                                                  // signaling NaN                               /// 00434
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00438
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00440
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00444
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00448
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0044c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00450
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0045c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00464
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00468
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0046c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00474
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00478
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00480
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00484
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00488
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0048c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00490
			 0x0e000003,                                                  // Trailing 1s:                                /// 00494
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00498
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0049c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004a0
			 0xcb000000,                                                  // -8388608.0                                  /// 004a4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004a8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004ac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004b0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004b8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004c0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004c4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004d4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004d8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004dc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004ec
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004f0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004fc
			 0x55555555,                                                  // 4 random values                             /// 00500
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00504
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00508
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0050c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00510
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00514
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00518
			 0x0e000001,                                                  // Trailing 1s:                                /// 0051c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00520
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00524
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0052c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00530
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00538
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00544
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00550
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00558
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00560
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00564
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00568
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0056c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00570
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0x00000000,                                                  // zero                                        /// 00578
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0057c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00584
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00590
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00594
			 0x7fc00001,                                                  // signaling NaN                               /// 00598
			 0xcb000000,                                                  // -8388608.0                                  /// 0059c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005a0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005a8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005ac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005b8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005bc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005c0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005c4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005cc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005d0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005d4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005dc
			 0x0c780000,                                                  // Leading 1s:                                 /// 005e0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005f0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005f8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005fc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00600
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00604
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00608
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0060c
			 0xffc00001,                                                  // -signaling NaN                              /// 00610
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00614
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00618
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00620
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00624
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0062c
			 0x00000000,                                                  // zero                                        /// 00630
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00634
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00638
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0063c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00640
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00644
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00648
			 0xbf028f5c,                                                  // -0.51                                       /// 0064c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00658
			 0xffc00001,                                                  // -signaling NaN                              /// 0065c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00660
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00670
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00678
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0067c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00680
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00688
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0068c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0069c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006a0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006a8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006ac
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006b0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x3f028f5c,                                                  // 0.51                                        /// 006bc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006c0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006c4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006c8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006d0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006d4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006d8
			 0x00000000,                                                  // zero                                        /// 006dc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006e0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006fc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00700
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00704
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00708
			 0x7f800000,                                                  // inf                                         /// 0070c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00710
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00714
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00718
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0071c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00720
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00728
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0072c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x4b000000,                                                  // 8388608.0                                   /// 00734
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0073c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00740
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00744
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00748
			 0x00000000,                                                  // zero                                        /// 0074c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00754
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00758
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0075c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00760
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00764
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00768
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0076c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00778
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0077c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00780
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00784
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00788
			 0xbf028f5c,                                                  // -0.51                                       /// 0078c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0xff800000,                                                  // -inf                                        /// 00794
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0079c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007a4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007a8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007ac
			 0x0e000007,                                                  // Trailing 1s:                                /// 007b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007b4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007b8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007bc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x0e000003,                                                  // Trailing 1s:                                /// 007c4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007c8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007cc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007d4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007d8
			 0x80011111,                                                  // -9.7958E-41                                 /// 007dc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007e4
			 0x7f800000,                                                  // inf                                         /// 007e8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007ec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007f0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x0c780000,                                                  // Leading 1s:                                 /// 007f8
			 0x0c400000,                                                  // Leading 1s:                                 /// 007fc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00800
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00808
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0080c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00818
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0081c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00820
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00824
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00828
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0082c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00838
			 0x0c600000,                                                  // Leading 1s:                                 /// 0083c
			 0x00000000,                                                  // zero                                        /// 00840
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00844
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00848
			 0x0e000007,                                                  // Trailing 1s:                                /// 0084c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00850
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00854
			 0x0c780000,                                                  // Leading 1s:                                 /// 00858
			 0xffc00001,                                                  // -signaling NaN                              /// 0085c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x0c700000,                                                  // Leading 1s:                                 /// 00864
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00868
			 0x0e000001,                                                  // Trailing 1s:                                /// 0086c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00870
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00874
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00878
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00880
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00884
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00888
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0088c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00890
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00894
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00898
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0089c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008a4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008a8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008b0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008b4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008b8
			 0xffc00001,                                                  // -signaling NaN                              /// 008bc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 008c8
			 0x0e000001,                                                  // Trailing 1s:                                /// 008cc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008d0
			 0x3f028f5c,                                                  // 0.51                                        /// 008d4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008d8
			 0x0e000001,                                                  // Trailing 1s:                                /// 008dc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008e0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008e8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008ec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008f0
			 0xcb000000,                                                  // -8388608.0                                  /// 008f4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008f8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008fc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00900
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00904
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00908
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0090c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00910
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00914
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0091c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00920
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00924
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00928
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00930
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00934
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00938
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0093c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00940
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00944
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00948
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0094c
			 0x4b000000,                                                  // 8388608.0                                   /// 00950
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00954
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00958
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00960
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0096c
			 0x80000000,                                                  // -zero                                       /// 00970
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00974
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00978
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0097c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00980
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x00000000,                                                  // zero                                        /// 00988
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0098c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00994
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00998
			 0x00011111,                                                  // 9.7958E-41                                  /// 0099c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009a0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009a4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0xff800000,                                                  // -inf                                        /// 009ac
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009b0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009b4
			 0xbf028f5c,                                                  // -0.51                                       /// 009b8
			 0x55555555,                                                  // 4 random values                             /// 009bc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009cc
			 0x0e000003,                                                  // Trailing 1s:                                /// 009d0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009d4
			 0x33333333,                                                  // 4 random values                             /// 009d8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009dc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x0e000007,                                                  // Trailing 1s:                                /// 009e4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009e8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009f0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009f8
			 0x7fc00001,                                                  // signaling NaN                               /// 009fc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a04
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a08
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a10
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a14
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a18
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a1c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a20
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a24
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a28
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a2c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a30
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a34
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a38
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a3c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a40
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a44
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a48
			 0xbf028f5c,                                                  // -0.51                                       /// 00a4c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a50
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a54
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a58
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a64
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a68
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a74
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a78
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a7c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a80
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a84
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a8c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a90
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a94
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a98
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00aa0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00aa4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00aa8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ab0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ab4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00abc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ac0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ac4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00acc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ad0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ad4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ad8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ae0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ae4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ae8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00aec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00af4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00afc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b00
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b04
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b08
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b0c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b10
			 0xff800000,                                                  // -inf                                        /// 00b14
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b18
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b1c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b20
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b24
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b2c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b30
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b3c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b40
			 0xff800000,                                                  // -inf                                        /// 00b44
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b48
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b4c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b50
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b58
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x33333333,                                                  // 4 random values                             /// 00b60
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b64
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b6c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b7c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b80
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b8c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b94
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b98
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b9c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ba4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bac
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bb0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bb8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bc0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bc8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bcc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bd4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00be0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bf0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bf8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bfc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c08
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c10
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c1c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c20
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c24
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c28
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c30
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c34
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c38
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c3c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c40
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c44
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c48
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c4c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c50
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c5c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c60
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c64
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c68
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c6c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c70
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c74
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c78
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c7c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c80
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c84
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c8c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c90
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c94
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c98
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c9c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ca8
			 0x7f800000,                                                  // inf                                         /// 00cac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cb0
			 0x4b000000,                                                  // 8388608.0                                   /// 00cb4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cb8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cbc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cc0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00cc4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cc8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ccc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00cd0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cd4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cd8
			 0x7f800000,                                                  // inf                                         /// 00cdc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ce0
			 0xbf028f5c,                                                  // -0.51                                       /// 00ce4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ce8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cec
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cf0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cf4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cf8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cfc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d00
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d04
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d08
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d0c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d10
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d14
			 0x7f800000,                                                  // inf                                         /// 00d18
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d1c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d20
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d24
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d2c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d34
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d38
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d44
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d48
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d4c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d50
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d54
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d58
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d5c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d60
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d68
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d6c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x7fc00001,                                                  // signaling NaN                               /// 00d78
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d80
			 0xff800000,                                                  // -inf                                        /// 00d84
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x55555555,                                                  // 4 random values                             /// 00d90
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d94
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0xff800000,                                                  // -inf                                        /// 00da0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00da8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00dac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00db0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00db4
			 0xff800000,                                                  // -inf                                        /// 00db8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00dbc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00dc0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dc4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00dc8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dcc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00dd4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00dd8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ddc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00de4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00dec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00df0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00df4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00df8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00dfc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e04
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e10
			 0x7fc00001,                                                  // signaling NaN                               /// 00e14
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e24
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e28
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e2c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e30
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e34
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e3c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e40
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e44
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e48
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e4c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e50
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e54
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e58
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e64
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e68
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e6c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e70
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e74
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e78
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e7c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e80
			 0x7fc00001,                                                  // signaling NaN                               /// 00e84
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e88
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e8c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e94
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e9c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ea4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ea8
			 0x33333333,                                                  // 4 random values                             /// 00eac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00eb0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00eb4
			 0x4b000000,                                                  // 8388608.0                                   /// 00eb8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ebc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ec0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ec4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ec8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ecc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ed0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x80000000,                                                  // -zero                                       /// 00ed8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00edc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ee0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ee8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00eec
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ef0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ef8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00efc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f04
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f08
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f18
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f1c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f20
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f2c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f34
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f38
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f3c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f40
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f44
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f4c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f50
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f54
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f60
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f64
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f68
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f6c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x7f800000,                                                  // inf                                         /// 00f7c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f80
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f84
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f88
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f8c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f90
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fa4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fa8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00fac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fb4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fb8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fbc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fc0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fc4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fc8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fcc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fd0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fd4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fdc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fe0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fe4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fe8
			 0x80000000,                                                  // -zero                                       /// 00fec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ff4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ff8
			 0xffc00001 // QNan                                          // SP - ve numbers                             /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x6aba54f3, /// 0x0
			 0xdb2bbb6b, /// 0x4
			 0x390b5b17, /// 0x8
			 0x6dbc2b27, /// 0xc
			 0xbf69a77d, /// 0x10
			 0x209275e0, /// 0x14
			 0x5a977460, /// 0x18
			 0x58d18049, /// 0x1c
			 0x66aaf4d9, /// 0x20
			 0x72b21a6c, /// 0x24
			 0x9be18977, /// 0x28
			 0x0bd34401, /// 0x2c
			 0x9fa936de, /// 0x30
			 0x4a7fa9a4, /// 0x34
			 0x38f44950, /// 0x38
			 0x000a303f, /// 0x3c
			 0x74619786, /// 0x40
			 0x853fad81, /// 0x44
			 0x1340e140, /// 0x48
			 0x49568b85, /// 0x4c
			 0x8991429f, /// 0x50
			 0x7cf45586, /// 0x54
			 0xec634565, /// 0x58
			 0xd20827f5, /// 0x5c
			 0x39f5e386, /// 0x60
			 0x14077987, /// 0x64
			 0x3183e1d7, /// 0x68
			 0x63f0f06a, /// 0x6c
			 0x9b731fd0, /// 0x70
			 0xb55d68dd, /// 0x74
			 0x5bd2ee52, /// 0x78
			 0x0bfc878d, /// 0x7c
			 0x2fbb7734, /// 0x80
			 0x342c11be, /// 0x84
			 0xd308745a, /// 0x88
			 0x5333f075, /// 0x8c
			 0xc422542c, /// 0x90
			 0xd04416fb, /// 0x94
			 0xd61bfe72, /// 0x98
			 0xa16e9ab9, /// 0x9c
			 0x23fa5336, /// 0xa0
			 0x4c3f73f5, /// 0xa4
			 0xdbf4e849, /// 0xa8
			 0x50aa95ae, /// 0xac
			 0xb7b363a5, /// 0xb0
			 0xfc9194e8, /// 0xb4
			 0x3b784590, /// 0xb8
			 0xff334820, /// 0xbc
			 0xaaeabded, /// 0xc0
			 0x79607175, /// 0xc4
			 0xd8344347, /// 0xc8
			 0xf69312b6, /// 0xcc
			 0x5e42e6ab, /// 0xd0
			 0x0abde119, /// 0xd4
			 0x732a614b, /// 0xd8
			 0x7fb58554, /// 0xdc
			 0x12e2d773, /// 0xe0
			 0x40bf070d, /// 0xe4
			 0xab065b37, /// 0xe8
			 0x27443ce7, /// 0xec
			 0x935b04ad, /// 0xf0
			 0x41994fb1, /// 0xf4
			 0xf993637f, /// 0xf8
			 0x940098b7, /// 0xfc
			 0x2522201e, /// 0x100
			 0x7c98e6c0, /// 0x104
			 0x4aca59ff, /// 0x108
			 0x27aba11a, /// 0x10c
			 0x721deaae, /// 0x110
			 0xd582dd6d, /// 0x114
			 0xb8440583, /// 0x118
			 0x3a2d5287, /// 0x11c
			 0xc604281a, /// 0x120
			 0x5a16a870, /// 0x124
			 0x82f8b2ae, /// 0x128
			 0xb01f8d42, /// 0x12c
			 0x729cf578, /// 0x130
			 0x284c0c81, /// 0x134
			 0x51dcf535, /// 0x138
			 0x67e6aee1, /// 0x13c
			 0xaa8f9df0, /// 0x140
			 0xd201d507, /// 0x144
			 0x8ff38b65, /// 0x148
			 0xab1593af, /// 0x14c
			 0x5984f466, /// 0x150
			 0xe8d2b947, /// 0x154
			 0xcbfcdbf7, /// 0x158
			 0x15112a2d, /// 0x15c
			 0xfb250e01, /// 0x160
			 0x8a5355df, /// 0x164
			 0x12a2f06b, /// 0x168
			 0x43226aaf, /// 0x16c
			 0x518140a7, /// 0x170
			 0xfe29cbb3, /// 0x174
			 0x3acc0e9b, /// 0x178
			 0xfcc6a1c3, /// 0x17c
			 0x1c9f27c1, /// 0x180
			 0x315605eb, /// 0x184
			 0x4640462d, /// 0x188
			 0x1e460946, /// 0x18c
			 0xa3d2fcf0, /// 0x190
			 0xb1372be8, /// 0x194
			 0xb93e4bc6, /// 0x198
			 0x638627cb, /// 0x19c
			 0x37172bc3, /// 0x1a0
			 0x2598e19e, /// 0x1a4
			 0x09c0bd7f, /// 0x1a8
			 0xb9c3c6b5, /// 0x1ac
			 0x6e56c6ad, /// 0x1b0
			 0xd83ba5ef, /// 0x1b4
			 0x33141455, /// 0x1b8
			 0x09e76df1, /// 0x1bc
			 0xbf92d98a, /// 0x1c0
			 0xb7c56601, /// 0x1c4
			 0x58a41447, /// 0x1c8
			 0xf1310b5d, /// 0x1cc
			 0x76acede6, /// 0x1d0
			 0xfef22f66, /// 0x1d4
			 0x667015f0, /// 0x1d8
			 0xe8ff46ea, /// 0x1dc
			 0x68fdad1b, /// 0x1e0
			 0x63079124, /// 0x1e4
			 0xd98a1e1c, /// 0x1e8
			 0xe000e2cb, /// 0x1ec
			 0xd0eb476b, /// 0x1f0
			 0x50421235, /// 0x1f4
			 0x1ff9cd6e, /// 0x1f8
			 0x3bcbb9a2, /// 0x1fc
			 0xf786f5bb, /// 0x200
			 0x28758dd0, /// 0x204
			 0x30df110b, /// 0x208
			 0x9f7f25f9, /// 0x20c
			 0xdce33506, /// 0x210
			 0xb84b1a10, /// 0x214
			 0xc651f330, /// 0x218
			 0xeeb08f69, /// 0x21c
			 0x8c2125bf, /// 0x220
			 0x5d789fe7, /// 0x224
			 0x898ed82f, /// 0x228
			 0xc86d0181, /// 0x22c
			 0x57dbf95d, /// 0x230
			 0xb938cc7d, /// 0x234
			 0x9d440498, /// 0x238
			 0x2b99e270, /// 0x23c
			 0x497647a6, /// 0x240
			 0x50008e9b, /// 0x244
			 0xb546650f, /// 0x248
			 0x38ec7ba3, /// 0x24c
			 0x2341e56c, /// 0x250
			 0x045b51b5, /// 0x254
			 0x51d711a6, /// 0x258
			 0xf3a0100a, /// 0x25c
			 0x696d5114, /// 0x260
			 0x58b7f7fb, /// 0x264
			 0xdb05e551, /// 0x268
			 0xd54a28f0, /// 0x26c
			 0xddd951df, /// 0x270
			 0xb9e17abb, /// 0x274
			 0xf749ff1e, /// 0x278
			 0x83079552, /// 0x27c
			 0x37a95718, /// 0x280
			 0x44b9c901, /// 0x284
			 0x43e570b5, /// 0x288
			 0x925d8bf2, /// 0x28c
			 0xeb251784, /// 0x290
			 0x31ce43a0, /// 0x294
			 0xda10b039, /// 0x298
			 0xe165ebe7, /// 0x29c
			 0x656cbb17, /// 0x2a0
			 0x42181fd6, /// 0x2a4
			 0x1e6e050f, /// 0x2a8
			 0xd66e192d, /// 0x2ac
			 0x8e500e86, /// 0x2b0
			 0x4449b0b1, /// 0x2b4
			 0x79d9322d, /// 0x2b8
			 0xeb860318, /// 0x2bc
			 0x7b185a88, /// 0x2c0
			 0xdc9d9318, /// 0x2c4
			 0x478a52f8, /// 0x2c8
			 0xa2bb6292, /// 0x2cc
			 0xd2a284f6, /// 0x2d0
			 0x3a9892f2, /// 0x2d4
			 0x805427c8, /// 0x2d8
			 0x876c539a, /// 0x2dc
			 0x56ec46c7, /// 0x2e0
			 0x260ac78d, /// 0x2e4
			 0xdb772292, /// 0x2e8
			 0x6d13f587, /// 0x2ec
			 0x7e7297cc, /// 0x2f0
			 0x04ae9835, /// 0x2f4
			 0x017d5986, /// 0x2f8
			 0xc0de4d4e, /// 0x2fc
			 0xbd7607ee, /// 0x300
			 0xe8003096, /// 0x304
			 0xe0357918, /// 0x308
			 0x90b1f97c, /// 0x30c
			 0xdddc019b, /// 0x310
			 0x4df43bbb, /// 0x314
			 0x1e256f7a, /// 0x318
			 0xd814b075, /// 0x31c
			 0xa03bf128, /// 0x320
			 0x99460fdc, /// 0x324
			 0x382d9dfa, /// 0x328
			 0x26172b9b, /// 0x32c
			 0x496e7f5c, /// 0x330
			 0x3944d7ea, /// 0x334
			 0xc026995c, /// 0x338
			 0xaff5a948, /// 0x33c
			 0x8303f63f, /// 0x340
			 0xe30494e7, /// 0x344
			 0x88455e73, /// 0x348
			 0xd2fabfdd, /// 0x34c
			 0x3d383441, /// 0x350
			 0xf54ae719, /// 0x354
			 0x853595ef, /// 0x358
			 0x86d3822a, /// 0x35c
			 0xc575848a, /// 0x360
			 0x9e50d9b8, /// 0x364
			 0x13e9cf44, /// 0x368
			 0xa72e71f0, /// 0x36c
			 0xf1075d6f, /// 0x370
			 0x37ee173f, /// 0x374
			 0x8325700c, /// 0x378
			 0xcf361615, /// 0x37c
			 0x63f11652, /// 0x380
			 0xbea6e935, /// 0x384
			 0x13018a9c, /// 0x388
			 0xcc76a5ff, /// 0x38c
			 0x6cd67517, /// 0x390
			 0x392ef64f, /// 0x394
			 0x5008ed18, /// 0x398
			 0x3cc49b25, /// 0x39c
			 0xc791f96a, /// 0x3a0
			 0x585e76e8, /// 0x3a4
			 0x1b618c26, /// 0x3a8
			 0x278f2a14, /// 0x3ac
			 0x6be7781e, /// 0x3b0
			 0xbbbaf554, /// 0x3b4
			 0xaf599795, /// 0x3b8
			 0xbf4ac129, /// 0x3bc
			 0x8772b6fb, /// 0x3c0
			 0x51ebcf0b, /// 0x3c4
			 0xd92355de, /// 0x3c8
			 0x1cab0588, /// 0x3cc
			 0x4b3d4a9c, /// 0x3d0
			 0x1532d66b, /// 0x3d4
			 0xebb5e99a, /// 0x3d8
			 0x4ee9b296, /// 0x3dc
			 0x9956f586, /// 0x3e0
			 0xb0ba5dbb, /// 0x3e4
			 0x931e03e0, /// 0x3e8
			 0x2a2d2790, /// 0x3ec
			 0xcce201e1, /// 0x3f0
			 0x043a7189, /// 0x3f4
			 0xefafd8cd, /// 0x3f8
			 0xa830e8a8, /// 0x3fc
			 0x30dd712d, /// 0x400
			 0x5c15d69b, /// 0x404
			 0xe2c71b81, /// 0x408
			 0x743fe268, /// 0x40c
			 0x05398168, /// 0x410
			 0x2e8c8dbb, /// 0x414
			 0x47848971, /// 0x418
			 0xe9397f41, /// 0x41c
			 0x35d3a315, /// 0x420
			 0xaa1db730, /// 0x424
			 0x03531c29, /// 0x428
			 0x51622ecd, /// 0x42c
			 0xb07fd62a, /// 0x430
			 0x5784f784, /// 0x434
			 0x81bbab52, /// 0x438
			 0x4b07f612, /// 0x43c
			 0x50e9bbe4, /// 0x440
			 0xd121eb19, /// 0x444
			 0x4c578656, /// 0x448
			 0x0a0aee62, /// 0x44c
			 0x0c566f4f, /// 0x450
			 0xbd8189e1, /// 0x454
			 0x017b1189, /// 0x458
			 0xd70a6c84, /// 0x45c
			 0xa080dd9c, /// 0x460
			 0xddcf4f09, /// 0x464
			 0xf5382da5, /// 0x468
			 0xbdd8b2ee, /// 0x46c
			 0x800c79c8, /// 0x470
			 0x8aa1c600, /// 0x474
			 0x3ddcba4c, /// 0x478
			 0xdfd30d79, /// 0x47c
			 0x9671bb26, /// 0x480
			 0x266e9a90, /// 0x484
			 0x1eb10e81, /// 0x488
			 0x74990d2e, /// 0x48c
			 0x07199e19, /// 0x490
			 0x48b170bf, /// 0x494
			 0xc749f5d0, /// 0x498
			 0x87b1e1ac, /// 0x49c
			 0xe337554e, /// 0x4a0
			 0xf6ce491c, /// 0x4a4
			 0xd2124084, /// 0x4a8
			 0x4a72dcb4, /// 0x4ac
			 0xf597fcb9, /// 0x4b0
			 0xfaeb290e, /// 0x4b4
			 0x73f21bcf, /// 0x4b8
			 0xe74e730f, /// 0x4bc
			 0x3273a778, /// 0x4c0
			 0xa215452f, /// 0x4c4
			 0x6eafed25, /// 0x4c8
			 0x5ad4abc9, /// 0x4cc
			 0x5f553a48, /// 0x4d0
			 0x2344ccf8, /// 0x4d4
			 0xa6aa1807, /// 0x4d8
			 0xf2f17021, /// 0x4dc
			 0xf44ecc1f, /// 0x4e0
			 0x8b1ed84f, /// 0x4e4
			 0x6cac14b4, /// 0x4e8
			 0xc7d03888, /// 0x4ec
			 0x4f96c646, /// 0x4f0
			 0xe4c52488, /// 0x4f4
			 0x9441691a, /// 0x4f8
			 0xe038dd54, /// 0x4fc
			 0x1b72dcdb, /// 0x500
			 0xde212ed1, /// 0x504
			 0x9a3bf18f, /// 0x508
			 0x351a4838, /// 0x50c
			 0x2916203b, /// 0x510
			 0x230933bd, /// 0x514
			 0xbd882e03, /// 0x518
			 0x93b98d05, /// 0x51c
			 0x46279e0e, /// 0x520
			 0x6139fa8f, /// 0x524
			 0xa0816472, /// 0x528
			 0x6e73d46b, /// 0x52c
			 0x6e55b539, /// 0x530
			 0x0c0c0496, /// 0x534
			 0xcee04b51, /// 0x538
			 0xedfe5e7b, /// 0x53c
			 0xf140daac, /// 0x540
			 0xbbcf5284, /// 0x544
			 0xb5f17058, /// 0x548
			 0x80f26d21, /// 0x54c
			 0xfae3ea21, /// 0x550
			 0x930e8062, /// 0x554
			 0x517f3b07, /// 0x558
			 0x74d5e00c, /// 0x55c
			 0x4d43cbe9, /// 0x560
			 0x56690789, /// 0x564
			 0xf7097715, /// 0x568
			 0x8b982669, /// 0x56c
			 0x2b6932b1, /// 0x570
			 0x34d9a310, /// 0x574
			 0x2a60a665, /// 0x578
			 0x17f40a34, /// 0x57c
			 0x66009d80, /// 0x580
			 0x53ceb420, /// 0x584
			 0xdfc664fc, /// 0x588
			 0x8adc07cb, /// 0x58c
			 0x1db74a37, /// 0x590
			 0x04446bd3, /// 0x594
			 0x7d61ae93, /// 0x598
			 0x1552cd14, /// 0x59c
			 0x7a3a894a, /// 0x5a0
			 0x3fa4d86e, /// 0x5a4
			 0x2ef5175c, /// 0x5a8
			 0xfdb4223d, /// 0x5ac
			 0xb10844f6, /// 0x5b0
			 0x248aa05a, /// 0x5b4
			 0xd92d6c5b, /// 0x5b8
			 0x7e45b940, /// 0x5bc
			 0xb79d2a0c, /// 0x5c0
			 0xf9650028, /// 0x5c4
			 0x8d6fb6f9, /// 0x5c8
			 0xfe9ade02, /// 0x5cc
			 0x8c120db6, /// 0x5d0
			 0xc26c0760, /// 0x5d4
			 0x1b06813a, /// 0x5d8
			 0x74ed3726, /// 0x5dc
			 0xe4e58ad4, /// 0x5e0
			 0x301efbb6, /// 0x5e4
			 0x9713ce09, /// 0x5e8
			 0x41883076, /// 0x5ec
			 0x98f8d226, /// 0x5f0
			 0x298bad35, /// 0x5f4
			 0xe94467bb, /// 0x5f8
			 0x133f4dac, /// 0x5fc
			 0x4a27a845, /// 0x600
			 0x7d62ec10, /// 0x604
			 0x96474b8d, /// 0x608
			 0x2114c25d, /// 0x60c
			 0xac6a7276, /// 0x610
			 0xe7023490, /// 0x614
			 0x859ca027, /// 0x618
			 0xcbaa84e9, /// 0x61c
			 0xc64b9ee9, /// 0x620
			 0xd6cb8a4f, /// 0x624
			 0x71e8be24, /// 0x628
			 0x21b87c84, /// 0x62c
			 0x99281c3f, /// 0x630
			 0xa04d8441, /// 0x634
			 0x2c6dfe16, /// 0x638
			 0x9fdce292, /// 0x63c
			 0x82de35b4, /// 0x640
			 0x59a18ef2, /// 0x644
			 0x9edae4bc, /// 0x648
			 0x260334dd, /// 0x64c
			 0xd3ed804d, /// 0x650
			 0x7737aa83, /// 0x654
			 0xea474af5, /// 0x658
			 0xdae85bb6, /// 0x65c
			 0xcd49cb11, /// 0x660
			 0x37f65adf, /// 0x664
			 0x3776d764, /// 0x668
			 0x359672b0, /// 0x66c
			 0x32bf07e4, /// 0x670
			 0xf6bf5964, /// 0x674
			 0x96a2e589, /// 0x678
			 0x94966e44, /// 0x67c
			 0xa108c865, /// 0x680
			 0x78c10a3d, /// 0x684
			 0x76cde77f, /// 0x688
			 0x0caae2f2, /// 0x68c
			 0x3556de08, /// 0x690
			 0xf4814a69, /// 0x694
			 0xb8211de2, /// 0x698
			 0x9da930ec, /// 0x69c
			 0xe74ab162, /// 0x6a0
			 0x8ca0bf41, /// 0x6a4
			 0xb79111e2, /// 0x6a8
			 0x9bb6bfd6, /// 0x6ac
			 0x61f1cac6, /// 0x6b0
			 0x94d45dd6, /// 0x6b4
			 0xc42394db, /// 0x6b8
			 0x1c200314, /// 0x6bc
			 0xc21110fd, /// 0x6c0
			 0x000e2f77, /// 0x6c4
			 0xa9c47c4e, /// 0x6c8
			 0x563e011a, /// 0x6cc
			 0xceaf69c7, /// 0x6d0
			 0x343627ce, /// 0x6d4
			 0xfd89fea9, /// 0x6d8
			 0x065c0bb0, /// 0x6dc
			 0x2ca70347, /// 0x6e0
			 0xcb6741da, /// 0x6e4
			 0x86ca9493, /// 0x6e8
			 0xe8d3e6e2, /// 0x6ec
			 0x6577e78b, /// 0x6f0
			 0x80cca5fd, /// 0x6f4
			 0x7fc78a3f, /// 0x6f8
			 0xa4d014c4, /// 0x6fc
			 0x5c837df0, /// 0x700
			 0x2eb6f206, /// 0x704
			 0x347963e8, /// 0x708
			 0xbb882015, /// 0x70c
			 0x7591a5bb, /// 0x710
			 0x5e6f5fa6, /// 0x714
			 0xeec3c356, /// 0x718
			 0x0a2b8ea7, /// 0x71c
			 0x4191b3f9, /// 0x720
			 0xf9b2ea8c, /// 0x724
			 0xa1d63ce5, /// 0x728
			 0x37edd232, /// 0x72c
			 0xb4593a93, /// 0x730
			 0x1f17f51d, /// 0x734
			 0xa40a7793, /// 0x738
			 0x71ef2868, /// 0x73c
			 0x4476bc49, /// 0x740
			 0xef063401, /// 0x744
			 0x785b6747, /// 0x748
			 0x01a6a381, /// 0x74c
			 0x32ea32d7, /// 0x750
			 0x4d9aee70, /// 0x754
			 0xd7b414a1, /// 0x758
			 0x80887669, /// 0x75c
			 0x3ab99faf, /// 0x760
			 0x3bd641f0, /// 0x764
			 0x7bc97ee3, /// 0x768
			 0xfbdd6c24, /// 0x76c
			 0x81b780ea, /// 0x770
			 0x5d0acae9, /// 0x774
			 0x4c890843, /// 0x778
			 0xbc6fbd0b, /// 0x77c
			 0x1e3ac2ce, /// 0x780
			 0xb6a65800, /// 0x784
			 0xc0888dbd, /// 0x788
			 0x8849b4c5, /// 0x78c
			 0x26f4ad35, /// 0x790
			 0x52091fe9, /// 0x794
			 0x66eb6eab, /// 0x798
			 0x8df8ce7f, /// 0x79c
			 0x8b8fbd6b, /// 0x7a0
			 0xa6717ec0, /// 0x7a4
			 0x551983c8, /// 0x7a8
			 0x2d4792f2, /// 0x7ac
			 0xb119e151, /// 0x7b0
			 0xe4b019cd, /// 0x7b4
			 0x2c0876bf, /// 0x7b8
			 0x6d4c25bd, /// 0x7bc
			 0x064484a5, /// 0x7c0
			 0x54f94e29, /// 0x7c4
			 0x95ebff0a, /// 0x7c8
			 0xc3ea285d, /// 0x7cc
			 0x3cf76215, /// 0x7d0
			 0x7fd37ba6, /// 0x7d4
			 0x3504d79d, /// 0x7d8
			 0x1192c131, /// 0x7dc
			 0xd76535ca, /// 0x7e0
			 0x4fc3dd97, /// 0x7e4
			 0xd4380448, /// 0x7e8
			 0x0db82496, /// 0x7ec
			 0xbb039d15, /// 0x7f0
			 0xe2053b4d, /// 0x7f4
			 0xf9d1ce15, /// 0x7f8
			 0x68c4c1d3, /// 0x7fc
			 0x06660e9c, /// 0x800
			 0x650da43b, /// 0x804
			 0xc83dfe60, /// 0x808
			 0x6d7cfaae, /// 0x80c
			 0xdab327f1, /// 0x810
			 0xc4a387a9, /// 0x814
			 0x0d86e337, /// 0x818
			 0x6c34653b, /// 0x81c
			 0x32abd34a, /// 0x820
			 0x53c9b862, /// 0x824
			 0x3f99671d, /// 0x828
			 0x821d62bc, /// 0x82c
			 0x353fa47d, /// 0x830
			 0x38a177f8, /// 0x834
			 0x87f5ba4f, /// 0x838
			 0xaf757eaf, /// 0x83c
			 0x6ad21bd7, /// 0x840
			 0xe87029f2, /// 0x844
			 0x2660427e, /// 0x848
			 0x81e61d89, /// 0x84c
			 0xc04fb652, /// 0x850
			 0x654c17ac, /// 0x854
			 0x1c2ebe66, /// 0x858
			 0xb453bab3, /// 0x85c
			 0x9d07426e, /// 0x860
			 0x1eb18805, /// 0x864
			 0xc7c6f30f, /// 0x868
			 0xddf8df74, /// 0x86c
			 0xc8d60417, /// 0x870
			 0x8d514952, /// 0x874
			 0x7e49de4a, /// 0x878
			 0xa918352a, /// 0x87c
			 0x2efd4d40, /// 0x880
			 0x72ecacc9, /// 0x884
			 0x5978d6bb, /// 0x888
			 0xbf814ad8, /// 0x88c
			 0xd2ab994c, /// 0x890
			 0x50560f3c, /// 0x894
			 0xc41ea9a0, /// 0x898
			 0x58907413, /// 0x89c
			 0x50ba4790, /// 0x8a0
			 0xd051c018, /// 0x8a4
			 0xac508c2f, /// 0x8a8
			 0xcb729d9f, /// 0x8ac
			 0xb4f8f273, /// 0x8b0
			 0xabd47880, /// 0x8b4
			 0x4ca35aa9, /// 0x8b8
			 0x39ef47e6, /// 0x8bc
			 0xf6b60f56, /// 0x8c0
			 0x0163d48f, /// 0x8c4
			 0xb1b0832c, /// 0x8c8
			 0x987e000f, /// 0x8cc
			 0xa5a1b5fb, /// 0x8d0
			 0xbf874b3c, /// 0x8d4
			 0x613646b0, /// 0x8d8
			 0x87788d3a, /// 0x8dc
			 0xe126575c, /// 0x8e0
			 0x938b2d51, /// 0x8e4
			 0x042ea1a7, /// 0x8e8
			 0x3f568e7e, /// 0x8ec
			 0x36838993, /// 0x8f0
			 0xd40b8c0b, /// 0x8f4
			 0x50f55bc3, /// 0x8f8
			 0xf606022b, /// 0x8fc
			 0xdff8bbf2, /// 0x900
			 0x3b9f58de, /// 0x904
			 0x59072448, /// 0x908
			 0xa7f1944d, /// 0x90c
			 0xdf5d470c, /// 0x910
			 0x79124cf7, /// 0x914
			 0x507031f6, /// 0x918
			 0x1b36ab36, /// 0x91c
			 0xce0ba150, /// 0x920
			 0x3b67c4c8, /// 0x924
			 0xd7c5a3f5, /// 0x928
			 0x51699d3b, /// 0x92c
			 0xb8ffa302, /// 0x930
			 0xc0c77e37, /// 0x934
			 0xa176f1e5, /// 0x938
			 0x230a7fb2, /// 0x93c
			 0x713ba121, /// 0x940
			 0xbb07a92c, /// 0x944
			 0xd8c88e7f, /// 0x948
			 0x23e56991, /// 0x94c
			 0xad2077ed, /// 0x950
			 0x7cfbc2ed, /// 0x954
			 0x68d0a27d, /// 0x958
			 0x647b4e7f, /// 0x95c
			 0xa5227cc0, /// 0x960
			 0x023c006c, /// 0x964
			 0x38ccc89b, /// 0x968
			 0xe520553c, /// 0x96c
			 0x610e6a74, /// 0x970
			 0xd4c631f4, /// 0x974
			 0xb7382be2, /// 0x978
			 0x12f10b6e, /// 0x97c
			 0x7638662d, /// 0x980
			 0x455feb01, /// 0x984
			 0x52166bf9, /// 0x988
			 0xbb14bd79, /// 0x98c
			 0xe3aeb061, /// 0x990
			 0x3951b341, /// 0x994
			 0x96f912a0, /// 0x998
			 0xf431b1b9, /// 0x99c
			 0x222448e2, /// 0x9a0
			 0x9caf6619, /// 0x9a4
			 0xc7e4b913, /// 0x9a8
			 0x53b0a2d1, /// 0x9ac
			 0x578262f0, /// 0x9b0
			 0xf8ae2e2c, /// 0x9b4
			 0xf49fc044, /// 0x9b8
			 0x85b9e758, /// 0x9bc
			 0xbd352ce2, /// 0x9c0
			 0x36cfa5c5, /// 0x9c4
			 0x3880d00e, /// 0x9c8
			 0x426dcbed, /// 0x9cc
			 0x7d952917, /// 0x9d0
			 0xd7083dc8, /// 0x9d4
			 0x8393b32e, /// 0x9d8
			 0xb11c5093, /// 0x9dc
			 0x6155c8ad, /// 0x9e0
			 0x5945fdf1, /// 0x9e4
			 0x3463d80e, /// 0x9e8
			 0x0402ddf6, /// 0x9ec
			 0x9fb9b7f4, /// 0x9f0
			 0xcadb5bcd, /// 0x9f4
			 0xb23d79fe, /// 0x9f8
			 0x84c1a355, /// 0x9fc
			 0x138b9189, /// 0xa00
			 0xe97f8ca4, /// 0xa04
			 0x33e03ebb, /// 0xa08
			 0xa1eaa9a8, /// 0xa0c
			 0x4c7a8ddd, /// 0xa10
			 0x2f0ece33, /// 0xa14
			 0xb48659fd, /// 0xa18
			 0xcbd29a2a, /// 0xa1c
			 0xf467beb4, /// 0xa20
			 0x25980472, /// 0xa24
			 0x4dcd2c80, /// 0xa28
			 0xe5692daa, /// 0xa2c
			 0xa030050e, /// 0xa30
			 0xd883cf00, /// 0xa34
			 0x2b54b8a0, /// 0xa38
			 0x133ab307, /// 0xa3c
			 0x31dfae94, /// 0xa40
			 0x8a6f37fe, /// 0xa44
			 0x2e727f21, /// 0xa48
			 0xaff97f4b, /// 0xa4c
			 0x8da9d1f5, /// 0xa50
			 0x71eee7ac, /// 0xa54
			 0x0131e635, /// 0xa58
			 0x4eb6c420, /// 0xa5c
			 0x52e52ea0, /// 0xa60
			 0xfdfc5783, /// 0xa64
			 0x4993559c, /// 0xa68
			 0x61fcbfb1, /// 0xa6c
			 0x9c1753b6, /// 0xa70
			 0xd78c47f7, /// 0xa74
			 0x5e03ad5c, /// 0xa78
			 0xf0f5dbef, /// 0xa7c
			 0x7b0359d8, /// 0xa80
			 0xfb384e02, /// 0xa84
			 0x118bff51, /// 0xa88
			 0x7203b844, /// 0xa8c
			 0x7ebff784, /// 0xa90
			 0x74ff5dbc, /// 0xa94
			 0x8568d20a, /// 0xa98
			 0x52014819, /// 0xa9c
			 0x9ae3f02f, /// 0xaa0
			 0xb8a194f3, /// 0xaa4
			 0xe95910dd, /// 0xaa8
			 0x6c2c089e, /// 0xaac
			 0x367a0605, /// 0xab0
			 0x4124b9e4, /// 0xab4
			 0x86776c7a, /// 0xab8
			 0x3af911c0, /// 0xabc
			 0xb054efa3, /// 0xac0
			 0xe014093b, /// 0xac4
			 0x7ad49db3, /// 0xac8
			 0xaf7b29a0, /// 0xacc
			 0x1ba3c86f, /// 0xad0
			 0xc41695f1, /// 0xad4
			 0x9f0a6dc9, /// 0xad8
			 0xd4a7e25e, /// 0xadc
			 0xe91dee33, /// 0xae0
			 0x91ab5988, /// 0xae4
			 0xa6fd21da, /// 0xae8
			 0xb57eadfd, /// 0xaec
			 0xf377ff70, /// 0xaf0
			 0x42154751, /// 0xaf4
			 0xec4d2228, /// 0xaf8
			 0x30331b55, /// 0xafc
			 0xcd32b55e, /// 0xb00
			 0xd5530f1a, /// 0xb04
			 0xaef30c00, /// 0xb08
			 0x53ce4106, /// 0xb0c
			 0xfb73ccd9, /// 0xb10
			 0x33f93df0, /// 0xb14
			 0xc43c8e30, /// 0xb18
			 0x7b916aa7, /// 0xb1c
			 0xc04f91f4, /// 0xb20
			 0x2fc43b2f, /// 0xb24
			 0x21da9308, /// 0xb28
			 0xfd22049e, /// 0xb2c
			 0xc276e374, /// 0xb30
			 0xa056bcc3, /// 0xb34
			 0xbedbbb0a, /// 0xb38
			 0x2d920586, /// 0xb3c
			 0xbde51d02, /// 0xb40
			 0xd4637ed6, /// 0xb44
			 0x3553e2b7, /// 0xb48
			 0x6ad5c775, /// 0xb4c
			 0xa84dcdde, /// 0xb50
			 0x92beae8a, /// 0xb54
			 0x7497d198, /// 0xb58
			 0x652db6f4, /// 0xb5c
			 0x6942aa62, /// 0xb60
			 0xc9eaa9b2, /// 0xb64
			 0x6b1a13a1, /// 0xb68
			 0xb97b0af1, /// 0xb6c
			 0xb13186bc, /// 0xb70
			 0x943e49fb, /// 0xb74
			 0xfb720808, /// 0xb78
			 0x37e1c08c, /// 0xb7c
			 0xf9a7c2ea, /// 0xb80
			 0x1aba135c, /// 0xb84
			 0xe202bb7a, /// 0xb88
			 0xbdd77d77, /// 0xb8c
			 0xc4d8fd8f, /// 0xb90
			 0xb066bdf9, /// 0xb94
			 0xb6e3abd9, /// 0xb98
			 0x5875597e, /// 0xb9c
			 0xfe3ec558, /// 0xba0
			 0xc16a98a2, /// 0xba4
			 0x8fe83884, /// 0xba8
			 0x33d3d10a, /// 0xbac
			 0x68b5ddb5, /// 0xbb0
			 0x7f974331, /// 0xbb4
			 0xf5f878cc, /// 0xbb8
			 0x5fec67a6, /// 0xbbc
			 0xf6dda35c, /// 0xbc0
			 0xa7d332b3, /// 0xbc4
			 0x9d941345, /// 0xbc8
			 0x87f873c7, /// 0xbcc
			 0x6fc2ac68, /// 0xbd0
			 0x99c5f43d, /// 0xbd4
			 0xe8a09f8a, /// 0xbd8
			 0x21b219cf, /// 0xbdc
			 0x992bcb46, /// 0xbe0
			 0x5a64e0af, /// 0xbe4
			 0xa30b12f9, /// 0xbe8
			 0x1e2f9297, /// 0xbec
			 0xf5e7fe76, /// 0xbf0
			 0x0ef5a0e0, /// 0xbf4
			 0xcce2ecd6, /// 0xbf8
			 0xc007460a, /// 0xbfc
			 0x8edf1605, /// 0xc00
			 0x95bcb5dd, /// 0xc04
			 0x05b6695a, /// 0xc08
			 0xb63739ad, /// 0xc0c
			 0xa066d7c8, /// 0xc10
			 0xc9cc9ca2, /// 0xc14
			 0x5e0e1224, /// 0xc18
			 0x7fae6a1a, /// 0xc1c
			 0xe23953ee, /// 0xc20
			 0x586cc1ca, /// 0xc24
			 0xd297721d, /// 0xc28
			 0xb27bd2a4, /// 0xc2c
			 0xfc1d1898, /// 0xc30
			 0xafb272b7, /// 0xc34
			 0x6ff7116d, /// 0xc38
			 0x75f82c06, /// 0xc3c
			 0xb4b178d8, /// 0xc40
			 0x9ca1499f, /// 0xc44
			 0x13c3268a, /// 0xc48
			 0x4b2c4424, /// 0xc4c
			 0x22937ac1, /// 0xc50
			 0x684afa76, /// 0xc54
			 0xc47f15bc, /// 0xc58
			 0x640eb90f, /// 0xc5c
			 0x5ab4dd4a, /// 0xc60
			 0x6bec8d79, /// 0xc64
			 0x2f2512d5, /// 0xc68
			 0xbf99a755, /// 0xc6c
			 0xe95b984c, /// 0xc70
			 0x93c957cc, /// 0xc74
			 0xd8844962, /// 0xc78
			 0x9fa709de, /// 0xc7c
			 0xc074289c, /// 0xc80
			 0x92a70e9f, /// 0xc84
			 0x1b1f39ac, /// 0xc88
			 0x6e94d191, /// 0xc8c
			 0xd851cbe7, /// 0xc90
			 0x7cc38367, /// 0xc94
			 0x0cf62f93, /// 0xc98
			 0x4e362bcd, /// 0xc9c
			 0x13f1b097, /// 0xca0
			 0xc3b37556, /// 0xca4
			 0xd045174b, /// 0xca8
			 0x036cbbb3, /// 0xcac
			 0xc4c532db, /// 0xcb0
			 0xb8efed58, /// 0xcb4
			 0xdb84986a, /// 0xcb8
			 0xb377f5ff, /// 0xcbc
			 0x9f962896, /// 0xcc0
			 0xdc7301f4, /// 0xcc4
			 0xa219bba3, /// 0xcc8
			 0x24cad472, /// 0xccc
			 0x95a40c51, /// 0xcd0
			 0x69900bcb, /// 0xcd4
			 0xd12dee62, /// 0xcd8
			 0xe76a83c0, /// 0xcdc
			 0xf6163bfc, /// 0xce0
			 0xc52bad2d, /// 0xce4
			 0xd3a18f21, /// 0xce8
			 0x52d4071c, /// 0xcec
			 0x3053007a, /// 0xcf0
			 0x53700fb6, /// 0xcf4
			 0xfa7b212e, /// 0xcf8
			 0x0e857640, /// 0xcfc
			 0x2a70f166, /// 0xd00
			 0x801be724, /// 0xd04
			 0xa47296d1, /// 0xd08
			 0x172d4b87, /// 0xd0c
			 0x46273bc9, /// 0xd10
			 0x2b54b56e, /// 0xd14
			 0xc643ddac, /// 0xd18
			 0xc28edbbc, /// 0xd1c
			 0x16afe123, /// 0xd20
			 0x803271d9, /// 0xd24
			 0xe3543373, /// 0xd28
			 0x6c079f44, /// 0xd2c
			 0x11d94b99, /// 0xd30
			 0x9907915b, /// 0xd34
			 0x8e47daff, /// 0xd38
			 0xd48bee6f, /// 0xd3c
			 0xdc49635f, /// 0xd40
			 0x2a140311, /// 0xd44
			 0x2c5b228b, /// 0xd48
			 0x70dd99b0, /// 0xd4c
			 0x04a103b7, /// 0xd50
			 0x16747a42, /// 0xd54
			 0x451edb19, /// 0xd58
			 0x250fc7da, /// 0xd5c
			 0x31f1c135, /// 0xd60
			 0x17c05679, /// 0xd64
			 0xc985a9b7, /// 0xd68
			 0x927c2293, /// 0xd6c
			 0xc60ea8df, /// 0xd70
			 0xa47b6bd2, /// 0xd74
			 0x4c6a6da0, /// 0xd78
			 0x668e79e5, /// 0xd7c
			 0x379644f6, /// 0xd80
			 0x16a080bc, /// 0xd84
			 0xac8db309, /// 0xd88
			 0x79520102, /// 0xd8c
			 0x80bef1b9, /// 0xd90
			 0xd895d0b9, /// 0xd94
			 0xf6e97886, /// 0xd98
			 0x4733b5e4, /// 0xd9c
			 0x119ede88, /// 0xda0
			 0x2a9cf19f, /// 0xda4
			 0x15463260, /// 0xda8
			 0x94d8e99b, /// 0xdac
			 0x67c74fac, /// 0xdb0
			 0x51d7f7a0, /// 0xdb4
			 0x374cf526, /// 0xdb8
			 0xfcf76bd4, /// 0xdbc
			 0x2a92276e, /// 0xdc0
			 0x3cfb926d, /// 0xdc4
			 0x53924176, /// 0xdc8
			 0xb94b370a, /// 0xdcc
			 0x48feff50, /// 0xdd0
			 0xf8c53a20, /// 0xdd4
			 0x93b06d6f, /// 0xdd8
			 0x75bed3be, /// 0xddc
			 0x0b5aadfa, /// 0xde0
			 0x9ae8efb4, /// 0xde4
			 0x9afcb343, /// 0xde8
			 0xd2e40771, /// 0xdec
			 0xc295b58c, /// 0xdf0
			 0x8ca3aa89, /// 0xdf4
			 0xd22b54c5, /// 0xdf8
			 0x1292f372, /// 0xdfc
			 0x8ea11bb9, /// 0xe00
			 0xd7699b24, /// 0xe04
			 0x981b6332, /// 0xe08
			 0x61ed6d29, /// 0xe0c
			 0x80486361, /// 0xe10
			 0x557745ab, /// 0xe14
			 0x7a204337, /// 0xe18
			 0x52e02f9b, /// 0xe1c
			 0x91cce6c0, /// 0xe20
			 0xe4bed254, /// 0xe24
			 0xa3791699, /// 0xe28
			 0xf3135ee3, /// 0xe2c
			 0xf1ac447a, /// 0xe30
			 0xf34fbe09, /// 0xe34
			 0x0fd19a28, /// 0xe38
			 0x32a73d50, /// 0xe3c
			 0x8ad993af, /// 0xe40
			 0x99add9ee, /// 0xe44
			 0x116b335e, /// 0xe48
			 0x33bd8b65, /// 0xe4c
			 0x0b4d394c, /// 0xe50
			 0x5fecdc1a, /// 0xe54
			 0x8c0e48b0, /// 0xe58
			 0xc40bbacf, /// 0xe5c
			 0x745ce5b8, /// 0xe60
			 0xc647fa1c, /// 0xe64
			 0x49257b9b, /// 0xe68
			 0x45ea4b93, /// 0xe6c
			 0x0173ee2f, /// 0xe70
			 0xfaa426ed, /// 0xe74
			 0xe5ca5f6c, /// 0xe78
			 0x35e2852e, /// 0xe7c
			 0xa3d69987, /// 0xe80
			 0x50ce72d6, /// 0xe84
			 0x0f7c60a8, /// 0xe88
			 0xef500248, /// 0xe8c
			 0xdd9de393, /// 0xe90
			 0x411b9f7c, /// 0xe94
			 0x402bf6d6, /// 0xe98
			 0xe52783ee, /// 0xe9c
			 0x4fa90400, /// 0xea0
			 0x74a85346, /// 0xea4
			 0xb2ef2782, /// 0xea8
			 0x3bac1a7a, /// 0xeac
			 0x3da6ae7e, /// 0xeb0
			 0xe67fa8e9, /// 0xeb4
			 0x3f0523ac, /// 0xeb8
			 0x5667c53f, /// 0xebc
			 0xd911483f, /// 0xec0
			 0x068353a5, /// 0xec4
			 0xb79dae78, /// 0xec8
			 0x4c787e3f, /// 0xecc
			 0x5b110767, /// 0xed0
			 0x7747a2e8, /// 0xed4
			 0xe89fe872, /// 0xed8
			 0x8255d297, /// 0xedc
			 0x54c585c8, /// 0xee0
			 0x0fc0adf7, /// 0xee4
			 0x0b8fa823, /// 0xee8
			 0x6ab4c51f, /// 0xeec
			 0xc34229eb, /// 0xef0
			 0x37a013f2, /// 0xef4
			 0x5c9751e0, /// 0xef8
			 0x1a75f1bc, /// 0xefc
			 0x0f282fe5, /// 0xf00
			 0xc0c3637b, /// 0xf04
			 0x452f318b, /// 0xf08
			 0x780dc8d4, /// 0xf0c
			 0x45c4e956, /// 0xf10
			 0x2efad7cf, /// 0xf14
			 0xd7290f0d, /// 0xf18
			 0xd7660528, /// 0xf1c
			 0x6933c38f, /// 0xf20
			 0xd78c34bd, /// 0xf24
			 0xdc485a7e, /// 0xf28
			 0xde2915e1, /// 0xf2c
			 0x1c3d55b5, /// 0xf30
			 0x50c5ca6d, /// 0xf34
			 0xd50b83e2, /// 0xf38
			 0xaaecd24e, /// 0xf3c
			 0xea9e2b30, /// 0xf40
			 0x22d93dde, /// 0xf44
			 0xfae1b269, /// 0xf48
			 0x4cbafb36, /// 0xf4c
			 0x257587b8, /// 0xf50
			 0x36928e15, /// 0xf54
			 0x2e175781, /// 0xf58
			 0x6e6fbb57, /// 0xf5c
			 0xc07c6b8f, /// 0xf60
			 0xa6d7a870, /// 0xf64
			 0xa251e64e, /// 0xf68
			 0x98a41b18, /// 0xf6c
			 0x3daafc76, /// 0xf70
			 0x74dff6c8, /// 0xf74
			 0x49d4eaf8, /// 0xf78
			 0xac0d9fd9, /// 0xf7c
			 0x9cb7b070, /// 0xf80
			 0xdf55c872, /// 0xf84
			 0x2777470d, /// 0xf88
			 0xc19484a0, /// 0xf8c
			 0xfe2b015a, /// 0xf90
			 0xb10411aa, /// 0xf94
			 0xd7363102, /// 0xf98
			 0x17fe943c, /// 0xf9c
			 0xd9e6e5ac, /// 0xfa0
			 0xf0a177c8, /// 0xfa4
			 0x123dd424, /// 0xfa8
			 0xd8f96ef2, /// 0xfac
			 0x5fc5c6f1, /// 0xfb0
			 0xbc154960, /// 0xfb4
			 0xdec2b651, /// 0xfb8
			 0xc7752e3c, /// 0xfbc
			 0xcfd77acc, /// 0xfc0
			 0x991e08b5, /// 0xfc4
			 0x9f5d390c, /// 0xfc8
			 0x6eac9c0a, /// 0xfcc
			 0x93e9b144, /// 0xfd0
			 0x18cdd4a2, /// 0xfd4
			 0x332f3aa7, /// 0xfd8
			 0xfdb00d84, /// 0xfdc
			 0x1f0a4aa8, /// 0xfe0
			 0x0af13f99, /// 0xfe4
			 0x4761b189, /// 0xfe8
			 0xa7369672, /// 0xfec
			 0x34e9082b, /// 0xff0
			 0x9148948f, /// 0xff4
			 0x3adbe9ac, /// 0xff8
			 0x9e1e5a32 /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00000
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0000c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00014
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0001c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00020
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00024
			 0xbf028f5c,                                                  // -0.51                                       /// 00028
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0002c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00034
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00038
			 0x00000000,                                                  // zero                                        /// 0003c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00040
			 0x0c780000,                                                  // Leading 1s:                                 /// 00044
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00048
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0004c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00060
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00064
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0006c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00070
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00074
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00078
			 0x0e000007,                                                  // Trailing 1s:                                /// 0007c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00080
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00088
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0008c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00090
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00094
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000a0
			 0x33333333,                                                  // 4 random values                             /// 000a4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000a8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000ac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000b0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000b4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000b8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000bc
			 0xbf028f5c,                                                  // -0.51                                       /// 000c0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0xff800000,                                                  // -inf                                        /// 000d4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000d8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000dc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000e0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000ec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000f0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000f8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00100
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00104
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00108
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0010c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00110
			 0x0c600000,                                                  // Leading 1s:                                 /// 00114
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0011c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00120
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00124
			 0xff800000,                                                  // -inf                                        /// 00128
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0012c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00130
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00140
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00148
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0014c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00154
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00158
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0015c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00160
			 0xcb000000,                                                  // -8388608.0                                  /// 00164
			 0x33333333,                                                  // 4 random values                             /// 00168
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0016c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00170
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00174
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00178
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00180
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00184
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00188
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0018c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00190
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00194
			 0x7fc00001,                                                  // signaling NaN                               /// 00198
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0019c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001a0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001ac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001b0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001b8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001bc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001cc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001d0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001d4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001e0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 001e4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001ec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x80000000,                                                  // -zero                                       /// 001f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001f8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00208
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00210
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00214
			 0x0e000001,                                                  // Trailing 1s:                                /// 00218
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0021c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00220
			 0x3f028f5c,                                                  // 0.51                                        /// 00224
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00228
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00230
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00238
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0023c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00240
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00248
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0024c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00250
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00258
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0025c
			 0xbf028f5c,                                                  // -0.51                                       /// 00260
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00264
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00270
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00278
			 0x55555555,                                                  // 4 random values                             /// 0027c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00280
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00284
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00288
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0028c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00290
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00294
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00298
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0029c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002a0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002a8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002ac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002b0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002b4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002b8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002bc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002c0
			 0x4b000000,                                                  // 8388608.0                                   /// 002c4
			 0xcb000000,                                                  // -8388608.0                                  /// 002c8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002d0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002d4
			 0x7fc00001,                                                  // signaling NaN                               /// 002d8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002dc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002e0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002e4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002f0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002f4
			 0x0c400000,                                                  // Leading 1s:                                 /// 002f8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00300
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00304
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00308
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0030c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00314
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00318
			 0xbf028f5c,                                                  // -0.51                                       /// 0031c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00320
			 0x7fc00001,                                                  // signaling NaN                               /// 00324
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00328
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0032c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00330
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00334
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00338
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0033c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00340
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00344
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00354
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00360
			 0x4b000000,                                                  // 8388608.0                                   /// 00364
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0036c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00370
			 0x0e000003,                                                  // Trailing 1s:                                /// 00374
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00378
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x00000000,                                                  // zero                                        /// 00380
			 0x4b000000,                                                  // 8388608.0                                   /// 00384
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0038c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00390
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00394
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00398
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0039c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003a8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003b0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003b8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003c0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 003d4
			 0x00000000,                                                  // zero                                        /// 003d8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003e4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003e8
			 0xcb000000,                                                  // -8388608.0                                  /// 003ec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003f0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003f8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003fc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00408
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0040c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00410
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00414
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00418
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0041c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00420
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00424
			 0x0c700000,                                                  // Leading 1s:                                 /// 00428
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0042c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00434
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00438
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0xffc00001,                                                  // -signaling NaN                              /// 00440
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00444
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00448
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0044c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00450
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00458
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00460
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00464
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00468
			 0x0e000003,                                                  // Trailing 1s:                                /// 0046c
			 0x7fc00001,                                                  // signaling NaN                               /// 00470
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00474
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00478
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00480
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00484
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00488
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0048c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00490
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00494
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00498
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0049c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x0e000003,                                                  // Trailing 1s:                                /// 004a4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004ac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004b0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004b4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x55555555,                                                  // 4 random values                             /// 004bc
			 0xff800000,                                                  // -inf                                        /// 004c0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004c4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004c8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004cc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004d0
			 0xff800000,                                                  // -inf                                        /// 004d4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004d8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004dc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004e4
			 0x80000000,                                                  // -zero                                       /// 004e8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004ec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004f0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004f8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00504
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00508
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0050c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0051c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00520
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00524
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00528
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0052c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x0c700000,                                                  // Leading 1s:                                 /// 0053c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00540
			 0x0c700000,                                                  // Leading 1s:                                 /// 00544
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00548
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0054c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00550
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00554
			 0xffc00001,                                                  // -signaling NaN                              /// 00558
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0055c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00564
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00568
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0056c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00570
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00574
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00578
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0057c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00580
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00588
			 0x0e000003,                                                  // Trailing 1s:                                /// 0058c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00590
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00594
			 0x0c780000,                                                  // Leading 1s:                                 /// 00598
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0059c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005a4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005a8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x0c700000,                                                  // Leading 1s:                                 /// 005b0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005b4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005b8
			 0x55555555,                                                  // 4 random values                             /// 005bc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x00011111,                                                  // 9.7958E-41                                  /// 005c4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005c8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005cc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005d0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x7fc00001,                                                  // signaling NaN                               /// 005d8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005e4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005e8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005ec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005f0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005f4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005f8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005fc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00600
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00608
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0060c
			 0xbf028f5c,                                                  // -0.51                                       /// 00610
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00614
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00618
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0061c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00620
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00628
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0062c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00630
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00634
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00638
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0063c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00640
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00648
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0064c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00650
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00654
			 0x3f028f5c,                                                  // 0.51                                        /// 00658
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0065c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00660
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00664
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00668
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00670
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00674
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00678
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0067c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00680
			 0xffc00001,                                                  // -signaling NaN                              /// 00684
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00688
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0068c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00690
			 0xff800000,                                                  // -inf                                        /// 00694
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00698
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0069c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006a0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006b0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006b4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006b8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006bc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006c0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006c4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006c8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006cc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006d0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006d4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006d8
			 0x4b000000,                                                  // 8388608.0                                   /// 006dc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006e0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006e4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 006e8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006f0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006f8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006fc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00700
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0070c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00710
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00714
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00718
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0071c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00728
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00738
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0073c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00740
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00744
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00748
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0074c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x0c780000,                                                  // Leading 1s:                                 /// 00754
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00758
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00760
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00764
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x80011111,                                                  // -9.7958E-41                                 /// 0076c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00774
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00778
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0077c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00780
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00784
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00788
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0078c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00790
			 0x0c780000,                                                  // Leading 1s:                                 /// 00794
			 0x0c600000,                                                  // Leading 1s:                                 /// 00798
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0079c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007a0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007a4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007a8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007ac
			 0xffc00001,                                                  // -signaling NaN                              /// 007b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007b4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007b8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007bc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x00011111,                                                  // 9.7958E-41                                  /// 007c8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007d0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007d4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007d8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007dc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007e0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007e4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007e8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007f8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007fc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00804
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0080c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00810
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00814
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00818
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0081c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00820
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00828
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00834
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00838
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0083c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00840
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00850
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00854
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x55555555,                                                  // 4 random values                             /// 0085c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00860
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00864
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0086c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00870
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00874
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x0e000003,                                                  // Trailing 1s:                                /// 0087c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00880
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00884
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0xffc00001,                                                  // -signaling NaN                              /// 0088c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00890
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00894
			 0x0c780000,                                                  // Leading 1s:                                 /// 00898
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008a8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008ac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008b0
			 0x7f800000,                                                  // inf                                         /// 008b4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008b8
			 0x00000000,                                                  // zero                                        /// 008bc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008c0
			 0x0e000001,                                                  // Trailing 1s:                                /// 008c4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008c8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008cc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008d0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008d4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008d8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008dc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008e0
			 0x3f028f5c,                                                  // 0.51                                        /// 008e4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008f0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008f4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008f8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008fc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00900
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00904
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0xff800000,                                                  // -inf                                        /// 00910
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00914
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0091c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00920
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00928
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0092c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00930
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00934
			 0x0c400000,                                                  // Leading 1s:                                 /// 00938
			 0x00000000,                                                  // zero                                        /// 0093c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00940
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00944
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00948
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0094c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0095c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00960
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00968
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00970
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00974
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00978
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0097c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00980
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00988
			 0x7f800000,                                                  // inf                                         /// 0098c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00994
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00998
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009a0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009a8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009b0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009b4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009b8
			 0x80011111,                                                  // -9.7958E-41                                 /// 009bc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009c0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009c4
			 0x3f028f5c,                                                  // 0.51                                        /// 009c8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009cc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009d4
			 0x33333333,                                                  // 4 random values                             /// 009d8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e0
			 0x80000000,                                                  // -zero                                       /// 009e4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x00011111,                                                  // 9.7958E-41                                  /// 009ec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009f0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009f4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009f8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009fc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a00
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a04
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a0c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a10
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a14
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a18
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a20
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a24
			 0x4b000000,                                                  // 8388608.0                                   /// 00a28
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a2c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a40
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a48
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a4c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a54
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a60
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a6c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a70
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a74
			 0x80000000,                                                  // -zero                                       /// 00a78
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a7c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a80
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a8c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a90
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a98
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a9c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00aa0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00aa4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00aa8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00aac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ab8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00abc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ac0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ac4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ac8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00acc
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ad0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ad4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ad8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ae0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ae4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ae8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00af8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b08
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b0c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b14
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b18
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b1c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b20
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b24
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b28
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b2c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b30
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b34
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b38
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b48
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b4c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b54
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b58
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b60
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b68
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b6c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b70
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b74
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b78
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b7c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b80
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b84
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b94
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b98
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b9c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ba0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ba8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bb0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bb4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bb8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bbc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bc4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bc8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bd0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bd8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bdc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00be4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00be8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bf0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bfc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c04
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c08
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c10
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c14
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c18
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c1c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c20
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c24
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c2c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c30
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c34
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c38
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c40
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c44
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c48
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c4c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c50
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c54
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c58
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c5c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c60
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c64
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c68
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c6c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c70
			 0xff800000,                                                  // -inf                                        /// 00c74
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c78
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c7c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c84
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c8c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c90
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c94
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c98
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ca0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ca4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ca8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cac
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cb0
			 0x55555555,                                                  // 4 random values                             /// 00cb4
			 0xff800000,                                                  // -inf                                        /// 00cb8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00cc0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cc4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00cc8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ccc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00cd0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cd8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ce8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cf4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cf8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cfc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d00
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d04
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d08
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d0c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d14
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d18
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d28
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d2c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d30
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d38
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d40
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d44
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d48
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d4c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d50
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d54
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d58
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d5c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d64
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d6c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d70
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d7c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d80
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d88
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d90
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d94
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d98
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d9c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00da0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00da4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00da8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00dac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00db4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00dbc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00dc0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dc8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00dd0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00dd4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ddc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00de0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00de4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00de8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00dec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00df0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00df4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00df8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00dfc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e00
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e04
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e08
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e0c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e18
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e20
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x4b000000,                                                  // 8388608.0                                   /// 00e2c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e34
			 0xff800000,                                                  // -inf                                        /// 00e38
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e3c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e40
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e48
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e4c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00e50
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e58
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e5c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e60
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e64
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e68
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e6c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e70
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e78
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e7c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e80
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e84
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e88
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e8c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e90
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e94
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e98
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e9c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ea0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ea4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ea8
			 0x55555555,                                                  // 4 random values                             /// 00eac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00eb8
			 0x4b000000,                                                  // 8388608.0                                   /// 00ebc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ec0
			 0xbf028f5c,                                                  // -0.51                                       /// 00ec4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ec8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ecc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ed4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ed8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00edc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ee4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00eec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ef0
			 0x7f800000,                                                  // inf                                         /// 00ef4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00efc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f00
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f14
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f18
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f1c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f20
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f24
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f28
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f2c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f30
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f34
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f38
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f3c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f40
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f48
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f54
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f58
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f5c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f60
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f64
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f68
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f70
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f74
			 0x3f028f5c,                                                  // 0.51                                        /// 00f78
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f80
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f84
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f8c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f90
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f94
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f98
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f9c
			 0x7f800000,                                                  // inf                                         /// 00fa0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00fa4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fb0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fb4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fb8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fbc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00fc0
			 0x00000000,                                                  // zero                                        /// 00fc4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fcc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fd0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fd4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fd8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fe0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fe8
			 0x00000000,                                                  // zero                                        /// 00fec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ff0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ff4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ff8
			 0x0f7ffffc                                                  // all bit of mantissa set upto -3ulp          /// last
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
			 0x00000544,
			 0x0000075c,
			 0x0000019c,
			 0x00000150,
			 0x00000798,
			 0x000005e8,
			 0x000006f8,
			 0x000001a8,

			 /// vpu register f2
			 0x41100000,
			 0x41b00000,
			 0x41500000,
			 0x41f00000,
			 0x41d80000,
			 0x40c00000,
			 0x41e80000,
			 0x41b00000,

			 /// vpu register f3
			 0x41980000,
			 0x42000000,
			 0x41600000,
			 0x41800000,
			 0x41d00000,
			 0x41880000,
			 0x41100000,
			 0x41b80000,

			 /// vpu register f4
			 0x41300000,
			 0x41980000,
			 0x41700000,
			 0x41e00000,
			 0x41980000,
			 0x41900000,
			 0x41c00000,
			 0x40400000,

			 /// vpu register f5
			 0x40a00000,
			 0x41600000,
			 0x41100000,
			 0x41600000,
			 0x41600000,
			 0x40c00000,
			 0x41a00000,
			 0x41880000,

			 /// vpu register f6
			 0x41400000,
			 0x41200000,
			 0x40800000,
			 0x41e80000,
			 0x41800000,
			 0x40e00000,
			 0x41f00000,
			 0x41000000,

			 /// vpu register f7
			 0x41a80000,
			 0x40400000,
			 0x42000000,
			 0x41100000,
			 0x41880000,
			 0x41300000,
			 0x41b80000,
			 0x41500000,

			 /// vpu register f8
			 0x41f80000,
			 0x40e00000,
			 0x41500000,
			 0x40a00000,
			 0x41e00000,
			 0x40000000,
			 0x41400000,
			 0x41d00000,

			 /// vpu register f9
			 0x3f800000,
			 0x41f00000,
			 0x41100000,
			 0x41500000,
			 0x41000000,
			 0x41980000,
			 0x41500000,
			 0x41c80000,

			 /// vpu register f10
			 0x41b80000,
			 0x41880000,
			 0x41900000,
			 0x41980000,
			 0x41600000,
			 0x40e00000,
			 0x41f80000,
			 0x41b80000,

			 /// vpu register f11
			 0x41980000,
			 0x41400000,
			 0x41880000,
			 0x41b80000,
			 0x41200000,
			 0x40a00000,
			 0x41880000,
			 0x41e00000,

			 /// vpu register f12
			 0x40400000,
			 0x41900000,
			 0x41800000,
			 0x41c80000,
			 0x41a80000,
			 0x40e00000,
			 0x41a80000,
			 0x41d00000,

			 /// vpu register f13
			 0x41a80000,
			 0x41900000,
			 0x41980000,
			 0x40000000,
			 0x41a00000,
			 0x41400000,
			 0x41300000,
			 0x41b00000,

			 /// vpu register f14
			 0x41d80000,
			 0x41c80000,
			 0x41e00000,
			 0x42000000,
			 0x42000000,
			 0x41600000,
			 0x42000000,
			 0x41900000,

			 /// vpu register f15
			 0x41300000,
			 0x41f80000,
			 0x41000000,
			 0x41200000,
			 0x41a80000,
			 0x41a00000,
			 0x40a00000,
			 0x41d00000,

			 /// vpu register f16
			 0x41d00000,
			 0x41980000,
			 0x41d00000,
			 0x40e00000,
			 0x40400000,
			 0x41f00000,
			 0x41800000,
			 0x41500000,

			 /// vpu register f17
			 0x40800000,
			 0x3f800000,
			 0x41900000,
			 0x41000000,
			 0x41c00000,
			 0x41b00000,
			 0x41700000,
			 0x41300000,

			 /// vpu register f18
			 0x41b00000,
			 0x41500000,
			 0x41900000,
			 0x41d80000,
			 0x41a00000,
			 0x41200000,
			 0x41100000,
			 0x41900000,

			 /// vpu register f19
			 0x41b80000,
			 0x40a00000,
			 0x41100000,
			 0x41d80000,
			 0x40a00000,
			 0x41900000,
			 0x41980000,
			 0x41900000,

			 /// vpu register f20
			 0x41800000,
			 0x40000000,
			 0x40a00000,
			 0x41880000,
			 0x41e80000,
			 0x41880000,
			 0x41e80000,
			 0x41f80000,

			 /// vpu register f21
			 0x40e00000,
			 0x41d80000,
			 0x41500000,
			 0x41700000,
			 0x41400000,
			 0x3f800000,
			 0x41e80000,
			 0x41500000,

			 /// vpu register f22
			 0x40e00000,
			 0x40400000,
			 0x40e00000,
			 0x41700000,
			 0x41000000,
			 0x41b80000,
			 0x41300000,
			 0x41800000,

			 /// vpu register f23
			 0x40400000,
			 0x41d00000,
			 0x41980000,
			 0x41500000,
			 0x41400000,
			 0x41c00000,
			 0x41e80000,
			 0x41e00000,

			 /// vpu register f24
			 0x41e00000,
			 0x41800000,
			 0x41600000,
			 0x40800000,
			 0x40e00000,
			 0x41d80000,
			 0x41e80000,
			 0x41000000,

			 /// vpu register f25
			 0x41800000,
			 0x41f00000,
			 0x40000000,
			 0x40c00000,
			 0x41d00000,
			 0x41d00000,
			 0x41a00000,
			 0x40a00000,

			 /// vpu register f26
			 0x42000000,
			 0x41c80000,
			 0x40c00000,
			 0x41a00000,
			 0x41400000,
			 0x41900000,
			 0x41980000,
			 0x41f00000,

			 /// vpu register f27
			 0x41800000,
			 0x40400000,
			 0x40400000,
			 0x41000000,
			 0x41c00000,
			 0x41a00000,
			 0x41d00000,
			 0x41e00000,

			 /// vpu register f28
			 0x41900000,
			 0x40800000,
			 0x41e00000,
			 0x40c00000,
			 0x41a00000,
			 0x41e00000,
			 0x41f80000,
			 0x41c00000,

			 /// vpu register f29
			 0x41980000,
			 0x41800000,
			 0x40400000,
			 0x41300000,
			 0x40a00000,
			 0x41100000,
			 0x41100000,
			 0x42000000,

			 /// vpu register f30
			 0x41d00000,
			 0x41980000,
			 0x41e00000,
			 0x41b00000,
			 0x41500000,
			 0x41a80000,
			 0x40000000,
			 0x40400000,

			 /// vpu register f31
			 0x41500000,
			 0x41f80000,
			 0x41d80000,
			 0x40000000,
			 0x40a00000,
			 0x41900000,
			 0x41300000,
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
		"fghl.ps f11, f1 (x13)\n"                             ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f25, f1 (x13)\n"                             ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f24, f1 (x11)\n"                             ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f13, f1 (x11)\n"                             ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f10, f1 (x11)\n"                             ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f20, f1 (x12)\n"                             ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f13, f1 (x12)\n"                             ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f29, f1 (x15)\n"                             ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f20, f1 (x13)\n"                             ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f12, f1 (x12)\n"                             ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f24, f1 (x14)\n"                             ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f9, f1 (x15)\n"                              ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f14, f1 (x15)\n"                             ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f20, f1 (x13)\n"                             ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f17, f1 (x14)\n"                             ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f17, f1 (x11)\n"                             ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f19, f1 (x15)\n"                             ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f21, f1 (x13)\n"                             ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f10, f1 (x11)\n"                             ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f16, f1 (x12)\n"                             ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f2, f1 (x11)\n"                              ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f10, f1 (x13)\n"                             ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f3, f1 (x15)\n"                              ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f4, f1 (x14)\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f9, f1 (x13)\n"                              ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f19, f1 (x13)\n"                             ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f29, f1 (x13)\n"                             ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f19, f1 (x11)\n"                             ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f12, f1 (x13)\n"                             ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f26, f1 (x13)\n"                             ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f19, f1 (x12)\n"                             ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f24, f1 (x12)\n"                             ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f11, f1 (x11)\n"                             ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f29, f1 (x13)\n"                             ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f24, f1 (x15)\n"                             ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f27, f1 (x13)\n"                             ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f7, f1 (x11)\n"                              ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f13, f1 (x15)\n"                             ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f28, f1 (x12)\n"                             ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f20, f1 (x11)\n"                             ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f7, f1 (x11)\n"                              ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f14, f1 (x11)\n"                             ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f5, f1 (x11)\n"                              ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f3, f1 (x11)\n"                              ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f28, f1 (x12)\n"                             ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f21, f1 (x15)\n"                             ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f25, f1 (x11)\n"                             ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f28, f1 (x14)\n"                             ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f16, f1 (x14)\n"                             ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f12, f1 (x12)\n"                             ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f15, f1 (x14)\n"                             ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f26, f1 (x15)\n"                             ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f16, f1 (x11)\n"                             ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f28, f1 (x12)\n"                             ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f10, f1 (x11)\n"                             ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f16, f1 (x13)\n"                             ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f19, f1 (x11)\n"                             ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f27, f1 (x14)\n"                             ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f15, f1 (x13)\n"                             ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f2, f1 (x13)\n"                              ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f6, f1 (x15)\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f26, f1 (x13)\n"                             ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f17, f1 (x15)\n"                             ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f8, f1 (x14)\n"                              ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f24, f1 (x14)\n"                             ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f27, f1 (x11)\n"                             ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f11, f1 (x13)\n"                             ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f22, f1 (x11)\n"                             ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f26, f1 (x11)\n"                             ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f6, f1 (x11)\n"                              ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f2, f1 (x13)\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f17, f1 (x13)\n"                             ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f5, f1 (x14)\n"                              ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f10, f1 (x13)\n"                             ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f12, f1 (x12)\n"                             ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f3, f1 (x14)\n"                              ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f22, f1 (x14)\n"                             ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f5, f1 (x13)\n"                              ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f27, f1 (x12)\n"                             ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f30, f1 (x13)\n"                             ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f19, f1 (x15)\n"                             ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f21, f1 (x13)\n"                             ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f29, f1 (x13)\n"                             ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f14, f1 (x14)\n"                             ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f3, f1 (x13)\n"                              ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f8, f1 (x15)\n"                              ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f13, f1 (x14)\n"                             ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f21, f1 (x15)\n"                             ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f29, f1 (x12)\n"                             ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f30, f1 (x13)\n"                             ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f30, f1 (x14)\n"                             ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f3, f1 (x15)\n"                              ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f14, f1 (x15)\n"                             ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f20, f1 (x12)\n"                             ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f4, f1 (x12)\n"                              ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f28, f1 (x13)\n"                             ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f8, f1 (x14)\n"                              ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f4, f1 (x12)\n"                              ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fghl.ps f23, f1 (x15)\n"                             ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
