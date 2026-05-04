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
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00004
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x00011111,                                                  // 9.7958E-41                                  /// 0000c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00010
			 0x0c600000,                                                  // Leading 1s:                                 /// 00014
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0001c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00020
			 0xbf028f5c,                                                  // -0.51                                       /// 00024
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00028
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0002c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00030
			 0x80000000,                                                  // -zero                                       /// 00034
			 0xffc00001,                                                  // -signaling NaN                              /// 00038
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0003c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00040
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00044
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00050
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00054
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00058
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0005c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00064
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0006c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00070
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00074
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00078
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0007c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00080
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00088
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0008c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00090
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00094
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0009c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000a8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000ac
			 0x0c600000,                                                  // Leading 1s:                                 /// 000b0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000b8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000bc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000c4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000cc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000d0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000d8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000dc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000e4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000e8
			 0xff800000,                                                  // -inf                                        /// 000ec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000f0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 000f4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000fc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00100
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00104
			 0x0c600000,                                                  // Leading 1s:                                 /// 00108
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0010c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00110
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00114
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00118
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0011c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00120
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00124
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00128
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0012c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00130
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00134
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x0c780000,                                                  // Leading 1s:                                 /// 0013c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00144
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00150
			 0x0c700000,                                                  // Leading 1s:                                 /// 00154
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00158
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00160
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00168
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00174
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x00011111,                                                  // 9.7958E-41                                  /// 0017c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00180
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00184
			 0x0c780000,                                                  // Leading 1s:                                 /// 00188
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0018c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x55555555,                                                  // 4 random values                             /// 00194
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00198
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0019c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001a4
			 0x33333333,                                                  // 4 random values                             /// 001a8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001ac
			 0xbf028f5c,                                                  // -0.51                                       /// 001b0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001b4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001b8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001bc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001c0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001c8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001d0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001dc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001e0
			 0xff800000,                                                  // -inf                                        /// 001e4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001e8
			 0x33333333,                                                  // 4 random values                             /// 001ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001f4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001f8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001fc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00200
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00204
			 0xff800000,                                                  // -inf                                        /// 00208
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00210
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00214
			 0x33333333,                                                  // 4 random values                             /// 00218
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0021c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00220
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00224
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00228
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0022c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00230
			 0x0e000003,                                                  // Trailing 1s:                                /// 00234
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00238
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00244
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00248
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00250
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00254
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00258
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0025c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00264
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00268
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00274
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0027c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00280
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00288
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0028c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00294
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0029c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002ac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002b8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002bc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002c0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002c4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002c8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002cc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002d4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002d8
			 0x00011111,                                                  // 9.7958E-41                                  /// 002dc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002e0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002e4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002e8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002f4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002f8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002fc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00300
			 0x00011111,                                                  // 9.7958E-41                                  /// 00304
			 0x0e000003,                                                  // Trailing 1s:                                /// 00308
			 0xbf028f5c,                                                  // -0.51                                       /// 0030c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00310
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00314
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00318
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00320
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00328
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0032c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00330
			 0xbf028f5c,                                                  // -0.51                                       /// 00334
			 0x0e000003,                                                  // Trailing 1s:                                /// 00338
			 0x33333333,                                                  // 4 random values                             /// 0033c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00340
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00344
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00348
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0034c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00350
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00358
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0035c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00364
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0036c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00374
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00378
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0037c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00380
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00384
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00388
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0038c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00390
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0039c
			 0x0c600000,                                                  // Leading 1s:                                 /// 003a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 003ac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003b0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003b4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003bc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003c0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003c4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003c8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003cc
			 0x0c400000,                                                  // Leading 1s:                                 /// 003d0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003d4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003e0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 003e4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003e8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 003ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003f0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003f4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x7fc00001,                                                  // signaling NaN                               /// 00400
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00408
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0040c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0xffc00001,                                                  // -signaling NaN                              /// 00414
			 0x7fc00001,                                                  // signaling NaN                               /// 00418
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0041c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00420
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00428
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0042c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00430
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00434
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0043c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00448
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0044c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00454
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00458
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00460
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00468
			 0x7f800000,                                                  // inf                                         /// 0046c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00470
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0047c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00484
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0048c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00490
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00494
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004a4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004ac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004b0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004b4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004b8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004bc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004c0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004cc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004d4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004dc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004e8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004f0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004f8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004fc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00500
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00504
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00508
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0050c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00510
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00514
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00518
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0051c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00520
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00524
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00528
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0052c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00530
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00534
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0053c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00544
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00548
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0054c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00550
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00558
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0055c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00560
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00568
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0056c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00574
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00578
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0057c
			 0xffc00001,                                                  // -signaling NaN                              /// 00580
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00588
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0058c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00590
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00594
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00598
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005a4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005ac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005b0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005b4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005b8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005bc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005c0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005c4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005c8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005d0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005d4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005d8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005e0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005e4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005e8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005f0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005f4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005fc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00600
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00604
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00608
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00614
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00620
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00624
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00628
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0062c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x0c400000,                                                  // Leading 1s:                                 /// 00634
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00638
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0063c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00640
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00644
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00648
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0064c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00654
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0065c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00660
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00664
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00668
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0066c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00670
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00674
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00678
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0067c
			 0x00000000,                                                  // zero                                        /// 00680
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00688
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0068c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00694
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00698
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006a4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006ac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006b0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006b4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 006b8
			 0x7f800000,                                                  // inf                                         /// 006bc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006c0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006c4
			 0xffc00001,                                                  // -signaling NaN                              /// 006c8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006d0
			 0x55555555,                                                  // 4 random values                             /// 006d4
			 0x80000000,                                                  // -zero                                       /// 006d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006dc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006e0
			 0x4b000000,                                                  // 8388608.0                                   /// 006e4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006e8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006ec
			 0x0c700000,                                                  // Leading 1s:                                 /// 006f0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x0c400000,                                                  // Leading 1s:                                 /// 006f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006fc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00708
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0070c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00710
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00714
			 0x0c700000,                                                  // Leading 1s:                                 /// 00718
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0071c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00720
			 0x0c780000,                                                  // Leading 1s:                                 /// 00724
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00728
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0072c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00730
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00738
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0073c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00740
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00744
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0074c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00750
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00754
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00758
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0075c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00760
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0076c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00774
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00778
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0077c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00780
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00784
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00788
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0078c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00790
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00798
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0079c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007a0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007a8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007b4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007b8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007c0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007c4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007c8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007cc
			 0x0c780000,                                                  // Leading 1s:                                 /// 007d0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007d4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007d8
			 0x0c600000,                                                  // Leading 1s:                                 /// 007dc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0xff800000,                                                  // -inf                                        /// 007e4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007e8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007f8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007fc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00804
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00808
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0080c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00814
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00818
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00824
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00828
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00834
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00838
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0083c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00840
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00844
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0084c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00850
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00858
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0085c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00860
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00864
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0086c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00870
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00874
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0087c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00880
			 0x0c400000,                                                  // Leading 1s:                                 /// 00884
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00888
			 0x0c780000,                                                  // Leading 1s:                                 /// 0088c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00890
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00894
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0089c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008a0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008b0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008c0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008c4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 008c8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008cc
			 0x4b000000,                                                  // 8388608.0                                   /// 008d0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008e0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008e4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008ec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008f0
			 0xffc00001,                                                  // -signaling NaN                              /// 008f4
			 0x0c700000,                                                  // Leading 1s:                                 /// 008f8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008fc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00900
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00904
			 0x80000000,                                                  // -zero                                       /// 00908
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0090c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00910
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00914
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00918
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00920
			 0x80000000,                                                  // -zero                                       /// 00924
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00928
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00930
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00934
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00938
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0093c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0094c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00950
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00954
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00958
			 0x55555555,                                                  // 4 random values                             /// 0095c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00960
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00964
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0096c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00970
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00978
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0097c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00980
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00984
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00988
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0098c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00990
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00994
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00998
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x0c780000,                                                  // Leading 1s:                                 /// 009a0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009a4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009b0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009b4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 009b8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009bc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 009c0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009c4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009c8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009cc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009d0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009d4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009dc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009e0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009e4
			 0xffc00001,                                                  // -signaling NaN                              /// 009e8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009f0
			 0x7fc00001,                                                  // signaling NaN                               /// 009f4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a04
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a08
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a0c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a10
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a14
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a18
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a1c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a20
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a24
			 0xcb000000,                                                  // -8388608.0                                  /// 00a28
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a2c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a30
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a34
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a38
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a3c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a40
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a44
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a48
			 0x7fc00001,                                                  // signaling NaN                               /// 00a4c
			 0xbf028f5c,                                                  // -0.51                                       /// 00a50
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a54
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a5c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a60
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a68
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a6c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a70
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a74
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a78
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a7c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a80
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a88
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a8c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a90
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a9c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00aa0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00aa4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00aa8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00aac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ab0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ab8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00abc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ac0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ac4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ac8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00acc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ad0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ad8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ae0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ae4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ae8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00aec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00af0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00af4
			 0x7fc00001,                                                  // signaling NaN                               /// 00af8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00afc
			 0xffc00001,                                                  // -signaling NaN                              /// 00b00
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b04
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b08
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b0c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b10
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b14
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b18
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b20
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b24
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b28
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b2c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b30
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b34
			 0x55555555,                                                  // 4 random values                             /// 00b38
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b3c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b40
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b44
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b48
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b4c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b50
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b54
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b58
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b5c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b6c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b70
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b78
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b7c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b80
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b88
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b8c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b90
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0xffc00001,                                                  // -signaling NaN                              /// 00b98
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b9c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ba0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ba4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ba8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bac
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00bb0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bb4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bb8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bc0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00bcc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bd4
			 0x3f028f5c,                                                  // 0.51                                        /// 00bd8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bdc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00be0
			 0x4b000000,                                                  // 8388608.0                                   /// 00be4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00be8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bec
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00bf0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x00000000,                                                  // zero                                        /// 00bf8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c00
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c04
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c08
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c0c
			 0x7f800000,                                                  // inf                                         /// 00c10
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c14
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c18
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c1c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c20
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c24
			 0xffc00001,                                                  // -signaling NaN                              /// 00c28
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c2c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c30
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c38
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c3c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c40
			 0x33333333,                                                  // 4 random values                             /// 00c44
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c4c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c58
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c5c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c60
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c64
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c68
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c74
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c78
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c80
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c84
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c8c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c90
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c94
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c98
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c9c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ca4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00cac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cb0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cb4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cb8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cbc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cc0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cc4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cc8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cd0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cdc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ce0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ce4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ce8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cf0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00cf8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cfc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d00
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d08
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d0c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d10
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d14
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0xff800000,                                                  // -inf                                        /// 00d20
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d34
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d38
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d3c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d40
			 0xffc00001,                                                  // -signaling NaN                              /// 00d44
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d48
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d4c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d50
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d54
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d5c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d64
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d68
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0xcb000000,                                                  // -8388608.0                                  /// 00d70
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d74
			 0x3f028f5c,                                                  // 0.51                                        /// 00d78
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d7c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d80
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d84
			 0x7fc00001,                                                  // signaling NaN                               /// 00d88
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d8c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d90
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d94
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d98
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d9c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00da0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00da4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00dac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00db0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00db8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00dc0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00dc8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00dcc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00dd4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ddc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00de0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00de4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00de8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00df0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00df4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e04
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e08
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e10
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e14
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e1c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e20
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e28
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e2c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e30
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e34
			 0xcb000000,                                                  // -8388608.0                                  /// 00e38
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e40
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e44
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e48
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e4c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e54
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e58
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e5c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e64
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e68
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e6c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e70
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e74
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e78
			 0x33333333,                                                  // 4 random values                             /// 00e7c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e80
			 0x80000000,                                                  // -zero                                       /// 00e84
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e88
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e8c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e90
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e94
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e98
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e9c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ea4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00eac
			 0x80000000,                                                  // -zero                                       /// 00eb0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x7f800000,                                                  // inf                                         /// 00eb8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00ebc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ec0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ec4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ec8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ecc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ed0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ed4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ed8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ee0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ee4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ee8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00eec
			 0x33333333,                                                  // 4 random values                             /// 00ef0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ef4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ef8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00efc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f04
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f08
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f0c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f10
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f14
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f18
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f1c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f20
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f24
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f28
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f2c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f30
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f34
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f38
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f44
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f48
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f4c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f50
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f54
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f60
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f64
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f68
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f74
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f78
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f80
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f84
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f88
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f90
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f94
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f98
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fa0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fa4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fa8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fb0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fb4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fb8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fc8
			 0xbf028f5c,                                                  // -0.51                                       /// 00fcc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fd4
			 0xbf028f5c,                                                  // -0.51                                       /// 00fd8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fe4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fe8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fec
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ff0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ff4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x0c7ffff8                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xca099e90, /// 0x0
			 0x21ff11f3, /// 0x4
			 0x1d651184, /// 0x8
			 0xd1886c94, /// 0xc
			 0x1405f105, /// 0x10
			 0x274c0c13, /// 0x14
			 0x3821d84a, /// 0x18
			 0xad64ed17, /// 0x1c
			 0x1cb89dc6, /// 0x20
			 0x80dbddf4, /// 0x24
			 0x9aba0e4e, /// 0x28
			 0x0c18e512, /// 0x2c
			 0x96f54283, /// 0x30
			 0xa6322a39, /// 0x34
			 0x92892f1b, /// 0x38
			 0x7afc898a, /// 0x3c
			 0xa6157cda, /// 0x40
			 0x4f811369, /// 0x44
			 0x4a4887f5, /// 0x48
			 0xdb536783, /// 0x4c
			 0x57502239, /// 0x50
			 0x0fdea7dd, /// 0x54
			 0x830c44cd, /// 0x58
			 0x5ffbafc0, /// 0x5c
			 0x8c7fe193, /// 0x60
			 0xa817af8d, /// 0x64
			 0x419ca688, /// 0x68
			 0x0ee1cc52, /// 0x6c
			 0xc14e85d4, /// 0x70
			 0x3eab7d59, /// 0x74
			 0xa033169b, /// 0x78
			 0x55b4cbb3, /// 0x7c
			 0x7e94a382, /// 0x80
			 0x5aaca66b, /// 0x84
			 0xa034e367, /// 0x88
			 0x6618ccf9, /// 0x8c
			 0x4e54bea9, /// 0x90
			 0x357859c6, /// 0x94
			 0x0444acaa, /// 0x98
			 0x52bb59e7, /// 0x9c
			 0x2f52ee2c, /// 0xa0
			 0x998fbdc6, /// 0xa4
			 0x2d59c32a, /// 0xa8
			 0x19ecac37, /// 0xac
			 0xd462691b, /// 0xb0
			 0xa088653f, /// 0xb4
			 0x11acbb88, /// 0xb8
			 0x90ce1887, /// 0xbc
			 0x7ecb48b0, /// 0xc0
			 0x7ec65a83, /// 0xc4
			 0x238876b2, /// 0xc8
			 0xabac29ec, /// 0xcc
			 0x8a24b808, /// 0xd0
			 0xc020e876, /// 0xd4
			 0xbc9e7a74, /// 0xd8
			 0xe365c452, /// 0xdc
			 0x6aaccf35, /// 0xe0
			 0x1b650d50, /// 0xe4
			 0x9179c774, /// 0xe8
			 0x8988ab86, /// 0xec
			 0x95ef5c41, /// 0xf0
			 0x99c03cb9, /// 0xf4
			 0xbac59024, /// 0xf8
			 0xd48b5c2d, /// 0xfc
			 0xa55e6f68, /// 0x100
			 0xbf984623, /// 0x104
			 0x5a3d9c47, /// 0x108
			 0xf6122801, /// 0x10c
			 0xcc5e9ba8, /// 0x110
			 0xa2d4aed5, /// 0x114
			 0xfbbdef5d, /// 0x118
			 0xd92d788b, /// 0x11c
			 0x2019242e, /// 0x120
			 0x7799c8b7, /// 0x124
			 0x8ca4372e, /// 0x128
			 0x5660424c, /// 0x12c
			 0x3ab1c1e9, /// 0x130
			 0x8a44e0dd, /// 0x134
			 0x51f8cb31, /// 0x138
			 0x56fc46e1, /// 0x13c
			 0xb039eb74, /// 0x140
			 0x28276424, /// 0x144
			 0xedb91002, /// 0x148
			 0x466c90f3, /// 0x14c
			 0xb6af6fba, /// 0x150
			 0xe7675b09, /// 0x154
			 0xe0a8728c, /// 0x158
			 0xcdf0e556, /// 0x15c
			 0xeaab4e85, /// 0x160
			 0x9c0c6889, /// 0x164
			 0x8643c0c9, /// 0x168
			 0x643e99ef, /// 0x16c
			 0x0e455ae0, /// 0x170
			 0x777ed82e, /// 0x174
			 0x6794c923, /// 0x178
			 0x8a6cec91, /// 0x17c
			 0x97bdf5aa, /// 0x180
			 0xe2df1001, /// 0x184
			 0x69a9befe, /// 0x188
			 0x84adc2c6, /// 0x18c
			 0xd5bf23d4, /// 0x190
			 0x05ec27c7, /// 0x194
			 0xe217ceae, /// 0x198
			 0x2290ab50, /// 0x19c
			 0x8cdcdb26, /// 0x1a0
			 0xebb3b808, /// 0x1a4
			 0x3fedf8a5, /// 0x1a8
			 0xbe43ace0, /// 0x1ac
			 0x661d49f7, /// 0x1b0
			 0x97ac7935, /// 0x1b4
			 0x97cb9979, /// 0x1b8
			 0x7f0a4772, /// 0x1bc
			 0x58461341, /// 0x1c0
			 0x3c8e87df, /// 0x1c4
			 0x5fe7f0be, /// 0x1c8
			 0x27513514, /// 0x1cc
			 0xe485e571, /// 0x1d0
			 0x95fa9b0b, /// 0x1d4
			 0x26b01dd7, /// 0x1d8
			 0x9d20043b, /// 0x1dc
			 0xbcaca417, /// 0x1e0
			 0xab57f6f3, /// 0x1e4
			 0xf9383a25, /// 0x1e8
			 0x70bd05bb, /// 0x1ec
			 0xeb056d0d, /// 0x1f0
			 0x0912298a, /// 0x1f4
			 0x3a4da848, /// 0x1f8
			 0xccfa966a, /// 0x1fc
			 0x9b59b171, /// 0x200
			 0x6c01e07c, /// 0x204
			 0xf610d4f7, /// 0x208
			 0x6c4f8c66, /// 0x20c
			 0xd20d2e4b, /// 0x210
			 0x6af74771, /// 0x214
			 0xbf26bbb0, /// 0x218
			 0x99425b49, /// 0x21c
			 0xdf5b217f, /// 0x220
			 0xe5a8b584, /// 0x224
			 0xead8f056, /// 0x228
			 0x6105f0f9, /// 0x22c
			 0xc55d6c57, /// 0x230
			 0x9c1bb384, /// 0x234
			 0x6c07cc7d, /// 0x238
			 0x84c2dd6e, /// 0x23c
			 0xb2a8ef17, /// 0x240
			 0x782efa64, /// 0x244
			 0x89d718bb, /// 0x248
			 0x80943a2d, /// 0x24c
			 0x4391b4e0, /// 0x250
			 0x5d2441bf, /// 0x254
			 0xb651f762, /// 0x258
			 0xa4983aeb, /// 0x25c
			 0x0690e91d, /// 0x260
			 0x06d14f26, /// 0x264
			 0xd8cb2031, /// 0x268
			 0x4e9aa28b, /// 0x26c
			 0x1fb09119, /// 0x270
			 0xfe5cc9d9, /// 0x274
			 0x2f84c009, /// 0x278
			 0x566a7791, /// 0x27c
			 0x54f95bff, /// 0x280
			 0x89488ff8, /// 0x284
			 0xd67c62e0, /// 0x288
			 0xec37af2d, /// 0x28c
			 0x88ec1cd4, /// 0x290
			 0x536c67f4, /// 0x294
			 0xf7e0d719, /// 0x298
			 0xfa3c75ff, /// 0x29c
			 0xa01b29f1, /// 0x2a0
			 0x35835782, /// 0x2a4
			 0x4e03e5a8, /// 0x2a8
			 0x7ded42bb, /// 0x2ac
			 0x235ce8f9, /// 0x2b0
			 0x189ca71c, /// 0x2b4
			 0x28b80b27, /// 0x2b8
			 0x7570b57d, /// 0x2bc
			 0x65e3eac4, /// 0x2c0
			 0x7d101edb, /// 0x2c4
			 0x455ed5cf, /// 0x2c8
			 0x989d89d2, /// 0x2cc
			 0xcfcccc71, /// 0x2d0
			 0x7c29dfbc, /// 0x2d4
			 0xd2755c20, /// 0x2d8
			 0x13839464, /// 0x2dc
			 0x472b595d, /// 0x2e0
			 0x835f7ad8, /// 0x2e4
			 0x0576c944, /// 0x2e8
			 0x898f0827, /// 0x2ec
			 0x4b383c9e, /// 0x2f0
			 0xd0184fee, /// 0x2f4
			 0x90820566, /// 0x2f8
			 0x9b2374cc, /// 0x2fc
			 0xa18657d8, /// 0x300
			 0x69211912, /// 0x304
			 0x8c17a770, /// 0x308
			 0xc070a839, /// 0x30c
			 0xcbb90516, /// 0x310
			 0x9e58f057, /// 0x314
			 0xb2c5f9f0, /// 0x318
			 0x85a54078, /// 0x31c
			 0x40b59e31, /// 0x320
			 0x8e732fd9, /// 0x324
			 0x8b474b45, /// 0x328
			 0x2c52d937, /// 0x32c
			 0xcb352c32, /// 0x330
			 0xe88e95a5, /// 0x334
			 0x0d83024a, /// 0x338
			 0x549623a4, /// 0x33c
			 0xa8b2f591, /// 0x340
			 0x191a8404, /// 0x344
			 0xf91fdbff, /// 0x348
			 0xf30dd391, /// 0x34c
			 0xd853f500, /// 0x350
			 0xa33bef19, /// 0x354
			 0xefeee792, /// 0x358
			 0xcaecfa76, /// 0x35c
			 0x9c7e2809, /// 0x360
			 0x54cbb530, /// 0x364
			 0xbd0c0ea7, /// 0x368
			 0x4d4d54f2, /// 0x36c
			 0x02f209ce, /// 0x370
			 0xe5e55aad, /// 0x374
			 0x8db57ba8, /// 0x378
			 0x6f30c53e, /// 0x37c
			 0x65e9a0fc, /// 0x380
			 0x31f14199, /// 0x384
			 0x7cb5e40e, /// 0x388
			 0xdbf3c5ab, /// 0x38c
			 0x246929a9, /// 0x390
			 0xb7e88b1b, /// 0x394
			 0x2e79d14f, /// 0x398
			 0x422d7ad2, /// 0x39c
			 0x2ff981f8, /// 0x3a0
			 0x045f201b, /// 0x3a4
			 0x50a89a1f, /// 0x3a8
			 0x79dd487d, /// 0x3ac
			 0x5e0ba084, /// 0x3b0
			 0x0e82fa54, /// 0x3b4
			 0x01421558, /// 0x3b8
			 0x180f8302, /// 0x3bc
			 0xf6cf062b, /// 0x3c0
			 0xae865939, /// 0x3c4
			 0x19b54d3d, /// 0x3c8
			 0x7ba59c0e, /// 0x3cc
			 0x6d80798f, /// 0x3d0
			 0xdb0df8d8, /// 0x3d4
			 0x6770c7ea, /// 0x3d8
			 0x69f7a990, /// 0x3dc
			 0x475c3fdf, /// 0x3e0
			 0x2660c577, /// 0x3e4
			 0xa082cdeb, /// 0x3e8
			 0x99319aa8, /// 0x3ec
			 0xfe914f0a, /// 0x3f0
			 0xba69fa73, /// 0x3f4
			 0x1e89a09c, /// 0x3f8
			 0xad685caa, /// 0x3fc
			 0x91dbd210, /// 0x400
			 0x453f0de4, /// 0x404
			 0xe0c6bd92, /// 0x408
			 0x1cf0d9e8, /// 0x40c
			 0xeea9a9a8, /// 0x410
			 0xaf5f7de5, /// 0x414
			 0xd6e73095, /// 0x418
			 0xd76c23ac, /// 0x41c
			 0x91aa88f1, /// 0x420
			 0x2ce26207, /// 0x424
			 0xee79509e, /// 0x428
			 0xab0b5917, /// 0x42c
			 0x5a12f0b4, /// 0x430
			 0xf0bbbb9c, /// 0x434
			 0x8cd47fdc, /// 0x438
			 0xe6c2dcd0, /// 0x43c
			 0x01102d2b, /// 0x440
			 0x897ff0e3, /// 0x444
			 0x7b861cc2, /// 0x448
			 0x6fb74083, /// 0x44c
			 0x7fabd424, /// 0x450
			 0x326ba239, /// 0x454
			 0x7f1b856a, /// 0x458
			 0x9dc1711b, /// 0x45c
			 0x2472e73e, /// 0x460
			 0x47d602aa, /// 0x464
			 0xfee674f8, /// 0x468
			 0x58103972, /// 0x46c
			 0x5c3535a4, /// 0x470
			 0x2d9eb0a9, /// 0x474
			 0xb402c613, /// 0x478
			 0xf197a3e8, /// 0x47c
			 0x97e5e830, /// 0x480
			 0x1837b0af, /// 0x484
			 0xba3c606d, /// 0x488
			 0xbeb5b45f, /// 0x48c
			 0x1b0bc8be, /// 0x490
			 0xf20fecb2, /// 0x494
			 0xa870574d, /// 0x498
			 0x0e57bd50, /// 0x49c
			 0x1e9a2c2a, /// 0x4a0
			 0xaacbcc98, /// 0x4a4
			 0xbe827a00, /// 0x4a8
			 0x4721d8e5, /// 0x4ac
			 0xedcda0fa, /// 0x4b0
			 0x9394cee3, /// 0x4b4
			 0x8491ebfa, /// 0x4b8
			 0x14150e53, /// 0x4bc
			 0x0fdf5fde, /// 0x4c0
			 0xa67b6d31, /// 0x4c4
			 0xa9f084c6, /// 0x4c8
			 0x5a80e9a9, /// 0x4cc
			 0xc69d2dbc, /// 0x4d0
			 0xa7348e38, /// 0x4d4
			 0x3b56b151, /// 0x4d8
			 0x8f87f0f4, /// 0x4dc
			 0xec53f1fd, /// 0x4e0
			 0xeb5f5622, /// 0x4e4
			 0x2697e6f4, /// 0x4e8
			 0x78c7d83b, /// 0x4ec
			 0x92b66485, /// 0x4f0
			 0xf000f042, /// 0x4f4
			 0x064f0f9b, /// 0x4f8
			 0x5fac9388, /// 0x4fc
			 0x950cebf3, /// 0x500
			 0xfe5c0561, /// 0x504
			 0x72084bd2, /// 0x508
			 0xd101cbc8, /// 0x50c
			 0xf20bc763, /// 0x510
			 0xafe4508d, /// 0x514
			 0x71b07d8d, /// 0x518
			 0x9052db44, /// 0x51c
			 0x6a3eb1bc, /// 0x520
			 0x0dd73ed7, /// 0x524
			 0xe18361df, /// 0x528
			 0xcdf8b264, /// 0x52c
			 0x8f72022f, /// 0x530
			 0x228cdb1d, /// 0x534
			 0xd0b20ca4, /// 0x538
			 0x79b38c6f, /// 0x53c
			 0xc3bb12b2, /// 0x540
			 0xdb238be5, /// 0x544
			 0x71d2912e, /// 0x548
			 0xf064f95c, /// 0x54c
			 0xc02f301a, /// 0x550
			 0xf4a79d3a, /// 0x554
			 0x1af71b73, /// 0x558
			 0x82e817d3, /// 0x55c
			 0x90f95d90, /// 0x560
			 0x9d271082, /// 0x564
			 0xc9e0b52e, /// 0x568
			 0x6cf01c00, /// 0x56c
			 0x23c1b84b, /// 0x570
			 0xc3e63884, /// 0x574
			 0xc647364f, /// 0x578
			 0x8a76f33c, /// 0x57c
			 0x48167bc2, /// 0x580
			 0x8191fbf4, /// 0x584
			 0xcb0af458, /// 0x588
			 0x40f9f318, /// 0x58c
			 0x400d08c4, /// 0x590
			 0x388dc378, /// 0x594
			 0x8194d561, /// 0x598
			 0x220a5f97, /// 0x59c
			 0x8223b4d4, /// 0x5a0
			 0x59398489, /// 0x5a4
			 0x342a922a, /// 0x5a8
			 0x1b38a4c6, /// 0x5ac
			 0x31e4492d, /// 0x5b0
			 0xe585a62a, /// 0x5b4
			 0x64e651b9, /// 0x5b8
			 0x92405368, /// 0x5bc
			 0x1856f243, /// 0x5c0
			 0xf8262506, /// 0x5c4
			 0x39574f9e, /// 0x5c8
			 0xc24b3e65, /// 0x5cc
			 0x0ea01a67, /// 0x5d0
			 0x76ba0889, /// 0x5d4
			 0x082667bf, /// 0x5d8
			 0x9965b273, /// 0x5dc
			 0x4e741444, /// 0x5e0
			 0xe45e938c, /// 0x5e4
			 0x56452371, /// 0x5e8
			 0x64e2c73a, /// 0x5ec
			 0x32ef3bed, /// 0x5f0
			 0x2bbe8c64, /// 0x5f4
			 0x99c7c8fd, /// 0x5f8
			 0xc166f47e, /// 0x5fc
			 0x500cd7fd, /// 0x600
			 0xe7c7a288, /// 0x604
			 0xded02ed0, /// 0x608
			 0x1dad8c71, /// 0x60c
			 0xfbbb7996, /// 0x610
			 0x868dad12, /// 0x614
			 0x9721bda3, /// 0x618
			 0xfb7e5be7, /// 0x61c
			 0x22ba0916, /// 0x620
			 0x0ea1029e, /// 0x624
			 0xead034f3, /// 0x628
			 0x4ed07fc5, /// 0x62c
			 0x6fb01120, /// 0x630
			 0x4e0dc8f3, /// 0x634
			 0x6c4d9594, /// 0x638
			 0x4f9b6b5a, /// 0x63c
			 0xd37af4f5, /// 0x640
			 0x037ccf03, /// 0x644
			 0xf1d301d3, /// 0x648
			 0x2f48cabb, /// 0x64c
			 0x1cb9a69a, /// 0x650
			 0x6cf63d08, /// 0x654
			 0x8f8eb4f0, /// 0x658
			 0x41e9aa19, /// 0x65c
			 0xd05e9d5d, /// 0x660
			 0xcc699ab9, /// 0x664
			 0xc30aaf04, /// 0x668
			 0xa465e2b2, /// 0x66c
			 0x850ceb77, /// 0x670
			 0xf90db50a, /// 0x674
			 0x4a47ce1a, /// 0x678
			 0x5cd38f2b, /// 0x67c
			 0xc41d433c, /// 0x680
			 0x81a4a480, /// 0x684
			 0x7d3fe4ec, /// 0x688
			 0xe6e842ed, /// 0x68c
			 0xb8b451fd, /// 0x690
			 0x89916fc1, /// 0x694
			 0x62c2411d, /// 0x698
			 0x9d375982, /// 0x69c
			 0x40008137, /// 0x6a0
			 0xad8192d4, /// 0x6a4
			 0xee9e8828, /// 0x6a8
			 0xbe16190f, /// 0x6ac
			 0x3ebfe13f, /// 0x6b0
			 0xd5babfff, /// 0x6b4
			 0x4790afbd, /// 0x6b8
			 0xa258ec66, /// 0x6bc
			 0x93a21c86, /// 0x6c0
			 0x80dca188, /// 0x6c4
			 0x73bef309, /// 0x6c8
			 0x37c7379f, /// 0x6cc
			 0x993314f5, /// 0x6d0
			 0xca0f97d6, /// 0x6d4
			 0xb20439f9, /// 0x6d8
			 0x0632acfa, /// 0x6dc
			 0xf4ac4b31, /// 0x6e0
			 0x8657f198, /// 0x6e4
			 0x061ce6b1, /// 0x6e8
			 0xb2592814, /// 0x6ec
			 0xbcfe0067, /// 0x6f0
			 0x0e66ba45, /// 0x6f4
			 0x56f655c7, /// 0x6f8
			 0x5862e03d, /// 0x6fc
			 0x85244fb7, /// 0x700
			 0x55878a4f, /// 0x704
			 0xd3b849ba, /// 0x708
			 0x6b34380d, /// 0x70c
			 0xe13292c4, /// 0x710
			 0x2331ceba, /// 0x714
			 0xf9212dac, /// 0x718
			 0xc3f847fa, /// 0x71c
			 0x9e53ca5c, /// 0x720
			 0x93a8b7c8, /// 0x724
			 0x17739690, /// 0x728
			 0x643a5021, /// 0x72c
			 0x6fcd7943, /// 0x730
			 0xef50155f, /// 0x734
			 0xb04e9c96, /// 0x738
			 0x77c122ee, /// 0x73c
			 0x3671ebee, /// 0x740
			 0x4c4db0ce, /// 0x744
			 0xff99cf4b, /// 0x748
			 0x49e902e9, /// 0x74c
			 0x5a0ed52a, /// 0x750
			 0x974ee1ec, /// 0x754
			 0x0723464f, /// 0x758
			 0xd9bad354, /// 0x75c
			 0x0300bb74, /// 0x760
			 0xe0ae0f60, /// 0x764
			 0xffea4a96, /// 0x768
			 0x776b401b, /// 0x76c
			 0x7d2e05c8, /// 0x770
			 0x023e536c, /// 0x774
			 0xa9e7ac0c, /// 0x778
			 0x2d8bd7d7, /// 0x77c
			 0xcec08cba, /// 0x780
			 0x28c8362e, /// 0x784
			 0xbfe91d25, /// 0x788
			 0xe62855b5, /// 0x78c
			 0x35d7e692, /// 0x790
			 0x5a65e0f7, /// 0x794
			 0xda53671f, /// 0x798
			 0xf738ed73, /// 0x79c
			 0x1580a625, /// 0x7a0
			 0xaea74e25, /// 0x7a4
			 0x235641ad, /// 0x7a8
			 0x35a12acf, /// 0x7ac
			 0xe01941ed, /// 0x7b0
			 0xaefa8de8, /// 0x7b4
			 0xe6199d25, /// 0x7b8
			 0xafc5b99d, /// 0x7bc
			 0xc2bf2346, /// 0x7c0
			 0xaad9f646, /// 0x7c4
			 0xf273ffd3, /// 0x7c8
			 0x71f54f9b, /// 0x7cc
			 0x60a778b2, /// 0x7d0
			 0x1565f45f, /// 0x7d4
			 0x476aa906, /// 0x7d8
			 0xa118a381, /// 0x7dc
			 0x41b9d071, /// 0x7e0
			 0x78d947bf, /// 0x7e4
			 0x67adefa8, /// 0x7e8
			 0x3f5ce56f, /// 0x7ec
			 0x7d458b31, /// 0x7f0
			 0x5dad9887, /// 0x7f4
			 0xe446d3c1, /// 0x7f8
			 0x5c873ec9, /// 0x7fc
			 0xbf17ff58, /// 0x800
			 0x862a4d4b, /// 0x804
			 0x441d6d9c, /// 0x808
			 0x5d627bb2, /// 0x80c
			 0x06233f72, /// 0x810
			 0x85eeb049, /// 0x814
			 0xbedce358, /// 0x818
			 0x7c47bccb, /// 0x81c
			 0x4f42ae63, /// 0x820
			 0x9ba18223, /// 0x824
			 0x67b7a500, /// 0x828
			 0x21186612, /// 0x82c
			 0x9210b9d2, /// 0x830
			 0x15fe9a71, /// 0x834
			 0xe7697169, /// 0x838
			 0x63f1689c, /// 0x83c
			 0x2ad3055c, /// 0x840
			 0x637739cb, /// 0x844
			 0x92a85ead, /// 0x848
			 0xb9e4a2af, /// 0x84c
			 0xf96072bb, /// 0x850
			 0xba0d3367, /// 0x854
			 0xdf480680, /// 0x858
			 0x2b7e9588, /// 0x85c
			 0xd2c14b2a, /// 0x860
			 0x4eddfc14, /// 0x864
			 0x4291b1cf, /// 0x868
			 0x1c3d6038, /// 0x86c
			 0x32f92221, /// 0x870
			 0x60bda046, /// 0x874
			 0xf02ea0ef, /// 0x878
			 0x873bf2c8, /// 0x87c
			 0x34250604, /// 0x880
			 0x1f716abc, /// 0x884
			 0x84dae4b4, /// 0x888
			 0xc4522b9d, /// 0x88c
			 0x5819c5b7, /// 0x890
			 0x29748806, /// 0x894
			 0xa4b0254f, /// 0x898
			 0x3a416e6d, /// 0x89c
			 0x67cb9200, /// 0x8a0
			 0x2708aff7, /// 0x8a4
			 0x4d5fa3fa, /// 0x8a8
			 0x602f76f0, /// 0x8ac
			 0xdaabdc45, /// 0x8b0
			 0xf691ebda, /// 0x8b4
			 0x24e6762b, /// 0x8b8
			 0x8e183f55, /// 0x8bc
			 0xc160504e, /// 0x8c0
			 0xd282b31d, /// 0x8c4
			 0xe4967ab8, /// 0x8c8
			 0xc240e32b, /// 0x8cc
			 0x24054251, /// 0x8d0
			 0xa8cca3cf, /// 0x8d4
			 0xe7a76531, /// 0x8d8
			 0x8c33ae4e, /// 0x8dc
			 0x6e018747, /// 0x8e0
			 0x5263da9b, /// 0x8e4
			 0xab6b9f60, /// 0x8e8
			 0x2921f088, /// 0x8ec
			 0xd71c9ba5, /// 0x8f0
			 0x680990a4, /// 0x8f4
			 0x40bcfb0f, /// 0x8f8
			 0xeba1626b, /// 0x8fc
			 0x85a9d10a, /// 0x900
			 0x40582d77, /// 0x904
			 0x872efa3c, /// 0x908
			 0xc7d2fd1c, /// 0x90c
			 0x035c41d3, /// 0x910
			 0x1e260f7b, /// 0x914
			 0xada5d975, /// 0x918
			 0x12e6508c, /// 0x91c
			 0xb22dfb01, /// 0x920
			 0x9f7f472f, /// 0x924
			 0xe58a4537, /// 0x928
			 0x53e5b489, /// 0x92c
			 0x9e4bf7c0, /// 0x930
			 0x50af0c38, /// 0x934
			 0xbd5d39a8, /// 0x938
			 0x05856866, /// 0x93c
			 0xb76f5a35, /// 0x940
			 0xc2947c77, /// 0x944
			 0x76384020, /// 0x948
			 0xc9d4ca7d, /// 0x94c
			 0x987af743, /// 0x950
			 0xe81fb17a, /// 0x954
			 0xa32df7f1, /// 0x958
			 0x8c654b36, /// 0x95c
			 0x330c77c4, /// 0x960
			 0xf6b41fec, /// 0x964
			 0x6843680d, /// 0x968
			 0xa7507949, /// 0x96c
			 0xcd8567b5, /// 0x970
			 0x94721d59, /// 0x974
			 0xfbceabed, /// 0x978
			 0x377e4513, /// 0x97c
			 0x2218c0d2, /// 0x980
			 0x23e94b5b, /// 0x984
			 0x538f6377, /// 0x988
			 0xaf29e07b, /// 0x98c
			 0x4c3c4281, /// 0x990
			 0x8c21bffd, /// 0x994
			 0x15e023bb, /// 0x998
			 0xa2bd5123, /// 0x99c
			 0xd56839ae, /// 0x9a0
			 0x3031ac84, /// 0x9a4
			 0x85e59113, /// 0x9a8
			 0x9d1def9b, /// 0x9ac
			 0x645a436f, /// 0x9b0
			 0xab098a62, /// 0x9b4
			 0x29e0ce59, /// 0x9b8
			 0x38b365bd, /// 0x9bc
			 0x2fb81e0a, /// 0x9c0
			 0x3b7a9be5, /// 0x9c4
			 0x1f6d164e, /// 0x9c8
			 0x2a3ba3d2, /// 0x9cc
			 0xdc127c37, /// 0x9d0
			 0x5742b1da, /// 0x9d4
			 0x60724492, /// 0x9d8
			 0xb51fcc95, /// 0x9dc
			 0x6bbd7c7e, /// 0x9e0
			 0x7c35d6a2, /// 0x9e4
			 0xb3af3f5e, /// 0x9e8
			 0x8df4f6b5, /// 0x9ec
			 0x440603a1, /// 0x9f0
			 0xeb7dbf9c, /// 0x9f4
			 0xf228b1f7, /// 0x9f8
			 0x1d0eed7d, /// 0x9fc
			 0x62bb1ef3, /// 0xa00
			 0x3cec8167, /// 0xa04
			 0xa77f5110, /// 0xa08
			 0x0912d267, /// 0xa0c
			 0x93e4d923, /// 0xa10
			 0x5ab60815, /// 0xa14
			 0xa7d34af3, /// 0xa18
			 0x47231f26, /// 0xa1c
			 0x46511c2f, /// 0xa20
			 0x2356e6b9, /// 0xa24
			 0xfc14beda, /// 0xa28
			 0x38f3491f, /// 0xa2c
			 0x051fbc28, /// 0xa30
			 0xb5b59779, /// 0xa34
			 0x340dae10, /// 0xa38
			 0xd8814902, /// 0xa3c
			 0xccd349b2, /// 0xa40
			 0xf7b972ec, /// 0xa44
			 0x862bc1c4, /// 0xa48
			 0x54a73629, /// 0xa4c
			 0x836d91c6, /// 0xa50
			 0xd452c478, /// 0xa54
			 0xaa0ba9b1, /// 0xa58
			 0x8ad84263, /// 0xa5c
			 0x6c11597c, /// 0xa60
			 0x08d03078, /// 0xa64
			 0x32d7e40b, /// 0xa68
			 0x17002556, /// 0xa6c
			 0x33496f56, /// 0xa70
			 0xe58f5eb8, /// 0xa74
			 0xc6f7869d, /// 0xa78
			 0x6cc3fdd7, /// 0xa7c
			 0x0e7fb086, /// 0xa80
			 0xe54c0997, /// 0xa84
			 0x692c9390, /// 0xa88
			 0xd3e1815f, /// 0xa8c
			 0x80cebb42, /// 0xa90
			 0x9dd2370e, /// 0xa94
			 0xb099baf6, /// 0xa98
			 0xd4c7e193, /// 0xa9c
			 0x5f6df9bf, /// 0xaa0
			 0x0a82ca74, /// 0xaa4
			 0xf99f35d4, /// 0xaa8
			 0xe1091046, /// 0xaac
			 0xd6466c27, /// 0xab0
			 0xf9905fd7, /// 0xab4
			 0xc60f2f65, /// 0xab8
			 0x2fadd55e, /// 0xabc
			 0x21cd14ba, /// 0xac0
			 0xf6a75450, /// 0xac4
			 0xd4e8a506, /// 0xac8
			 0x3d905502, /// 0xacc
			 0x4c24a376, /// 0xad0
			 0xe22b8746, /// 0xad4
			 0x0824ee81, /// 0xad8
			 0xdc182fa9, /// 0xadc
			 0x2f937dc9, /// 0xae0
			 0xb1786030, /// 0xae4
			 0x793584a3, /// 0xae8
			 0x2afe46a6, /// 0xaec
			 0x3b3ff81e, /// 0xaf0
			 0x2c3e23f3, /// 0xaf4
			 0x3579f835, /// 0xaf8
			 0x325cfc99, /// 0xafc
			 0x63f9abc4, /// 0xb00
			 0x060a3a32, /// 0xb04
			 0x559322fd, /// 0xb08
			 0x69a58cd1, /// 0xb0c
			 0xd3425b0c, /// 0xb10
			 0xa899937e, /// 0xb14
			 0x57792bba, /// 0xb18
			 0xae322d40, /// 0xb1c
			 0xa6087b9f, /// 0xb20
			 0x71e1961f, /// 0xb24
			 0x67ef8381, /// 0xb28
			 0x567426da, /// 0xb2c
			 0xe46f8a0d, /// 0xb30
			 0xb3d8c51a, /// 0xb34
			 0x8caf9f0e, /// 0xb38
			 0xb77de2ac, /// 0xb3c
			 0xf901e6d0, /// 0xb40
			 0x74481655, /// 0xb44
			 0x54334474, /// 0xb48
			 0xdceb2b8a, /// 0xb4c
			 0x831cd714, /// 0xb50
			 0xf0d1c939, /// 0xb54
			 0x9a94b9e6, /// 0xb58
			 0x9712d413, /// 0xb5c
			 0xb540ae89, /// 0xb60
			 0xb3a7346a, /// 0xb64
			 0xb884501f, /// 0xb68
			 0xeeae4be2, /// 0xb6c
			 0x03cd3b3a, /// 0xb70
			 0xb591617f, /// 0xb74
			 0x392abcdd, /// 0xb78
			 0x4ee99616, /// 0xb7c
			 0xea3cd5bf, /// 0xb80
			 0x79a0494d, /// 0xb84
			 0x7532b996, /// 0xb88
			 0x37b358f0, /// 0xb8c
			 0x91019a73, /// 0xb90
			 0xe17603ed, /// 0xb94
			 0x76c2fd27, /// 0xb98
			 0xa025f93f, /// 0xb9c
			 0x40c871ac, /// 0xba0
			 0x1cd482c5, /// 0xba4
			 0x65e1c47f, /// 0xba8
			 0x770a9fa5, /// 0xbac
			 0x0fb2e11c, /// 0xbb0
			 0x5d4256d6, /// 0xbb4
			 0x9f795926, /// 0xbb8
			 0x74574402, /// 0xbbc
			 0x4be331ef, /// 0xbc0
			 0x9638b9a4, /// 0xbc4
			 0x2a76ca38, /// 0xbc8
			 0xb1c78758, /// 0xbcc
			 0xdead531d, /// 0xbd0
			 0xf98ea7fb, /// 0xbd4
			 0x99a075fc, /// 0xbd8
			 0x4b24105b, /// 0xbdc
			 0xa8c5b25e, /// 0xbe0
			 0x6cfa7d4d, /// 0xbe4
			 0x801745a4, /// 0xbe8
			 0xbc6b52ad, /// 0xbec
			 0x86840a82, /// 0xbf0
			 0x0322fb2e, /// 0xbf4
			 0x9445dbeb, /// 0xbf8
			 0x3dd5946c, /// 0xbfc
			 0xe3d6e32e, /// 0xc00
			 0x8e364d93, /// 0xc04
			 0x32627aec, /// 0xc08
			 0xf9b1233c, /// 0xc0c
			 0x24186876, /// 0xc10
			 0xcc7baee1, /// 0xc14
			 0xab6e2d23, /// 0xc18
			 0x104f971d, /// 0xc1c
			 0x4f4f9c1e, /// 0xc20
			 0x1de1aed6, /// 0xc24
			 0x8dd6a40a, /// 0xc28
			 0x0c3f1e3b, /// 0xc2c
			 0x160bc6be, /// 0xc30
			 0xce79ce1a, /// 0xc34
			 0xeffde994, /// 0xc38
			 0xd17329fa, /// 0xc3c
			 0xe0e93d93, /// 0xc40
			 0xe1ad4793, /// 0xc44
			 0x426def7e, /// 0xc48
			 0x39272a2d, /// 0xc4c
			 0x82ee7e16, /// 0xc50
			 0x01b8f55a, /// 0xc54
			 0x0e2049e1, /// 0xc58
			 0xfb9e0a11, /// 0xc5c
			 0xbc153f9c, /// 0xc60
			 0x539286d0, /// 0xc64
			 0x8b4cc8ae, /// 0xc68
			 0xa33ef748, /// 0xc6c
			 0x65bb0f92, /// 0xc70
			 0xcf50dbe2, /// 0xc74
			 0x69a1b61c, /// 0xc78
			 0x764605ac, /// 0xc7c
			 0xf88d1fad, /// 0xc80
			 0xc089d236, /// 0xc84
			 0x982e5f56, /// 0xc88
			 0x2fda65f7, /// 0xc8c
			 0xbe6133b0, /// 0xc90
			 0x3795088e, /// 0xc94
			 0xdd78454a, /// 0xc98
			 0x6ba31bd7, /// 0xc9c
			 0x334781ee, /// 0xca0
			 0x39c0db33, /// 0xca4
			 0x3407ed29, /// 0xca8
			 0xca71ff51, /// 0xcac
			 0xaa3c154e, /// 0xcb0
			 0xade9e932, /// 0xcb4
			 0x1aa51afd, /// 0xcb8
			 0xaa35a228, /// 0xcbc
			 0xa9d8ec50, /// 0xcc0
			 0x28f90474, /// 0xcc4
			 0xb16ba6ea, /// 0xcc8
			 0x4df882dd, /// 0xccc
			 0xbd0154f5, /// 0xcd0
			 0xc2a8c6f8, /// 0xcd4
			 0x6835dd0d, /// 0xcd8
			 0x81547380, /// 0xcdc
			 0xe0d320e9, /// 0xce0
			 0xdd805def, /// 0xce4
			 0x765b075a, /// 0xce8
			 0x5d1f573f, /// 0xcec
			 0x15b51f36, /// 0xcf0
			 0x93167cec, /// 0xcf4
			 0x2c158307, /// 0xcf8
			 0x2b902787, /// 0xcfc
			 0xe8b2107f, /// 0xd00
			 0x5a85fe9e, /// 0xd04
			 0xa326087e, /// 0xd08
			 0x0bcf0075, /// 0xd0c
			 0x02f66d79, /// 0xd10
			 0xf09fe3a3, /// 0xd14
			 0x071fde0f, /// 0xd18
			 0xe192bf82, /// 0xd1c
			 0x087fc073, /// 0xd20
			 0x0667fc19, /// 0xd24
			 0x1529e24a, /// 0xd28
			 0x22337d26, /// 0xd2c
			 0xb8c4d00c, /// 0xd30
			 0x9b8265b8, /// 0xd34
			 0x4f9e5fb2, /// 0xd38
			 0x3ec71196, /// 0xd3c
			 0xf8551f16, /// 0xd40
			 0x597afe00, /// 0xd44
			 0x77b3c214, /// 0xd48
			 0x12d8b108, /// 0xd4c
			 0xaf8e7c32, /// 0xd50
			 0xefad551b, /// 0xd54
			 0x7600c32d, /// 0xd58
			 0xd3bb5ab9, /// 0xd5c
			 0xeb3bc3ee, /// 0xd60
			 0x5afde05f, /// 0xd64
			 0x5373b8c0, /// 0xd68
			 0x1398adef, /// 0xd6c
			 0xfeb853c7, /// 0xd70
			 0x72716780, /// 0xd74
			 0x58fb7fa8, /// 0xd78
			 0xecad9f9c, /// 0xd7c
			 0x70917191, /// 0xd80
			 0xfeab07e7, /// 0xd84
			 0x8aab55aa, /// 0xd88
			 0xdacedc71, /// 0xd8c
			 0x50d08c08, /// 0xd90
			 0xd4004923, /// 0xd94
			 0x1216bd8e, /// 0xd98
			 0xe8d33246, /// 0xd9c
			 0x6dcc8bb7, /// 0xda0
			 0x8856506b, /// 0xda4
			 0x059464d4, /// 0xda8
			 0x59b3eb06, /// 0xdac
			 0xd41e24fd, /// 0xdb0
			 0x3a53fea2, /// 0xdb4
			 0xb57aa39c, /// 0xdb8
			 0xba75ff3f, /// 0xdbc
			 0xe924d237, /// 0xdc0
			 0x2e7b19e6, /// 0xdc4
			 0xb6b62b36, /// 0xdc8
			 0x5eac6a77, /// 0xdcc
			 0x93ae74c1, /// 0xdd0
			 0xfa6e5b23, /// 0xdd4
			 0xcc7aea82, /// 0xdd8
			 0xf7e5d67d, /// 0xddc
			 0x5ca78601, /// 0xde0
			 0xfce0a7cb, /// 0xde4
			 0x1278c2dc, /// 0xde8
			 0xdf9f1737, /// 0xdec
			 0x0d9f8689, /// 0xdf0
			 0xcc760d35, /// 0xdf4
			 0x1b0dfe7b, /// 0xdf8
			 0xe224429d, /// 0xdfc
			 0xf5ceb281, /// 0xe00
			 0x22f33545, /// 0xe04
			 0x0af704a2, /// 0xe08
			 0x1c9dacca, /// 0xe0c
			 0x365cbc17, /// 0xe10
			 0x79abc882, /// 0xe14
			 0x0f8d08ff, /// 0xe18
			 0x70d3b22f, /// 0xe1c
			 0xab5e51d6, /// 0xe20
			 0x04eecfbf, /// 0xe24
			 0x4ad3a948, /// 0xe28
			 0xf0bcdba3, /// 0xe2c
			 0xc769d949, /// 0xe30
			 0x3bad18db, /// 0xe34
			 0x425051f9, /// 0xe38
			 0xfd57e363, /// 0xe3c
			 0x6b8f647a, /// 0xe40
			 0x762ccc0c, /// 0xe44
			 0x609072ed, /// 0xe48
			 0x8ebea5bd, /// 0xe4c
			 0xfd2764eb, /// 0xe50
			 0x6350581a, /// 0xe54
			 0xd630c324, /// 0xe58
			 0x993d9f25, /// 0xe5c
			 0x58b28b3d, /// 0xe60
			 0xa4103099, /// 0xe64
			 0x2b3f435a, /// 0xe68
			 0xd66e8f8f, /// 0xe6c
			 0xd87ccd4a, /// 0xe70
			 0x50459240, /// 0xe74
			 0x9b537e13, /// 0xe78
			 0x5d5c28a0, /// 0xe7c
			 0x371fe0f5, /// 0xe80
			 0xe5d38574, /// 0xe84
			 0xd01e8ba8, /// 0xe88
			 0xe79bb8a5, /// 0xe8c
			 0x9880c3b1, /// 0xe90
			 0xab0585aa, /// 0xe94
			 0x1a0afdf1, /// 0xe98
			 0x78073a70, /// 0xe9c
			 0x577d7285, /// 0xea0
			 0xe29be07e, /// 0xea4
			 0x9d140d7e, /// 0xea8
			 0x13818bb3, /// 0xeac
			 0xb22dce61, /// 0xeb0
			 0x6d1f378c, /// 0xeb4
			 0xcec3535e, /// 0xeb8
			 0xdb0a078d, /// 0xebc
			 0xcacce7bf, /// 0xec0
			 0xd379d9ae, /// 0xec4
			 0xd7dceb0e, /// 0xec8
			 0xfd2c01c2, /// 0xecc
			 0x4d24cbce, /// 0xed0
			 0xd351bc76, /// 0xed4
			 0x169ef8c4, /// 0xed8
			 0x6906fec8, /// 0xedc
			 0x59274aff, /// 0xee0
			 0xc3f3284b, /// 0xee4
			 0xddce2250, /// 0xee8
			 0xdce5f75d, /// 0xeec
			 0x28e48203, /// 0xef0
			 0x06761324, /// 0xef4
			 0x07f2aed8, /// 0xef8
			 0xc15b7d48, /// 0xefc
			 0x3cca4dc6, /// 0xf00
			 0xc8a07f03, /// 0xf04
			 0xe281a857, /// 0xf08
			 0x800dc847, /// 0xf0c
			 0xa38301ca, /// 0xf10
			 0xf08b269a, /// 0xf14
			 0xd5874d4d, /// 0xf18
			 0x80ed0475, /// 0xf1c
			 0x0a9b52c1, /// 0xf20
			 0x0a11262f, /// 0xf24
			 0x62edc6eb, /// 0xf28
			 0x3c9fa3bc, /// 0xf2c
			 0x80b6c3cc, /// 0xf30
			 0xf2c979c4, /// 0xf34
			 0xebfee4a6, /// 0xf38
			 0xc65f6cad, /// 0xf3c
			 0xaf0e6a5d, /// 0xf40
			 0xbadcc4e6, /// 0xf44
			 0x110a3638, /// 0xf48
			 0x576595ef, /// 0xf4c
			 0xc63e6075, /// 0xf50
			 0x47662c98, /// 0xf54
			 0x116b9d3d, /// 0xf58
			 0x9c98d818, /// 0xf5c
			 0x98f86889, /// 0xf60
			 0xf8434fd2, /// 0xf64
			 0x5f8fee95, /// 0xf68
			 0x1f403602, /// 0xf6c
			 0xe4b6df89, /// 0xf70
			 0xe2db7586, /// 0xf74
			 0xc3169caa, /// 0xf78
			 0x6728b20e, /// 0xf7c
			 0x03addcc6, /// 0xf80
			 0xf62e9a7e, /// 0xf84
			 0x1fcb63fe, /// 0xf88
			 0x0071e462, /// 0xf8c
			 0xf86d32bd, /// 0xf90
			 0xca3a5c3a, /// 0xf94
			 0x552df952, /// 0xf98
			 0x7254f297, /// 0xf9c
			 0xcb6dd365, /// 0xfa0
			 0xd1a70b1c, /// 0xfa4
			 0xe1d3fa75, /// 0xfa8
			 0xd89af46f, /// 0xfac
			 0xf68f2080, /// 0xfb0
			 0x60fb064b, /// 0xfb4
			 0x14de5905, /// 0xfb8
			 0x24368dd4, /// 0xfbc
			 0x930d9a5e, /// 0xfc0
			 0x285e39f0, /// 0xfc4
			 0x312f83cd, /// 0xfc8
			 0x82b2ef3b, /// 0xfcc
			 0x886e1d85, /// 0xfd0
			 0x7d93beb0, /// 0xfd4
			 0xef875345, /// 0xfd8
			 0x71a0e010, /// 0xfdc
			 0xf5e0edaa, /// 0xfe0
			 0x8e692be1, /// 0xfe4
			 0x937db768, /// 0xfe8
			 0x7dcbb683, /// 0xfec
			 0x54cd6ee8, /// 0xff0
			 0x337f22c6, /// 0xff4
			 0xaff3a932, /// 0xff8
			 0xdd23a06d /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0xa3cdf745, /// 0x0
			 0xfaf56289, /// 0x4
			 0x1e779971, /// 0x8
			 0xb56169eb, /// 0xc
			 0xab05ba2e, /// 0x10
			 0xf72e4020, /// 0x14
			 0xc470f1a0, /// 0x18
			 0x8035b675, /// 0x1c
			 0x42ff5249, /// 0x20
			 0x676f413f, /// 0x24
			 0xaf6f53aa, /// 0x28
			 0xcd00971a, /// 0x2c
			 0x6ef7f156, /// 0x30
			 0x0f2f6031, /// 0x34
			 0xa7b74e52, /// 0x38
			 0xd17ba032, /// 0x3c
			 0x5af029d9, /// 0x40
			 0xeca0f94a, /// 0x44
			 0x37a45d57, /// 0x48
			 0x33c78e7a, /// 0x4c
			 0xdcf8fa6e, /// 0x50
			 0xa57a33bb, /// 0x54
			 0x936a6aa5, /// 0x58
			 0x5c6999ff, /// 0x5c
			 0x4957ab3c, /// 0x60
			 0x81f58b54, /// 0x64
			 0x7669466a, /// 0x68
			 0xadc4cc2c, /// 0x6c
			 0xbb08e2d5, /// 0x70
			 0xbd94f12a, /// 0x74
			 0xb918eb30, /// 0x78
			 0x905649e7, /// 0x7c
			 0xdf4ee4e3, /// 0x80
			 0x6c545a70, /// 0x84
			 0x40ece1ea, /// 0x88
			 0x40782f93, /// 0x8c
			 0x02a26e40, /// 0x90
			 0xd98be6e0, /// 0x94
			 0xded98136, /// 0x98
			 0x96f05a63, /// 0x9c
			 0xfd3d04d2, /// 0xa0
			 0x8fd1733d, /// 0xa4
			 0x6bc77401, /// 0xa8
			 0x2d0a74be, /// 0xac
			 0x7ab8ac98, /// 0xb0
			 0xc3e16113, /// 0xb4
			 0xfb83605b, /// 0xb8
			 0xefa4b29d, /// 0xbc
			 0xd91af361, /// 0xc0
			 0xc722303f, /// 0xc4
			 0xd65469ee, /// 0xc8
			 0xcc771c2c, /// 0xcc
			 0xc68833cb, /// 0xd0
			 0x1bb90740, /// 0xd4
			 0x4a76e685, /// 0xd8
			 0xdaaf59b0, /// 0xdc
			 0x8c2c6813, /// 0xe0
			 0xeb689ac5, /// 0xe4
			 0x92435410, /// 0xe8
			 0xf9a4baf0, /// 0xec
			 0xb062a91b, /// 0xf0
			 0xfa60d959, /// 0xf4
			 0x91924b32, /// 0xf8
			 0x620df6a8, /// 0xfc
			 0x64d44f79, /// 0x100
			 0x5897458e, /// 0x104
			 0x1a1106ef, /// 0x108
			 0x3b064cb9, /// 0x10c
			 0xa0f4dc8f, /// 0x110
			 0xb99abe85, /// 0x114
			 0xdb939131, /// 0x118
			 0xe2cf02e1, /// 0x11c
			 0xecbb1ac6, /// 0x120
			 0x6e6bca2a, /// 0x124
			 0x30977f94, /// 0x128
			 0x63bf2b5d, /// 0x12c
			 0xe1660e51, /// 0x130
			 0x715cbaf2, /// 0x134
			 0xdd5ab658, /// 0x138
			 0xdfcfd525, /// 0x13c
			 0x139e74fc, /// 0x140
			 0xd0025e14, /// 0x144
			 0x52f8967e, /// 0x148
			 0xe0b8ced4, /// 0x14c
			 0xc04518fc, /// 0x150
			 0x5a9250b7, /// 0x154
			 0x340b4dfb, /// 0x158
			 0x2f4c74b6, /// 0x15c
			 0xaed835c0, /// 0x160
			 0x023c090b, /// 0x164
			 0xd571c938, /// 0x168
			 0x63869319, /// 0x16c
			 0x3dab935f, /// 0x170
			 0x8ca44ed1, /// 0x174
			 0x79b3d973, /// 0x178
			 0x6300f4cc, /// 0x17c
			 0xb9ef70a0, /// 0x180
			 0x1250d9f7, /// 0x184
			 0x0ad7b702, /// 0x188
			 0x1549ca65, /// 0x18c
			 0x9ac1fa1c, /// 0x190
			 0x7e845254, /// 0x194
			 0x7e9441f2, /// 0x198
			 0xa2737215, /// 0x19c
			 0xe2391f0d, /// 0x1a0
			 0xcbb1e6e4, /// 0x1a4
			 0x01ca3d61, /// 0x1a8
			 0x43c75c70, /// 0x1ac
			 0x95014669, /// 0x1b0
			 0x0f304f05, /// 0x1b4
			 0xed8bdeeb, /// 0x1b8
			 0xfb4bac69, /// 0x1bc
			 0xa1059a2e, /// 0x1c0
			 0x03398e30, /// 0x1c4
			 0x24ab115e, /// 0x1c8
			 0xaa66623c, /// 0x1cc
			 0x45ec6b23, /// 0x1d0
			 0xe2f3d58d, /// 0x1d4
			 0x12b5d850, /// 0x1d8
			 0x9abd96ee, /// 0x1dc
			 0x8443cf01, /// 0x1e0
			 0x9c66af6c, /// 0x1e4
			 0x9c7d6c78, /// 0x1e8
			 0xf3dd6cb5, /// 0x1ec
			 0x9411f72b, /// 0x1f0
			 0x0c5fc400, /// 0x1f4
			 0x90ed54f2, /// 0x1f8
			 0x675ce0bb, /// 0x1fc
			 0xa12683c6, /// 0x200
			 0x4fb7f62b, /// 0x204
			 0x3b8fb31a, /// 0x208
			 0xaec3a066, /// 0x20c
			 0xf86e1f28, /// 0x210
			 0x6d71ba20, /// 0x214
			 0x52c0ef0c, /// 0x218
			 0x4ef72daa, /// 0x21c
			 0xd8ca59c6, /// 0x220
			 0x85789f43, /// 0x224
			 0xc286363f, /// 0x228
			 0xf1ee91c5, /// 0x22c
			 0xa8954929, /// 0x230
			 0x143ff7e5, /// 0x234
			 0xde42b50c, /// 0x238
			 0xeb8be576, /// 0x23c
			 0xb4b0e343, /// 0x240
			 0x43c4b505, /// 0x244
			 0xef2ae71d, /// 0x248
			 0xa3490871, /// 0x24c
			 0xbaf4e9da, /// 0x250
			 0x66a866e9, /// 0x254
			 0xa90a58e0, /// 0x258
			 0xae6f5a30, /// 0x25c
			 0x733844d6, /// 0x260
			 0xe1098c1b, /// 0x264
			 0x6a594d45, /// 0x268
			 0xfe4ec8e5, /// 0x26c
			 0xf513289a, /// 0x270
			 0x0228ef49, /// 0x274
			 0x2c4e00bb, /// 0x278
			 0x77ed6b7e, /// 0x27c
			 0x84cbc275, /// 0x280
			 0x04a19105, /// 0x284
			 0xf78df3a1, /// 0x288
			 0x0d5cd782, /// 0x28c
			 0xc2b157af, /// 0x290
			 0x761fa474, /// 0x294
			 0x3ef62b78, /// 0x298
			 0xb3c1c88c, /// 0x29c
			 0x320ed664, /// 0x2a0
			 0xafe2d35f, /// 0x2a4
			 0x31f25491, /// 0x2a8
			 0x4d43be7e, /// 0x2ac
			 0x8755914b, /// 0x2b0
			 0xac47f6ba, /// 0x2b4
			 0x196c18e8, /// 0x2b8
			 0x03c6c7ab, /// 0x2bc
			 0x8565af8e, /// 0x2c0
			 0xe145ce32, /// 0x2c4
			 0xffb7c9aa, /// 0x2c8
			 0xd28b13ce, /// 0x2cc
			 0x8a55f15a, /// 0x2d0
			 0x6f462d52, /// 0x2d4
			 0xa1c375af, /// 0x2d8
			 0xc5bb8dba, /// 0x2dc
			 0x55974139, /// 0x2e0
			 0x09be1b02, /// 0x2e4
			 0x37cdd915, /// 0x2e8
			 0x28275968, /// 0x2ec
			 0x18afecc1, /// 0x2f0
			 0x3118c7dd, /// 0x2f4
			 0x3debd411, /// 0x2f8
			 0xca5aab9e, /// 0x2fc
			 0x919d6f25, /// 0x300
			 0xf1df70cb, /// 0x304
			 0x9966f8ba, /// 0x308
			 0x5ae2ad85, /// 0x30c
			 0xd5abfb60, /// 0x310
			 0xd78affa1, /// 0x314
			 0x596521f6, /// 0x318
			 0xd5b856ba, /// 0x31c
			 0x768b3d91, /// 0x320
			 0xf8a16adf, /// 0x324
			 0xe9486c1e, /// 0x328
			 0xbac7a44d, /// 0x32c
			 0x9350e7c4, /// 0x330
			 0xdd82ca17, /// 0x334
			 0x5bbe58ee, /// 0x338
			 0xb5c81171, /// 0x33c
			 0xb01a5ac7, /// 0x340
			 0x0c466997, /// 0x344
			 0x76d6f01c, /// 0x348
			 0x7c391f61, /// 0x34c
			 0xfee8b8e5, /// 0x350
			 0x302c04e8, /// 0x354
			 0x71d05630, /// 0x358
			 0x95f2216c, /// 0x35c
			 0xd5176fc8, /// 0x360
			 0x92eaa0fb, /// 0x364
			 0x6d9456ad, /// 0x368
			 0x4fed1c2c, /// 0x36c
			 0x74440dc8, /// 0x370
			 0x448c9dd3, /// 0x374
			 0x0fdb827b, /// 0x378
			 0x5401cfe7, /// 0x37c
			 0xb11f88e3, /// 0x380
			 0x0f123db6, /// 0x384
			 0x8750b193, /// 0x388
			 0x84e6afb4, /// 0x38c
			 0x01f5d35e, /// 0x390
			 0xb1e7f62c, /// 0x394
			 0x324d7ab0, /// 0x398
			 0x55a584b5, /// 0x39c
			 0x5fbfa236, /// 0x3a0
			 0xc4f4e5a0, /// 0x3a4
			 0x3332527d, /// 0x3a8
			 0x1e89f9c1, /// 0x3ac
			 0xadc9c210, /// 0x3b0
			 0xabc0de90, /// 0x3b4
			 0x7d573543, /// 0x3b8
			 0x55e5c2f3, /// 0x3bc
			 0xe647ead9, /// 0x3c0
			 0xf00cff8a, /// 0x3c4
			 0x665c539f, /// 0x3c8
			 0xeae54800, /// 0x3cc
			 0x2ee0e41b, /// 0x3d0
			 0xd81d251b, /// 0x3d4
			 0x3b57663f, /// 0x3d8
			 0xa8d9c297, /// 0x3dc
			 0x82f7fac2, /// 0x3e0
			 0x876eee10, /// 0x3e4
			 0xf23cb75b, /// 0x3e8
			 0x7fb55dfd, /// 0x3ec
			 0xab4dfaaf, /// 0x3f0
			 0x007092a5, /// 0x3f4
			 0x4e37ef76, /// 0x3f8
			 0x060e5ccd, /// 0x3fc
			 0x338c2888, /// 0x400
			 0xa6b3f1be, /// 0x404
			 0x01f57d20, /// 0x408
			 0x8b1ba85a, /// 0x40c
			 0x55814eff, /// 0x410
			 0x38a68537, /// 0x414
			 0xf91d963a, /// 0x418
			 0x92da5767, /// 0x41c
			 0xb3ec2242, /// 0x420
			 0xf123ebea, /// 0x424
			 0x96f12268, /// 0x428
			 0x708c6a8d, /// 0x42c
			 0xffb6a034, /// 0x430
			 0x0b5da738, /// 0x434
			 0x7175960a, /// 0x438
			 0x9d190cfa, /// 0x43c
			 0x149deda8, /// 0x440
			 0x3f88b6b6, /// 0x444
			 0xaba85063, /// 0x448
			 0xa96cb19d, /// 0x44c
			 0x33ace5c9, /// 0x450
			 0x0b223449, /// 0x454
			 0x1ccd34af, /// 0x458
			 0xfab239d8, /// 0x45c
			 0xd9258693, /// 0x460
			 0x85ad3fa4, /// 0x464
			 0x0b64d73a, /// 0x468
			 0x98239414, /// 0x46c
			 0xf07bdcf4, /// 0x470
			 0x81783ffa, /// 0x474
			 0x52457fde, /// 0x478
			 0xaca88567, /// 0x47c
			 0xa8675ed6, /// 0x480
			 0xc302f4d7, /// 0x484
			 0x076a74f8, /// 0x488
			 0x6ca72bf9, /// 0x48c
			 0xa0a84f03, /// 0x490
			 0x44e9b116, /// 0x494
			 0xdc63fb33, /// 0x498
			 0xb181e823, /// 0x49c
			 0xd50e0ff6, /// 0x4a0
			 0xd46e824e, /// 0x4a4
			 0x1fcd00fd, /// 0x4a8
			 0xe1cda8ef, /// 0x4ac
			 0xd6fb786e, /// 0x4b0
			 0x2434d861, /// 0x4b4
			 0x485d3b14, /// 0x4b8
			 0xf60868ce, /// 0x4bc
			 0x0c12c431, /// 0x4c0
			 0xebb00d11, /// 0x4c4
			 0x700e9e0d, /// 0x4c8
			 0xffcfc044, /// 0x4cc
			 0x23b863ca, /// 0x4d0
			 0x8c8b9f95, /// 0x4d4
			 0x92970f03, /// 0x4d8
			 0x6cedf826, /// 0x4dc
			 0xffeb72f1, /// 0x4e0
			 0x619aee90, /// 0x4e4
			 0x530ced6c, /// 0x4e8
			 0x0cfd3943, /// 0x4ec
			 0x2655fa96, /// 0x4f0
			 0xcf62e59a, /// 0x4f4
			 0xd06bbce1, /// 0x4f8
			 0x232c3ce3, /// 0x4fc
			 0xa115622f, /// 0x500
			 0xb670520e, /// 0x504
			 0x8385d950, /// 0x508
			 0x2e786b56, /// 0x50c
			 0xefae0c1a, /// 0x510
			 0x405d2ab7, /// 0x514
			 0xc5947fee, /// 0x518
			 0x7115d09e, /// 0x51c
			 0x522ad871, /// 0x520
			 0x30ccdc02, /// 0x524
			 0x763a126e, /// 0x528
			 0xe98d9802, /// 0x52c
			 0xfdf7607f, /// 0x530
			 0x470b77e8, /// 0x534
			 0x7894c84d, /// 0x538
			 0x2c66b8a7, /// 0x53c
			 0x8a664331, /// 0x540
			 0xff5e3e61, /// 0x544
			 0xe734ec10, /// 0x548
			 0x31325362, /// 0x54c
			 0x5564f999, /// 0x550
			 0xfe6b3a56, /// 0x554
			 0xe9d18ac1, /// 0x558
			 0x4e9c4f25, /// 0x55c
			 0xd7c3a77d, /// 0x560
			 0xb81707ff, /// 0x564
			 0x9811a9a5, /// 0x568
			 0x430b3692, /// 0x56c
			 0x5e624fbe, /// 0x570
			 0x31effae4, /// 0x574
			 0x520d5de1, /// 0x578
			 0x03ee9e08, /// 0x57c
			 0xf92cb7c2, /// 0x580
			 0xe80ee9b3, /// 0x584
			 0x305c5786, /// 0x588
			 0xb0129474, /// 0x58c
			 0xe97c1af2, /// 0x590
			 0xb5b07768, /// 0x594
			 0x59dc38a5, /// 0x598
			 0xf05472b1, /// 0x59c
			 0xd2707cab, /// 0x5a0
			 0xad904423, /// 0x5a4
			 0xc73f207f, /// 0x5a8
			 0x4d033006, /// 0x5ac
			 0xdf491ea9, /// 0x5b0
			 0xf529133a, /// 0x5b4
			 0x4da9e844, /// 0x5b8
			 0xffd4b7fb, /// 0x5bc
			 0x0398b70c, /// 0x5c0
			 0x7783623d, /// 0x5c4
			 0x4d23b334, /// 0x5c8
			 0x449bf4e9, /// 0x5cc
			 0xcf6eb518, /// 0x5d0
			 0x38677e15, /// 0x5d4
			 0x93d8af46, /// 0x5d8
			 0xd1bc7b50, /// 0x5dc
			 0xa782275d, /// 0x5e0
			 0x4004ef4e, /// 0x5e4
			 0xcc49b106, /// 0x5e8
			 0x040f083e, /// 0x5ec
			 0x656f50d1, /// 0x5f0
			 0xb7d337ca, /// 0x5f4
			 0x7aee62b9, /// 0x5f8
			 0xf196731b, /// 0x5fc
			 0x191c42c3, /// 0x600
			 0xa32034ac, /// 0x604
			 0x4b1b5a5b, /// 0x608
			 0x032f3d5d, /// 0x60c
			 0x62ae4af7, /// 0x610
			 0xc1563385, /// 0x614
			 0x674d4703, /// 0x618
			 0x2f658b14, /// 0x61c
			 0xf62c301c, /// 0x620
			 0xd84ed64c, /// 0x624
			 0x37db2e24, /// 0x628
			 0x20fb9cce, /// 0x62c
			 0x410e3ef9, /// 0x630
			 0x268fde82, /// 0x634
			 0x028423d5, /// 0x638
			 0xadfa9c7c, /// 0x63c
			 0x70c7a157, /// 0x640
			 0x4ca40202, /// 0x644
			 0x35059852, /// 0x648
			 0xa21df14c, /// 0x64c
			 0x1e16ed71, /// 0x650
			 0x55367520, /// 0x654
			 0x6cf9854a, /// 0x658
			 0x2ac28d0d, /// 0x65c
			 0x1372c18a, /// 0x660
			 0x91462eaf, /// 0x664
			 0x517be13d, /// 0x668
			 0x3f11a9f2, /// 0x66c
			 0x07bd7f79, /// 0x670
			 0x9427689e, /// 0x674
			 0xfb91ddd9, /// 0x678
			 0x26a479ea, /// 0x67c
			 0x34bf5ef6, /// 0x680
			 0xdd01fe43, /// 0x684
			 0x129ed36c, /// 0x688
			 0x9925a8a9, /// 0x68c
			 0x39590b3d, /// 0x690
			 0xeac0c4d8, /// 0x694
			 0x0f8c8ab4, /// 0x698
			 0x16e85daf, /// 0x69c
			 0x87911bc9, /// 0x6a0
			 0xf3b9e568, /// 0x6a4
			 0x333899b9, /// 0x6a8
			 0xb4e558ec, /// 0x6ac
			 0xc9606228, /// 0x6b0
			 0x8d08c90b, /// 0x6b4
			 0xae5c23d5, /// 0x6b8
			 0x59490151, /// 0x6bc
			 0x4423c252, /// 0x6c0
			 0x46e652e6, /// 0x6c4
			 0x403e7026, /// 0x6c8
			 0x0e5b6ec4, /// 0x6cc
			 0x298b4223, /// 0x6d0
			 0xeb85b100, /// 0x6d4
			 0x1ae6f735, /// 0x6d8
			 0x6b6260d1, /// 0x6dc
			 0x41399320, /// 0x6e0
			 0xbd31e01a, /// 0x6e4
			 0x44771058, /// 0x6e8
			 0x85adfcb1, /// 0x6ec
			 0x85a19e86, /// 0x6f0
			 0xec06a4ce, /// 0x6f4
			 0x8bda9262, /// 0x6f8
			 0xa7e438e0, /// 0x6fc
			 0x475f2ff5, /// 0x700
			 0x0d2fc276, /// 0x704
			 0x99735d7e, /// 0x708
			 0x695aea68, /// 0x70c
			 0x693c023c, /// 0x710
			 0x368cf550, /// 0x714
			 0xdf074ed5, /// 0x718
			 0x8f8e5ad9, /// 0x71c
			 0x64642249, /// 0x720
			 0x7ed4e7c8, /// 0x724
			 0x63c7b351, /// 0x728
			 0x56cd7b5f, /// 0x72c
			 0x40b772db, /// 0x730
			 0xe46ed77d, /// 0x734
			 0x873dafa9, /// 0x738
			 0x471e2b19, /// 0x73c
			 0xca94efec, /// 0x740
			 0x8648266c, /// 0x744
			 0x82031d78, /// 0x748
			 0xbda74859, /// 0x74c
			 0x55416ae2, /// 0x750
			 0xdf4fcda3, /// 0x754
			 0xcec149c8, /// 0x758
			 0x58582001, /// 0x75c
			 0x2c916b87, /// 0x760
			 0x364e83f9, /// 0x764
			 0x9ce0a7c2, /// 0x768
			 0xd86e5c20, /// 0x76c
			 0x1ce79b21, /// 0x770
			 0x193593c8, /// 0x774
			 0x4418c4e8, /// 0x778
			 0x6762256f, /// 0x77c
			 0x8af53948, /// 0x780
			 0x3c4ced84, /// 0x784
			 0x6d4beff9, /// 0x788
			 0x472747f5, /// 0x78c
			 0x262d9015, /// 0x790
			 0xd1436ef4, /// 0x794
			 0x81fad621, /// 0x798
			 0x145da266, /// 0x79c
			 0x71e17fd5, /// 0x7a0
			 0x9029e759, /// 0x7a4
			 0x65599f05, /// 0x7a8
			 0x93f388f1, /// 0x7ac
			 0x6b774232, /// 0x7b0
			 0xf14be30f, /// 0x7b4
			 0xa7330452, /// 0x7b8
			 0x76224d85, /// 0x7bc
			 0x21b62b2b, /// 0x7c0
			 0x579085a2, /// 0x7c4
			 0xa08a10d0, /// 0x7c8
			 0xa80976b0, /// 0x7cc
			 0x9c3a2622, /// 0x7d0
			 0xe8f6e9d4, /// 0x7d4
			 0xa6c04fa2, /// 0x7d8
			 0xfbdc5780, /// 0x7dc
			 0x3e2afb06, /// 0x7e0
			 0x0a910607, /// 0x7e4
			 0xf330e9ec, /// 0x7e8
			 0x7cf27968, /// 0x7ec
			 0x38f48915, /// 0x7f0
			 0x706be934, /// 0x7f4
			 0x43ba9aa6, /// 0x7f8
			 0xfaedfc94, /// 0x7fc
			 0xa27d1e2e, /// 0x800
			 0x780ec17d, /// 0x804
			 0x519a9f54, /// 0x808
			 0xdf5b21c1, /// 0x80c
			 0xbb725c5f, /// 0x810
			 0xa7aabe1b, /// 0x814
			 0x4b5999fc, /// 0x818
			 0xe1f1e071, /// 0x81c
			 0x032d5008, /// 0x820
			 0xd7c21c70, /// 0x824
			 0x1437c65b, /// 0x828
			 0x56f0440e, /// 0x82c
			 0x8d2526a8, /// 0x830
			 0x17ef323e, /// 0x834
			 0x28eff977, /// 0x838
			 0x7d4b1a23, /// 0x83c
			 0xa45f6661, /// 0x840
			 0x77ed4e54, /// 0x844
			 0x07c1e700, /// 0x848
			 0xd31e342c, /// 0x84c
			 0xf8534683, /// 0x850
			 0x26541279, /// 0x854
			 0x85ac5b27, /// 0x858
			 0x0e7650d3, /// 0x85c
			 0x4d4b7026, /// 0x860
			 0x78609ef5, /// 0x864
			 0xc0c1110c, /// 0x868
			 0xd0c27371, /// 0x86c
			 0xaa6d7f44, /// 0x870
			 0xcf47ba05, /// 0x874
			 0x5f2271a3, /// 0x878
			 0x90f93d55, /// 0x87c
			 0x3b3a015f, /// 0x880
			 0xf523ed42, /// 0x884
			 0xf387f523, /// 0x888
			 0xf4335cac, /// 0x88c
			 0xd9070428, /// 0x890
			 0x16e3fabc, /// 0x894
			 0xa9d4a4ec, /// 0x898
			 0x0d9c17ac, /// 0x89c
			 0x7a17d3a8, /// 0x8a0
			 0xd5786eb4, /// 0x8a4
			 0x951931ba, /// 0x8a8
			 0xfc62aecd, /// 0x8ac
			 0x359c771d, /// 0x8b0
			 0x206bbf2e, /// 0x8b4
			 0xceac702f, /// 0x8b8
			 0x0a553736, /// 0x8bc
			 0x45fdee02, /// 0x8c0
			 0x5f2e9efc, /// 0x8c4
			 0x331a9675, /// 0x8c8
			 0xca00ece2, /// 0x8cc
			 0x1fdb1338, /// 0x8d0
			 0x5e61087a, /// 0x8d4
			 0x417a3423, /// 0x8d8
			 0x480ed336, /// 0x8dc
			 0x4ae96536, /// 0x8e0
			 0x7e1372ed, /// 0x8e4
			 0x116be6c0, /// 0x8e8
			 0x11aac447, /// 0x8ec
			 0xc3420185, /// 0x8f0
			 0xa2d95848, /// 0x8f4
			 0x2cf23ca9, /// 0x8f8
			 0xde0c4678, /// 0x8fc
			 0xa4ea51ee, /// 0x900
			 0x6943d6a8, /// 0x904
			 0x85b8347a, /// 0x908
			 0xd2d16717, /// 0x90c
			 0x8f6b3231, /// 0x910
			 0x661f4f7d, /// 0x914
			 0x611eee79, /// 0x918
			 0x91bc15ee, /// 0x91c
			 0x49efa1b6, /// 0x920
			 0xbff1a864, /// 0x924
			 0x81acfa9a, /// 0x928
			 0xf37f49c6, /// 0x92c
			 0x46b10a57, /// 0x930
			 0x494dbcc4, /// 0x934
			 0x17916a44, /// 0x938
			 0x91603afd, /// 0x93c
			 0x6ee351db, /// 0x940
			 0x1eb39a41, /// 0x944
			 0x9cc70916, /// 0x948
			 0xc9fb11b9, /// 0x94c
			 0xe25c82ab, /// 0x950
			 0x1586a90b, /// 0x954
			 0xd1b4df43, /// 0x958
			 0xa104899c, /// 0x95c
			 0xf369dcda, /// 0x960
			 0x676b3003, /// 0x964
			 0x7e01a96e, /// 0x968
			 0x737eff33, /// 0x96c
			 0xff03e282, /// 0x970
			 0x74878ce4, /// 0x974
			 0xcb61a240, /// 0x978
			 0xfc89e364, /// 0x97c
			 0xc7b97909, /// 0x980
			 0x04d964a5, /// 0x984
			 0x31c42829, /// 0x988
			 0x264221fa, /// 0x98c
			 0xfec43af7, /// 0x990
			 0x99811c1b, /// 0x994
			 0x0cafacc7, /// 0x998
			 0x23d94df9, /// 0x99c
			 0xc82ae10a, /// 0x9a0
			 0x862a0590, /// 0x9a4
			 0x104cfe0d, /// 0x9a8
			 0x7c5a6011, /// 0x9ac
			 0x3e135eba, /// 0x9b0
			 0x5f1bed88, /// 0x9b4
			 0x7e771697, /// 0x9b8
			 0x026696d1, /// 0x9bc
			 0x2b3e0fc8, /// 0x9c0
			 0x6374381e, /// 0x9c4
			 0x1fae5269, /// 0x9c8
			 0x1a23d58a, /// 0x9cc
			 0x110ec0b1, /// 0x9d0
			 0xa4cfbc85, /// 0x9d4
			 0x72718983, /// 0x9d8
			 0x96530c17, /// 0x9dc
			 0xb8a83f31, /// 0x9e0
			 0x0981f430, /// 0x9e4
			 0xdf6b4d56, /// 0x9e8
			 0x4d51c687, /// 0x9ec
			 0xc4f035d1, /// 0x9f0
			 0x18f0844b, /// 0x9f4
			 0xcb89ae61, /// 0x9f8
			 0xbfffb347, /// 0x9fc
			 0x4956dd08, /// 0xa00
			 0x0ddebcc6, /// 0xa04
			 0xfbce6c94, /// 0xa08
			 0x846aa592, /// 0xa0c
			 0xe9f73719, /// 0xa10
			 0x75ed9493, /// 0xa14
			 0x5c60c0fc, /// 0xa18
			 0x74a0082a, /// 0xa1c
			 0x4b26fc61, /// 0xa20
			 0xb02e9931, /// 0xa24
			 0x00d58ab8, /// 0xa28
			 0xd34947fe, /// 0xa2c
			 0xd274c05c, /// 0xa30
			 0x132b349f, /// 0xa34
			 0x41e7603e, /// 0xa38
			 0x86e4ac8a, /// 0xa3c
			 0xac03a225, /// 0xa40
			 0xf061aa0f, /// 0xa44
			 0x73b09dbc, /// 0xa48
			 0xa8ce8538, /// 0xa4c
			 0xa24d7376, /// 0xa50
			 0xc7b1847b, /// 0xa54
			 0x4f33102e, /// 0xa58
			 0x54f321ba, /// 0xa5c
			 0xbf69b127, /// 0xa60
			 0x8305ab6a, /// 0xa64
			 0x0fca260f, /// 0xa68
			 0xe8717d0f, /// 0xa6c
			 0xfc51bd17, /// 0xa70
			 0x7624eb3a, /// 0xa74
			 0xdf5d08f3, /// 0xa78
			 0x75248617, /// 0xa7c
			 0xb4e7c3e0, /// 0xa80
			 0x6aac31c5, /// 0xa84
			 0xea636988, /// 0xa88
			 0x2cf4b1ed, /// 0xa8c
			 0xade5de65, /// 0xa90
			 0x5912457f, /// 0xa94
			 0xaf93d894, /// 0xa98
			 0xb51356b4, /// 0xa9c
			 0x0d136398, /// 0xaa0
			 0x316e412a, /// 0xaa4
			 0xae2ebe33, /// 0xaa8
			 0x25f0287e, /// 0xaac
			 0x75107139, /// 0xab0
			 0x9eece19a, /// 0xab4
			 0x92d75001, /// 0xab8
			 0x5217b044, /// 0xabc
			 0x19e73486, /// 0xac0
			 0x56cce991, /// 0xac4
			 0x6ce09584, /// 0xac8
			 0xb9dc6aa6, /// 0xacc
			 0xdcc1d82d, /// 0xad0
			 0xb073df83, /// 0xad4
			 0x7d3b9a7e, /// 0xad8
			 0xd9a227ca, /// 0xadc
			 0xbab44710, /// 0xae0
			 0x159dadda, /// 0xae4
			 0x5f3a7b79, /// 0xae8
			 0xa472e8de, /// 0xaec
			 0x4ffa25df, /// 0xaf0
			 0x69936342, /// 0xaf4
			 0x548e4832, /// 0xaf8
			 0x0bfed5db, /// 0xafc
			 0xeacc4b47, /// 0xb00
			 0x8f1ad0a9, /// 0xb04
			 0x2a3a0c22, /// 0xb08
			 0xc7fdd25f, /// 0xb0c
			 0x5d28ef4e, /// 0xb10
			 0x6735e144, /// 0xb14
			 0x4ec7fc62, /// 0xb18
			 0x56edbb9e, /// 0xb1c
			 0xfe580496, /// 0xb20
			 0xebb0aa17, /// 0xb24
			 0x76346310, /// 0xb28
			 0x048f05cc, /// 0xb2c
			 0x3051d0e6, /// 0xb30
			 0x4a688d2f, /// 0xb34
			 0x041bc79a, /// 0xb38
			 0xb9ae170e, /// 0xb3c
			 0x024c6ca8, /// 0xb40
			 0x4361beac, /// 0xb44
			 0x1053f8fa, /// 0xb48
			 0x836862d9, /// 0xb4c
			 0x693ad4f6, /// 0xb50
			 0x50511e79, /// 0xb54
			 0x73554e46, /// 0xb58
			 0x28db6e68, /// 0xb5c
			 0x975258b0, /// 0xb60
			 0xba3a193c, /// 0xb64
			 0x3df7982e, /// 0xb68
			 0x4974d706, /// 0xb6c
			 0x9ae5b819, /// 0xb70
			 0xf31b3e85, /// 0xb74
			 0x39958a24, /// 0xb78
			 0xe3061104, /// 0xb7c
			 0xedc4b042, /// 0xb80
			 0x00885d03, /// 0xb84
			 0x63a8318b, /// 0xb88
			 0x35aac847, /// 0xb8c
			 0x839acaf1, /// 0xb90
			 0x7e9d2157, /// 0xb94
			 0x926875eb, /// 0xb98
			 0xf721db03, /// 0xb9c
			 0x864884a7, /// 0xba0
			 0x47450b4f, /// 0xba4
			 0xbbd84ec1, /// 0xba8
			 0xc3f19577, /// 0xbac
			 0x2d24ff95, /// 0xbb0
			 0xffd8ab83, /// 0xbb4
			 0x7e228a2a, /// 0xbb8
			 0x4e707726, /// 0xbbc
			 0x49655fc9, /// 0xbc0
			 0xf78f30ad, /// 0xbc4
			 0x2e5e9ed3, /// 0xbc8
			 0x43d91ddf, /// 0xbcc
			 0xb7f6895b, /// 0xbd0
			 0xb63f42f2, /// 0xbd4
			 0x47689395, /// 0xbd8
			 0x38ebd760, /// 0xbdc
			 0xb53a614c, /// 0xbe0
			 0xeac70621, /// 0xbe4
			 0x20b0c185, /// 0xbe8
			 0x0faffde3, /// 0xbec
			 0x2385ecc1, /// 0xbf0
			 0xba40d105, /// 0xbf4
			 0x25ac35bc, /// 0xbf8
			 0x42efa99c, /// 0xbfc
			 0xdaafdcbb, /// 0xc00
			 0xd2b76765, /// 0xc04
			 0x55476a58, /// 0xc08
			 0x8c4f389f, /// 0xc0c
			 0x8f645d8b, /// 0xc10
			 0x37a8695c, /// 0xc14
			 0x4f082d0b, /// 0xc18
			 0xc5933677, /// 0xc1c
			 0xf56c8a05, /// 0xc20
			 0xdb60e167, /// 0xc24
			 0xc66c4ebf, /// 0xc28
			 0x8dcd684a, /// 0xc2c
			 0x43426033, /// 0xc30
			 0x7cb1f13f, /// 0xc34
			 0xac60f4aa, /// 0xc38
			 0x2b5ba8ad, /// 0xc3c
			 0x50925581, /// 0xc40
			 0x9a33dd38, /// 0xc44
			 0xd2d7046e, /// 0xc48
			 0xc80d5d07, /// 0xc4c
			 0xb08681fe, /// 0xc50
			 0x4c7793c7, /// 0xc54
			 0x57173c00, /// 0xc58
			 0xa4b822b1, /// 0xc5c
			 0x396f4ab4, /// 0xc60
			 0x469593f8, /// 0xc64
			 0xba987d50, /// 0xc68
			 0x662968b9, /// 0xc6c
			 0xa9cdcc02, /// 0xc70
			 0x947302db, /// 0xc74
			 0xf9e28671, /// 0xc78
			 0xf5814bc7, /// 0xc7c
			 0xf8eb888d, /// 0xc80
			 0x680acd72, /// 0xc84
			 0xb6e271d4, /// 0xc88
			 0x2358de13, /// 0xc8c
			 0x66e79bb9, /// 0xc90
			 0xe5d9cde6, /// 0xc94
			 0xf913bb68, /// 0xc98
			 0x6b5238f0, /// 0xc9c
			 0xca804a9b, /// 0xca0
			 0x69c568c9, /// 0xca4
			 0x2650d259, /// 0xca8
			 0xfb6c4002, /// 0xcac
			 0x4cb225f6, /// 0xcb0
			 0x8883dd1d, /// 0xcb4
			 0x7ec6c090, /// 0xcb8
			 0xa6d03bac, /// 0xcbc
			 0x6de1780e, /// 0xcc0
			 0x0f7ef6ff, /// 0xcc4
			 0xf5508ed5, /// 0xcc8
			 0x59414864, /// 0xccc
			 0x547c8c12, /// 0xcd0
			 0x2b049c7c, /// 0xcd4
			 0x123d1466, /// 0xcd8
			 0x8dc7bb73, /// 0xcdc
			 0x1f7c8a71, /// 0xce0
			 0xaf73747a, /// 0xce4
			 0x3ee8630e, /// 0xce8
			 0xbab41809, /// 0xcec
			 0x9613489f, /// 0xcf0
			 0x160a8980, /// 0xcf4
			 0x80136269, /// 0xcf8
			 0x2d50ed50, /// 0xcfc
			 0xc362fdeb, /// 0xd00
			 0xe11bec80, /// 0xd04
			 0xa50f679d, /// 0xd08
			 0x46364734, /// 0xd0c
			 0x47deb9a8, /// 0xd10
			 0x05a8a082, /// 0xd14
			 0xf050645c, /// 0xd18
			 0x0eef7a74, /// 0xd1c
			 0xcfaeac91, /// 0xd20
			 0x4c42fcaa, /// 0xd24
			 0x468428bf, /// 0xd28
			 0xcba7d466, /// 0xd2c
			 0xf8ce8d7c, /// 0xd30
			 0x14ac71b5, /// 0xd34
			 0x5d43e713, /// 0xd38
			 0x8258713b, /// 0xd3c
			 0x0de9f7da, /// 0xd40
			 0x06b74172, /// 0xd44
			 0x5220fa11, /// 0xd48
			 0x33af9cf3, /// 0xd4c
			 0xca71c0f5, /// 0xd50
			 0xb3ff10ca, /// 0xd54
			 0x64bf847e, /// 0xd58
			 0x5672dd01, /// 0xd5c
			 0xd2975ae3, /// 0xd60
			 0x2cbd3597, /// 0xd64
			 0x9840bde0, /// 0xd68
			 0x5f6efa2e, /// 0xd6c
			 0x23c025f0, /// 0xd70
			 0x3ec148ea, /// 0xd74
			 0x5962ea1d, /// 0xd78
			 0x65f40672, /// 0xd7c
			 0xb822b631, /// 0xd80
			 0x7797ba26, /// 0xd84
			 0x1caaf2fb, /// 0xd88
			 0xa8642326, /// 0xd8c
			 0x5886bc54, /// 0xd90
			 0xef139252, /// 0xd94
			 0x13c82d70, /// 0xd98
			 0xd037340a, /// 0xd9c
			 0xa91f3d52, /// 0xda0
			 0x21fc9b79, /// 0xda4
			 0x539ab52c, /// 0xda8
			 0x769925d7, /// 0xdac
			 0xf0f66cad, /// 0xdb0
			 0x52cc0432, /// 0xdb4
			 0x56c39794, /// 0xdb8
			 0xa9ea2233, /// 0xdbc
			 0xd48cb556, /// 0xdc0
			 0x9b6aca4b, /// 0xdc4
			 0x35111c4b, /// 0xdc8
			 0x59b8b577, /// 0xdcc
			 0x8ad2aad5, /// 0xdd0
			 0x4402a2c3, /// 0xdd4
			 0x3bb0f18a, /// 0xdd8
			 0x1a692b62, /// 0xddc
			 0x6e1d3bd9, /// 0xde0
			 0x8b7e5f32, /// 0xde4
			 0xb6083f86, /// 0xde8
			 0x45194c94, /// 0xdec
			 0x22525293, /// 0xdf0
			 0xea1d57d3, /// 0xdf4
			 0x8c7cb59e, /// 0xdf8
			 0x27e58a77, /// 0xdfc
			 0x6e5c4b86, /// 0xe00
			 0x04f8adb6, /// 0xe04
			 0x4fcb3b3f, /// 0xe08
			 0xc0862990, /// 0xe0c
			 0x6d9b9c5e, /// 0xe10
			 0xecfa6eb6, /// 0xe14
			 0x4843e658, /// 0xe18
			 0xacb43f30, /// 0xe1c
			 0x51dbef36, /// 0xe20
			 0xa1b7081f, /// 0xe24
			 0xc50dbe01, /// 0xe28
			 0x14a8374c, /// 0xe2c
			 0x20d103e0, /// 0xe30
			 0x9ae9a8ee, /// 0xe34
			 0x47cdc2cd, /// 0xe38
			 0x79b4be85, /// 0xe3c
			 0xb38e0b51, /// 0xe40
			 0xf54ad546, /// 0xe44
			 0x9b1c1e47, /// 0xe48
			 0xb7fad2ff, /// 0xe4c
			 0x8ff80dc0, /// 0xe50
			 0x1e26547d, /// 0xe54
			 0xb04f3990, /// 0xe58
			 0x38b230f1, /// 0xe5c
			 0xeed04f2d, /// 0xe60
			 0xa2a22e87, /// 0xe64
			 0x7d4c5866, /// 0xe68
			 0x64c97686, /// 0xe6c
			 0xb0136ff3, /// 0xe70
			 0x879135a0, /// 0xe74
			 0xec537c1c, /// 0xe78
			 0x5d9bebd2, /// 0xe7c
			 0xedc9d592, /// 0xe80
			 0x6709e1a6, /// 0xe84
			 0x09ee6200, /// 0xe88
			 0x8055226f, /// 0xe8c
			 0x80d52c47, /// 0xe90
			 0xa453a67d, /// 0xe94
			 0x4f42423a, /// 0xe98
			 0x7ffb4a8b, /// 0xe9c
			 0x1d11dae0, /// 0xea0
			 0x4d7ae0b6, /// 0xea4
			 0xab6603a5, /// 0xea8
			 0xeabe14a1, /// 0xeac
			 0x987c32c4, /// 0xeb0
			 0xf17835f2, /// 0xeb4
			 0x004b7ba9, /// 0xeb8
			 0x620b2bdc, /// 0xebc
			 0xa21e9cb3, /// 0xec0
			 0x4907cd5f, /// 0xec4
			 0xb97f5532, /// 0xec8
			 0x60c7e567, /// 0xecc
			 0x5b2ffaa9, /// 0xed0
			 0x2d307949, /// 0xed4
			 0xbaad170b, /// 0xed8
			 0x357ae3b9, /// 0xedc
			 0x7418c973, /// 0xee0
			 0x11f8bc08, /// 0xee4
			 0x54c26909, /// 0xee8
			 0x5144db43, /// 0xeec
			 0xc66f1049, /// 0xef0
			 0xee19b281, /// 0xef4
			 0x2950b105, /// 0xef8
			 0x6bc74fb7, /// 0xefc
			 0x60431260, /// 0xf00
			 0x9fa4f4f6, /// 0xf04
			 0xaa2ca089, /// 0xf08
			 0x37c341b7, /// 0xf0c
			 0x2f495f91, /// 0xf10
			 0x8e58a051, /// 0xf14
			 0x95a13b43, /// 0xf18
			 0x42ef0d1b, /// 0xf1c
			 0x26b6870e, /// 0xf20
			 0x43b947c6, /// 0xf24
			 0x7f1b55a2, /// 0xf28
			 0x7e4e2a6d, /// 0xf2c
			 0xfcf11229, /// 0xf30
			 0x80959c33, /// 0xf34
			 0x46779e6f, /// 0xf38
			 0x019d9952, /// 0xf3c
			 0x2d032f56, /// 0xf40
			 0xde014802, /// 0xf44
			 0xa0295874, /// 0xf48
			 0x41b84eec, /// 0xf4c
			 0xff5cdef6, /// 0xf50
			 0xe943ae35, /// 0xf54
			 0xf742589d, /// 0xf58
			 0x02a8887f, /// 0xf5c
			 0x640b0504, /// 0xf60
			 0x582ce47b, /// 0xf64
			 0x18b8d084, /// 0xf68
			 0x053bae2c, /// 0xf6c
			 0x24549555, /// 0xf70
			 0x1a0506ed, /// 0xf74
			 0xce46eb9a, /// 0xf78
			 0x7e3df7d9, /// 0xf7c
			 0xe67423cd, /// 0xf80
			 0x7b396ca3, /// 0xf84
			 0xf897a51c, /// 0xf88
			 0x50c679cf, /// 0xf8c
			 0xd1d0e19b, /// 0xf90
			 0x74efe4af, /// 0xf94
			 0x7e1761b5, /// 0xf98
			 0x0b9340c4, /// 0xf9c
			 0xa98b9c2a, /// 0xfa0
			 0x3dfbe779, /// 0xfa4
			 0x32375233, /// 0xfa8
			 0x44ff8cfe, /// 0xfac
			 0x4c244991, /// 0xfb0
			 0x1ed38205, /// 0xfb4
			 0x64ea96ea, /// 0xfb8
			 0x9fa51792, /// 0xfbc
			 0x5591c613, /// 0xfc0
			 0xd3c2223b, /// 0xfc4
			 0x944e3e5c, /// 0xfc8
			 0x63ddde72, /// 0xfcc
			 0x01d3c989, /// 0xfd0
			 0xc0e74042, /// 0xfd4
			 0xb9ecc916, /// 0xfd8
			 0x6e6d4a7f, /// 0xfdc
			 0x3ea65d3e, /// 0xfe0
			 0x065d7ad6, /// 0xfe4
			 0xac314ab8, /// 0xfe8
			 0xb267d11b, /// 0xfec
			 0x6e537c7a, /// 0xff0
			 0x0ec54f47, /// 0xff4
			 0x7cddcb9d, /// 0xff8
			 0x87ff9105 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x0e000001,                                                  // Trailing 1s:                                /// 00004
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00010
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00014
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00018
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0001c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00020
			 0x33333333,                                                  // 4 random values                             /// 00024
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00028
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0002c
			 0x55555555,                                                  // 4 random values                             /// 00030
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00034
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00038
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0003c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00040
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00044
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0004c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00050
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00054
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00058
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0005c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00060
			 0x80011111,                                                  // -9.7958E-41                                 /// 00064
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00068
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0006c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00070
			 0x33333333,                                                  // 4 random values                             /// 00074
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00078
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00080
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00084
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00088
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0008c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00090
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00094
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00098
			 0x0c780000,                                                  // Leading 1s:                                 /// 0009c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000a0
			 0x80000000,                                                  // -zero                                       /// 000a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000a8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000ac
			 0xbf028f5c,                                                  // -0.51                                       /// 000b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000c4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000dc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000f8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00100
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0010c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00110
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00118
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0011c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00120
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00124
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00128
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0012c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x0c780000,                                                  // Leading 1s:                                 /// 00134
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00138
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0013c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00140
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x4b000000,                                                  // 8388608.0                                   /// 00148
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0014c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00150
			 0xff800000,                                                  // -inf                                        /// 00154
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00158
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0015c
			 0xffc00001,                                                  // -signaling NaN                              /// 00160
			 0x80000000,                                                  // -zero                                       /// 00164
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00168
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0016c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00174
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00178
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0017c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00188
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0018c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00190
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00194
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00198
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0019c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001a0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0xffc00001,                                                  // -signaling NaN                              /// 001a8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001ac
			 0x3f028f5c,                                                  // 0.51                                        /// 001b0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001b4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001b8
			 0x55555555,                                                  // 4 random values                             /// 001bc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001c0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001c4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001c8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001cc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001d0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001d4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001e0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x80000000,                                                  // -zero                                       /// 001e8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001ec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001f0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001f4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00200
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00204
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00208
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0020c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00210
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00218
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0021c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00220
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00224
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00228
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0022c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00230
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00234
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0023c
			 0x80000000,                                                  // -zero                                       /// 00240
			 0xffc00001,                                                  // -signaling NaN                              /// 00244
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00248
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0024c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00250
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00254
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00258
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0025c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00260
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00264
			 0x55555555,                                                  // 4 random values                             /// 00268
			 0x33333333,                                                  // 4 random values                             /// 0026c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00270
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00274
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00278
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0027c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00280
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00284
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00288
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0028c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00294
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00298
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0029c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002a0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002a4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002a8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002ac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002b0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002b4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002b8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002c0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002c4
			 0x55555555,                                                  // 4 random values                             /// 002c8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002cc
			 0x0c600000,                                                  // Leading 1s:                                 /// 002d0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002e0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002e4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002ec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002f0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002f8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002fc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00300
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00304
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00308
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0030c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00310
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00314
			 0x0e000003,                                                  // Trailing 1s:                                /// 00318
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0031c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00320
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00328
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0032c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00330
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0033c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00340
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00344
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00348
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00358
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0035c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00364
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0036c
			 0x4b000000,                                                  // 8388608.0                                   /// 00370
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00374
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00378
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0037c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00380
			 0x0e000001,                                                  // Trailing 1s:                                /// 00384
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0038c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00398
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0039c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003a0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003a4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003ac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003b0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003b4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003b8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003bc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003c0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003c4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003d4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x0e000007,                                                  // Trailing 1s:                                /// 003dc
			 0x0c700000,                                                  // Leading 1s:                                 /// 003e0
			 0xffc00001,                                                  // -signaling NaN                              /// 003e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003e8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 003f0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003f4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003f8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003fc
			 0x3f028f5c,                                                  // 0.51                                        /// 00400
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x3f028f5c,                                                  // 0.51                                        /// 00408
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00414
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0041c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00420
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00424
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00428
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0042c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00430
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00434
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0043c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00448
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0044c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00450
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00458
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0045c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00460
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00468
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00470
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00474
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00478
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0047c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00480
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00484
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00488
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0048c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00490
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00494
			 0x0c700000,                                                  // Leading 1s:                                 /// 00498
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0049c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004b8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004c8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 004cc
			 0x33333333,                                                  // 4 random values                             /// 004d0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004d4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004e0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004e4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004f0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004f8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004fc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00504
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00508
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0050c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00510
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00518
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0051c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00520
			 0x80011111,                                                  // -9.7958E-41                                 /// 00524
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00528
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0052c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0xff800000,                                                  // -inf                                        /// 00534
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00538
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0053c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00540
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00548
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0054c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00550
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00554
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00558
			 0x0c780000,                                                  // Leading 1s:                                 /// 0055c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00560
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00568
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0056c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00570
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00574
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00578
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00580
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00584
			 0x0c400000,                                                  // Leading 1s:                                 /// 00588
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0058c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x4b000000,                                                  // 8388608.0                                   /// 00594
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00598
			 0x0c400000,                                                  // Leading 1s:                                 /// 0059c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005a8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005ac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005b0
			 0x0e000007,                                                  // Trailing 1s:                                /// 005b4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005b8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005bc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 005c8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005d0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005d4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005d8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005dc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005e0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005e4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005ec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 005f0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005f8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005fc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00600
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00604
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00608
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0060c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00610
			 0x00011111,                                                  // 9.7958E-41                                  /// 00614
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00618
			 0x0c780000,                                                  // Leading 1s:                                 /// 0061c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00620
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x0c600000,                                                  // Leading 1s:                                 /// 00628
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0062c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00634
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00640
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00644
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00648
			 0xff800000,                                                  // -inf                                        /// 0064c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00650
			 0xffc00001,                                                  // -signaling NaN                              /// 00654
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00658
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0065c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00660
			 0x0c600000,                                                  // Leading 1s:                                 /// 00664
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00668
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x7fc00001,                                                  // signaling NaN                               /// 00670
			 0x00000000,                                                  // zero                                        /// 00674
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00678
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00680
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00690
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00694
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0069c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006a4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006a8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006b0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006b4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006b8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 006bc
			 0x7fc00001,                                                  // signaling NaN                               /// 006c0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006d0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006d4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006d8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006e0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006e4
			 0x0c780000,                                                  // Leading 1s:                                 /// 006e8
			 0x55555555,                                                  // 4 random values                             /// 006ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006f0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006fc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00700
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00704
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0070c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00710
			 0x0c400000,                                                  // Leading 1s:                                 /// 00714
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00718
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00720
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00724
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00728
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x0e000003,                                                  // Trailing 1s:                                /// 00734
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00738
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0073c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00740
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00744
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00748
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0074c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00750
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0075c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00764
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00768
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0076c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00774
			 0x0c400000,                                                  // Leading 1s:                                 /// 00778
			 0x0e000007,                                                  // Trailing 1s:                                /// 0077c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00780
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00788
			 0x0e000003,                                                  // Trailing 1s:                                /// 0078c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00790
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00794
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00798
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x0e000003,                                                  // Trailing 1s:                                /// 007a0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007a8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007b4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007b8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007c0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007d0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007dc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007e4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 007e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007ec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007f0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007f8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007fc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00800
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00804
			 0x00011111,                                                  // 9.7958E-41                                  /// 00808
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0080c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00814
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00818
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0081c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00824
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0082c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00830
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00838
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0083c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00844
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0084c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00850
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00854
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x7fc00001,                                                  // signaling NaN                               /// 0085c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00860
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0086c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00870
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00878
			 0x80011111,                                                  // -9.7958E-41                                 /// 0087c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00880
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00884
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00888
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0088c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00890
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00894
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00898
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0089c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008a4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008a8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008ac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008b0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008bc
			 0x33333333,                                                  // 4 random values                             /// 008c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008c8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008e0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008e8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008ec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008f0
			 0xcb000000,                                                  // -8388608.0                                  /// 008f4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008f8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00900
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00904
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00908
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0090c
			 0x33333333,                                                  // 4 random values                             /// 00910
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00914
			 0x0e000007,                                                  // Trailing 1s:                                /// 00918
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0091c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00920
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00924
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00928
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00934
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00938
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0093c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00940
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00948
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0094c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00950
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00958
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0095c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00968
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00970
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00974
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00978
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0097c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00980
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00984
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00988
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0098c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00990
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x7f800000,                                                  // inf                                         /// 0099c
			 0xcb000000,                                                  // -8388608.0                                  /// 009a0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009a4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009a8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009b0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009b4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x0e000007,                                                  // Trailing 1s:                                /// 009c0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009c4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 009cc
			 0x33333333,                                                  // 4 random values                             /// 009d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009d4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009dc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009e8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009f0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a08
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a0c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a10
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a14
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a18
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a1c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a20
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a24
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00a28
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a30
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a34
			 0x55555555,                                                  // 4 random values                             /// 00a38
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a44
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a4c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a50
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a54
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a5c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a60
			 0x00000000,                                                  // zero                                        /// 00a64
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a68
			 0x00000000,                                                  // zero                                        /// 00a6c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a74
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a7c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a80
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a98
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a9c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00aa0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00aa4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00aa8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00aac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ab0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ab4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ab8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00abc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ac0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ac8
			 0x55555555,                                                  // 4 random values                             /// 00acc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ad4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ad8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ae8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00aec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00af0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00af4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00af8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00afc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b04
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b08
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b0c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b14
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b18
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b20
			 0x33333333,                                                  // 4 random values                             /// 00b24
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b28
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b2c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b30
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b38
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b3c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b40
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b44
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b4c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b50
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b54
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b58
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b60
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b64
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b6c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b70
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b74
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b80
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b84
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b90
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b94
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b98
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ba8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bb0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bb4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bbc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bc0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bc4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bcc
			 0x33333333,                                                  // 4 random values                             /// 00bd0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bd8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00be0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00be4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00be8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x7f800000,                                                  // inf                                         /// 00bf0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00bfc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c00
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c0c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c10
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c14
			 0x80000000,                                                  // -zero                                       /// 00c18
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c1c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c20
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c24
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c2c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c30
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c38
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c3c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c40
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c44
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c48
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c4c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c50
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c54
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c58
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c5c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c60
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c64
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c68
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c6c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c70
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c74
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c78
			 0xbf028f5c,                                                  // -0.51                                       /// 00c7c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c80
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c84
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c88
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c8c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c90
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c98
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c9c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ca4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ca8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cb0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cb4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cb8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cbc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cc0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cc4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cc8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cd0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cd4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cd8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cdc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ce0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ce4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ce8
			 0x33333333,                                                  // 4 random values                             /// 00cec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cf0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cf4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cf8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d00
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d04
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d0c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d10
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d14
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d1c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d20
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d24
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d28
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d30
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d34
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d38
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d40
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d44
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d48
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d4c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d50
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d54
			 0xcb000000,                                                  // -8388608.0                                  /// 00d58
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d5c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d60
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d64
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d68
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d6c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d70
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d78
			 0x7fc00001,                                                  // signaling NaN                               /// 00d7c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d84
			 0x3f028f5c,                                                  // 0.51                                        /// 00d88
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d8c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d90
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d94
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d98
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d9c
			 0xff800000,                                                  // -inf                                        /// 00da0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00da4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00da8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00dac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00db0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00db4
			 0xbf028f5c,                                                  // -0.51                                       /// 00db8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00dbc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00dc0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00dc4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00dcc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00dd4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00dd8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00de0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00de4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00de8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00dec
			 0x0c400000,                                                  // Leading 1s:                                 /// 00df0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00df4
			 0x7f800000,                                                  // inf                                         /// 00df8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dfc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e00
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e04
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e08
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e10
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e14
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e20
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e24
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e28
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e2c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e30
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e38
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e3c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e40
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e44
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e48
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e50
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e5c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e60
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e68
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e6c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e70
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e74
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e78
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e7c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e84
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e88
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e90
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e94
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ea4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ea8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ebc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ec8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ecc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ed0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ed4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ed8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00edc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ee4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ef0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ef4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ef8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00efc
			 0x55555555,                                                  // 4 random values                             /// 00f00
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f08
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f10
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f14
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f18
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f1c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f20
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f24
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f28
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f2c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f30
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f34
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f38
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f3c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f44
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f48
			 0xff800000,                                                  // -inf                                        /// 00f4c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f50
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f54
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f58
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f5c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f60
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f64
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f68
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f6c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f70
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f74
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f78
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f7c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f80
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f84
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f88
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f8c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f98
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fa0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fa4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fa8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fb4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00fb8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fbc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fc0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fc4
			 0x33333333,                                                  // 4 random values                             /// 00fc8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fcc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fd0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fd4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fd8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fdc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fe0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fe4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fe8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ff4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ff8
			 0x80000040                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00000
			 0xff800000,                                                  // -inf                                        /// 00004
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0000c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00014
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00018
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0001c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00020
			 0x00011111,                                                  // 9.7958E-41                                  /// 00024
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00028
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0002c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x3f028f5c,                                                  // 0.51                                        /// 00034
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00038
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0003c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00040
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00044
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00048
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0004c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00050
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00058
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0005c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00060
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00064
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00068
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00070
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00074
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00078
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00080
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00084
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00088
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0008c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00094
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00098
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0009c
			 0x0e000007,                                                  // Trailing 1s:                                /// 000a0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000a4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000a8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000b4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000b8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000bc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000cc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000d4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000d8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000dc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000e4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000e8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000ec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000f0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000f4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000f8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00100
			 0xcb000000,                                                  // -8388608.0                                  /// 00104
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00108
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0010c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x7f800000,                                                  // inf                                         /// 00118
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0011c
			 0xffc00001,                                                  // -signaling NaN                              /// 00120
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00128
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00134
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00138
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00140
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00144
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00148
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0014c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00150
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00154
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00158
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0015c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00160
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00168
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00170
			 0x0e000007,                                                  // Trailing 1s:                                /// 00174
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0017c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0018c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x0c780000,                                                  // Leading 1s:                                 /// 00194
			 0xffc00001,                                                  // -signaling NaN                              /// 00198
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0019c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001a0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001a8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001ac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x55555555,                                                  // 4 random values                             /// 001b4
			 0x7fc00001,                                                  // signaling NaN                               /// 001b8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001bc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001c0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001c8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001d0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001d4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001d8
			 0x4b000000,                                                  // 8388608.0                                   /// 001dc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x0c700000,                                                  // Leading 1s:                                 /// 001f4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001fc
			 0x4b000000,                                                  // 8388608.0                                   /// 00200
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00204
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00208
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0020c
			 0x80000000,                                                  // -zero                                       /// 00210
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00214
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00218
			 0x3f028f5c,                                                  // 0.51                                        /// 0021c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00220
			 0x7f800000,                                                  // inf                                         /// 00224
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00228
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00230
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00234
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0023c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00240
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00244
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00248
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00250
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00254
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00258
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0025c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00260
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00264
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00268
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0xcb000000,                                                  // -8388608.0                                  /// 00270
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00278
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0027c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00280
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00284
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00288
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x00000000,                                                  // zero                                        /// 00290
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00294
			 0x55555555,                                                  // 4 random values                             /// 00298
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0029c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002a0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002a4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002a8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002ac
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002b4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002b8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002c4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002c8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002d0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002d8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002dc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002ec
			 0x0e00000f,                                                  // Trailing 1s:                                /// 002f0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002f4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00300
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00304
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00308
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0030c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00314
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00318
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0031c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00320
			 0x0c700000,                                                  // Leading 1s:                                 /// 00324
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00328
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00330
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0033c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00340
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00344
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0034c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00350
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00354
			 0x7f800000,                                                  // inf                                         /// 00358
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0035c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00360
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00364
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00368
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0036c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00370
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00374
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00378
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0037c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00380
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00384
			 0x0e000007,                                                  // Trailing 1s:                                /// 00388
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0038c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00398
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0039c
			 0x0c780000,                                                  // Leading 1s:                                 /// 003a0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003a4
			 0x80000000,                                                  // -zero                                       /// 003a8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003b0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003b8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003bc
			 0x7f800000,                                                  // inf                                         /// 003c0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003c4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003c8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003cc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 003d4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003d8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003dc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003e4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003e8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x33333333,                                                  // 4 random values                             /// 003f0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003f4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003f8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00400
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00404
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0040c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00410
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00414
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00418
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0041c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00420
			 0x80000000,                                                  // -zero                                       /// 00424
			 0x0e000003,                                                  // Trailing 1s:                                /// 00428
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0042c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00430
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00434
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0043c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00440
			 0x80000000,                                                  // -zero                                       /// 00444
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00448
			 0x7fc00001,                                                  // signaling NaN                               /// 0044c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00450
			 0xcb000000,                                                  // -8388608.0                                  /// 00454
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00458
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0045c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00460
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0046c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00470
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00474
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00478
			 0x7fc00001,                                                  // signaling NaN                               /// 0047c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00480
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00484
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00488
			 0x0e000003,                                                  // Trailing 1s:                                /// 0048c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00490
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00494
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00498
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004a4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004a8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004ac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004b8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004bc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004c0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004c4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004d4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004d8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004e0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0xffc00001,                                                  // -signaling NaN                              /// 004e8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004ec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004f0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004f8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00500
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00504
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00508
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0050c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00510
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00514
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00518
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00520
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00524
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00528
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0052c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00530
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0053c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00540
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00548
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0054c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00558
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0055c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00560
			 0x4b000000,                                                  // 8388608.0                                   /// 00564
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00568
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0056c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00574
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00578
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0057c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00580
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00584
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00588
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00590
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00594
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00598
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005a4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005a8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005b0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005b4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005bc
			 0x3f028f5c,                                                  // 0.51                                        /// 005c0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005c4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005c8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005d4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005dc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005e0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005e4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005f8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00600
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00608
			 0x00000000,                                                  // zero                                        /// 0060c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00610
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x33333333,                                                  // 4 random values                             /// 00618
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0061c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00624
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00628
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0062c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00630
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00634
			 0x7fc00001,                                                  // signaling NaN                               /// 00638
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0063c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00640
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00644
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00648
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00650
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00658
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0065c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x3f028f5c,                                                  // 0.51                                        /// 00664
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00668
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0066c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00670
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00674
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00678
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0067c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00680
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00688
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0068c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00690
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00694
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00698
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006a0
			 0x4b000000,                                                  // 8388608.0                                   /// 006a4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006a8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006ac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006b4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006b8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006c0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006c4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006c8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 006cc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006d0
			 0x4b000000,                                                  // 8388608.0                                   /// 006d4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006d8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006dc
			 0x00000000,                                                  // zero                                        /// 006e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006e4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006e8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006f0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006f4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006f8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00700
			 0x0c400000,                                                  // Leading 1s:                                 /// 00704
			 0x80000000,                                                  // -zero                                       /// 00708
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0070c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00710
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00714
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0xbf028f5c,                                                  // -0.51                                       /// 0071c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00720
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00724
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00728
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0072c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00730
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00738
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00740
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00748
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0074c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00750
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00758
			 0xcb000000,                                                  // -8388608.0                                  /// 0075c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00764
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0076c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00770
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00774
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0077c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00780
			 0x0c700000,                                                  // Leading 1s:                                 /// 00784
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00788
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0078c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00794
			 0x4b000000,                                                  // 8388608.0                                   /// 00798
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0079c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007a0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007a8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007ac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007b0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007b4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007b8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007bc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007c0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007c8
			 0x0e000001,                                                  // Trailing 1s:                                /// 007cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007d0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007d4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007d8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007dc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007e0
			 0x4b000000,                                                  // 8388608.0                                   /// 007e4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 007f0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007f4
			 0x00000000,                                                  // zero                                        /// 007f8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007fc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00800
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00804
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00808
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0080c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00810
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00814
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00818
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0081c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00820
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00824
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00834
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00838
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0083c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00840
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00844
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00848
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0084c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00850
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00854
			 0x80011111,                                                  // -9.7958E-41                                 /// 00858
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0085c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00860
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00868
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0086c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00874
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00878
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0087c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00880
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00884
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00888
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0088c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00890
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00894
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0089c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008a4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008a8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008b8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008bc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008c8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008cc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008ec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008f0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008f4
			 0xffc00001,                                                  // -signaling NaN                              /// 008f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008fc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00904
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00908
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0090c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00910
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00914
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00918
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00920
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00924
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00928
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00930
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00934
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00938
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0093c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00940
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00944
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00948
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0094c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00950
			 0x7fc00001,                                                  // signaling NaN                               /// 00954
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00958
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0095c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00964
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00968
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00970
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00974
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00978
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0097c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00980
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00984
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00988
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0098c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00990
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00994
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x0c700000,                                                  // Leading 1s:                                 /// 0099c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009a0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009a4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009a8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009b4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009b8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009bc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009c0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x0e000007,                                                  // Trailing 1s:                                /// 009c8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009cc
			 0x80000000,                                                  // -zero                                       /// 009d0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009dc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009e4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009f4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009f8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a00
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a04
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a08
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a0c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a10
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a14
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a18
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a1c
			 0x33333333,                                                  // 4 random values                             /// 00a20
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a24
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a28
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a2c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a30
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a34
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a38
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a3c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a40
			 0xcb000000,                                                  // -8388608.0                                  /// 00a44
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a50
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a54
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a5c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a60
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a64
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a68
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a6c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a74
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a78
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a7c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a80
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a84
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a8c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a90
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a94
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a98
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a9c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00aa8
			 0x00000000,                                                  // zero                                        /// 00aac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ab0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00ab4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ab8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00abc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ac0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ac4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ad4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ad8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00adc
			 0xbf028f5c,                                                  // -0.51                                       /// 00ae0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ae4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00aec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00af0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00af4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00afc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b04
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b08
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b0c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b10
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b14
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b18
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b1c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b20
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b2c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b30
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b34
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b3c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x55555555,                                                  // 4 random values                             /// 00b50
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b54
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b58
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b60
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b64
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b74
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b80
			 0x55555555,                                                  // 4 random values                             /// 00b84
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b88
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b8c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b90
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b98
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b9c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ba0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ba8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bb0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bb8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00bc8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00bcc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bd0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bd4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bd8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bdc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00be4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00be8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bec
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bf0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bf4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bf8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00bfc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c00
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c04
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c08
			 0xffc00001,                                                  // -signaling NaN                              /// 00c0c
			 0x55555555,                                                  // 4 random values                             /// 00c10
			 0xffc00001,                                                  // -signaling NaN                              /// 00c14
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c18
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c1c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c20
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c24
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c28
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c2c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c30
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c34
			 0xbf028f5c,                                                  // -0.51                                       /// 00c38
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c40
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c44
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c48
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c4c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c50
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c54
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c58
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c5c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c60
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c64
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c6c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c70
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c74
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c7c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c84
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c88
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c8c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c94
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c98
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c9c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ca4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ca8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cb0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00cb4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cb8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00cbc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cc0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cc8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cd4
			 0xbf028f5c,                                                  // -0.51                                       /// 00cd8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00cdc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ce0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ce4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ce8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cec
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00cf8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cfc
			 0x7f800000,                                                  // inf                                         /// 00d00
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d04
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d08
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d0c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d14
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d18
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d1c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d24
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d28
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d2c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d30
			 0xcb000000,                                                  // -8388608.0                                  /// 00d34
			 0xffc00001,                                                  // -signaling NaN                              /// 00d38
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d3c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d40
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d44
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d48
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d4c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d50
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d54
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d58
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d5c
			 0x3f028f5c,                                                  // 0.51                                        /// 00d60
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d68
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d6c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d70
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d74
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d78
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d7c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d80
			 0x80000000,                                                  // -zero                                       /// 00d84
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d88
			 0x55555555,                                                  // 4 random values                             /// 00d8c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d94
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d98
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d9c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00da0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00da4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00da8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00db4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00db8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dbc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00dc0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00dc4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00dcc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00dd4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00dd8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00de0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x00000000,                                                  // zero                                        /// 00de8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00df0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00df4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00df8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00dfc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e00
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e04
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e08
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00e0c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e10
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e14
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e18
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e1c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e24
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e28
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e2c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e30
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e34
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e38
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e3c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e40
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e48
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e50
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e54
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e58
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e5c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e60
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e64
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e68
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e6c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x4b000000,                                                  // 8388608.0                                   /// 00e74
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e80
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e84
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e88
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e8c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e98
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e9c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ea0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00eac
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00eb4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00eb8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ebc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ec0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ec4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ec8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ecc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ed0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ed4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ed8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00edc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ee0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ee4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ee8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00eec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ef0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ef4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ef8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00efc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f00
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f04
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f08
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f10
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f18
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f1c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00f20
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f28
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f2c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f34
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f38
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f3c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f40
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f44
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f48
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f4c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f50
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f5c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f64
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x80000000,                                                  // -zero                                       /// 00f6c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f70
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0xffc00001,                                                  // -signaling NaN                              /// 00f78
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f80
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f84
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f8c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f90
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f94
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f98
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f9c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fa0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fa4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fa8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fb4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fb8
			 0xcb000000,                                                  // -8388608.0                                  /// 00fbc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fc0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fc8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fcc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fd0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fd4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fd8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fdc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fe0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fe4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fe8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fec
			 0x55555555,                                                  // 4 random values                             /// 00ff0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ff4
			 0x33333333,                                                  // 4 random values                             /// 00ff8
			 0x0e0007ff                                                  // Trailing 1s:                                /// last
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
			 0x000001b0,
			 0x000001c8,
			 0x000001a8,
			 0x0000017c,
			 0x000007cc,
			 0x0000032c,
			 0x0000007c,
			 0x000006b8,

			 /// vpu register f2
			 0x40800000,
			 0x41300000,
			 0x41a00000,
			 0x41d00000,
			 0x41b00000,
			 0x41500000,
			 0x41300000,
			 0x41f00000,

			 /// vpu register f3
			 0x41b80000,
			 0x41c80000,
			 0x3f800000,
			 0x40800000,
			 0x41e80000,
			 0x41980000,
			 0x41200000,
			 0x41b80000,

			 /// vpu register f4
			 0x41700000,
			 0x40800000,
			 0x41500000,
			 0x41a80000,
			 0x40e00000,
			 0x40000000,
			 0x41300000,
			 0x41500000,

			 /// vpu register f5
			 0x41f00000,
			 0x41700000,
			 0x41b00000,
			 0x41500000,
			 0x41100000,
			 0x41700000,
			 0x41b00000,
			 0x41f80000,

			 /// vpu register f6
			 0x41e00000,
			 0x41c00000,
			 0x40000000,
			 0x41a80000,
			 0x41000000,
			 0x3f800000,
			 0x41900000,
			 0x41f80000,

			 /// vpu register f7
			 0x41f00000,
			 0x41000000,
			 0x41b80000,
			 0x41a80000,
			 0x40400000,
			 0x42000000,
			 0x41980000,
			 0x3f800000,

			 /// vpu register f8
			 0x41f80000,
			 0x42000000,
			 0x41b80000,
			 0x40000000,
			 0x40c00000,
			 0x41f00000,
			 0x40e00000,
			 0x41d00000,

			 /// vpu register f9
			 0x41c80000,
			 0x41f00000,
			 0x41300000,
			 0x41800000,
			 0x41d80000,
			 0x41c00000,
			 0x40e00000,
			 0x41300000,

			 /// vpu register f10
			 0x40a00000,
			 0x41a00000,
			 0x41f80000,
			 0x41980000,
			 0x41e80000,
			 0x40c00000,
			 0x41e80000,
			 0x41c00000,

			 /// vpu register f11
			 0x41a80000,
			 0x41400000,
			 0x41900000,
			 0x41f80000,
			 0x41700000,
			 0x40800000,
			 0x41c80000,
			 0x41900000,

			 /// vpu register f12
			 0x41c80000,
			 0x42000000,
			 0x41d80000,
			 0x41f00000,
			 0x41a00000,
			 0x41980000,
			 0x41900000,
			 0x41c00000,

			 /// vpu register f13
			 0x41c80000,
			 0x41e80000,
			 0x40c00000,
			 0x41d00000,
			 0x40c00000,
			 0x41b00000,
			 0x41300000,
			 0x41e80000,

			 /// vpu register f14
			 0x41900000,
			 0x41d80000,
			 0x41a00000,
			 0x42000000,
			 0x41000000,
			 0x41880000,
			 0x41a00000,
			 0x41700000,

			 /// vpu register f15
			 0x41600000,
			 0x41c80000,
			 0x41d80000,
			 0x41c00000,
			 0x40a00000,
			 0x41600000,
			 0x41000000,
			 0x41980000,

			 /// vpu register f16
			 0x3f800000,
			 0x40c00000,
			 0x40000000,
			 0x42000000,
			 0x41b80000,
			 0x41000000,
			 0x41d80000,
			 0x40a00000,

			 /// vpu register f17
			 0x40800000,
			 0x41600000,
			 0x41980000,
			 0x40400000,
			 0x41400000,
			 0x41f00000,
			 0x40a00000,
			 0x41d00000,

			 /// vpu register f18
			 0x41300000,
			 0x40a00000,
			 0x41400000,
			 0x41880000,
			 0x41f80000,
			 0x41000000,
			 0x40000000,
			 0x42000000,

			 /// vpu register f19
			 0x41a80000,
			 0x41e00000,
			 0x41700000,
			 0x41c00000,
			 0x41980000,
			 0x40000000,
			 0x41d00000,
			 0x41800000,

			 /// vpu register f20
			 0x40a00000,
			 0x41b80000,
			 0x40c00000,
			 0x41980000,
			 0x41f00000,
			 0x41300000,
			 0x41b80000,
			 0x41a00000,

			 /// vpu register f21
			 0x41500000,
			 0x41c00000,
			 0x41f00000,
			 0x42000000,
			 0x41900000,
			 0x40a00000,
			 0x41500000,
			 0x40c00000,

			 /// vpu register f22
			 0x41100000,
			 0x41800000,
			 0x42000000,
			 0x40a00000,
			 0x40800000,
			 0x41100000,
			 0x40800000,
			 0x40c00000,

			 /// vpu register f23
			 0x41500000,
			 0x41200000,
			 0x3f800000,
			 0x41e80000,
			 0x40c00000,
			 0x41f00000,
			 0x41a80000,
			 0x41a00000,

			 /// vpu register f24
			 0x41300000,
			 0x40e00000,
			 0x42000000,
			 0x41f80000,
			 0x40c00000,
			 0x42000000,
			 0x42000000,
			 0x41e80000,

			 /// vpu register f25
			 0x40000000,
			 0x40a00000,
			 0x41c00000,
			 0x41f00000,
			 0x41880000,
			 0x40e00000,
			 0x41e00000,
			 0x41880000,

			 /// vpu register f26
			 0x41a80000,
			 0x41f00000,
			 0x41700000,
			 0x41b00000,
			 0x41d00000,
			 0x41400000,
			 0x41980000,
			 0x41500000,

			 /// vpu register f27
			 0x41980000,
			 0x41e80000,
			 0x40000000,
			 0x41a00000,
			 0x41a80000,
			 0x41500000,
			 0x41d00000,
			 0x40000000,

			 /// vpu register f28
			 0x40000000,
			 0x41800000,
			 0x41f00000,
			 0x40400000,
			 0x41700000,
			 0x41980000,
			 0x41900000,
			 0x40400000,

			 /// vpu register f29
			 0x41e00000,
			 0x40a00000,
			 0x41900000,
			 0x41980000,
			 0x41500000,
			 0x41880000,
			 0x41880000,
			 0x3f800000,

			 /// vpu register f30
			 0x42000000,
			 0x40000000,
			 0x40800000,
			 0x41800000,
			 0x41400000,
			 0x41d00000,
			 0x41600000,
			 0x41d00000,

			 /// vpu register f31
			 0x41b80000,
			 0x41100000,
			 0x40000000,
			 0x41a80000,
			 0x41980000,
			 0x41e80000,
			 0x41d00000,
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
		"fadd.pi f28, f9, f20\n"                              ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f19, f1, f14\n"                              ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f23, f5, f2\n"                               ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f9, f27, f3\n"                               ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f4, f16, f1\n"                               ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f27, f6, f16\n"                              ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f18, f1, f22\n"                              ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f24, f1, f3\n"                               ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f24, f12, f21\n"                             ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f30, f3, f7\n"                               ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f9, f26, f25\n"                              ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f19, f8, f22\n"                              ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f14, f26, f5\n"                              ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f23, f29, f26\n"                             ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f4, f24, f6\n"                               ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f21, f11, f17\n"                             ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f16, f17, f21\n"                             ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f28, f6, f3\n"                               ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f30, f12, f19\n"                             ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f27, f22, f3\n"                              ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f12, f10, f15\n"                             ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f27, f28, f0\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f19, f2, f27\n"                              ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f19, f17, f7\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f18, f23, f3\n"                              ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f22, f13, f25\n"                             ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f2, f16, f20\n"                              ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f5, f30, f13\n"                              ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f17, f15, f16\n"                             ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f13, f16, f22\n"                             ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f29, f29, f18\n"                             ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f28, f16, f18\n"                             ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f22, f0, f17\n"                              ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f23, f24, f7\n"                              ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f20, f25, f10\n"                             ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f25, f17, f10\n"                             ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f16, f29, f29\n"                             ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f11, f12, f2\n"                              ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f23, f23, f28\n"                             ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f12, f26, f19\n"                             ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f26, f9, f28\n"                              ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f23, f24, f0\n"                              ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f24, f26, f3\n"                              ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f17, f4, f21\n"                              ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f7, f25, f27\n"                              ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f3, f25, f2\n"                               ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f15, f25, f5\n"                              ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f19, f5, f25\n"                              ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f5, f27, f6\n"                               ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f13, f11, f0\n"                              ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f12, f3, f15\n"                              ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f16, f17, f10\n"                             ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f10, f22, f30\n"                             ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f21, f5, f11\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f30, f26, f6\n"                              ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f26, f22, f2\n"                              ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f7, f25, f23\n"                              ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f29, f3, f0\n"                               ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f28, f20, f4\n"                              ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f28, f6, f13\n"                              ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f2, f29, f17\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f10, f11, f4\n"                              ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f23, f27, f27\n"                             ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f22, f9, f1\n"                               ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f30, f20, f13\n"                             ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f14, f6, f2\n"                               ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f12, f6, f19\n"                              ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f9, f20, f27\n"                              ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f21, f26, f5\n"                              ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f3, f7, f18\n"                               ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f19, f18, f9\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f6, f23, f28\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f30, f22, f26\n"                             ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f7, f3, f21\n"                               ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f3, f16, f13\n"                              ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f25, f19, f27\n"                             ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f23, f28, f14\n"                             ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f2, f17, f0\n"                               ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f20, f27, f13\n"                             ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f19, f18, f5\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f19, f25, f5\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f15, f4, f25\n"                              ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f10, f0, f15\n"                              ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f8, f30, f14\n"                              ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f16, f5, f16\n"                              ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f17, f20, f22\n"                             ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f12, f26, f23\n"                             ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f24, f26, f17\n"                             ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f17, f14, f21\n"                             ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f4, f16, f29\n"                              ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f13, f9, f2\n"                               ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f21, f7, f22\n"                              ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f7, f27, f15\n"                              ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f20, f15, f13\n"                             ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f21, f9, f0\n"                               ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f21, f8, f23\n"                              ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f3, f1, f10\n"                               ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f9, f3, f1\n"                                ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fadd.pi f23, f14, f14\n"                             ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
