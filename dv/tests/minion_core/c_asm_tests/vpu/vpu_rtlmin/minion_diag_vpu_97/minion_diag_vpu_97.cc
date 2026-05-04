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
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00000
			 0xffc00001,                                                  // -signaling NaN                              /// 00004
			 0x0e000007,                                                  // Trailing 1s:                                /// 00008
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0000c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00010
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00014
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00018
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0001c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00020
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00024
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0002c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00038
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0003c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00040
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00048
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0004c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00054
			 0x00000000,                                                  // zero                                        /// 00058
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0xffc00001,                                                  // -signaling NaN                              /// 00060
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00064
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00068
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x4b000000,                                                  // 8388608.0                                   /// 00070
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00074
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00078
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0007c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00080
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00084
			 0x0e000003,                                                  // Trailing 1s:                                /// 00088
			 0x80011111,                                                  // -9.7958E-41                                 /// 0008c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00090
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00094
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00098
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0009c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000a0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000ac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 000b0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000b8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000bc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000c0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 000c8
			 0x0c780000,                                                  // Leading 1s:                                 /// 000cc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000d4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000d8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000e0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000e8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000ec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000f0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000f4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000f8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000fc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00100
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00108
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00110
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00114
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00118
			 0x00000000,                                                  // zero                                        /// 0011c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00120
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00124
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0012c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00130
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00138
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0013c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00140
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00144
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00148
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0014c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00150
			 0x33333333,                                                  // 4 random values                             /// 00154
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00158
			 0x00000000,                                                  // zero                                        /// 0015c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00160
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00164
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00168
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0016c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00170
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00174
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0017c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00184
			 0x80011111,                                                  // -9.7958E-41                                 /// 00188
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0018c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00194
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00198
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001a4
			 0xffc00001,                                                  // -signaling NaN                              /// 001a8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001b0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001bc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x55555555,                                                  // 4 random values                             /// 001c4
			 0x0c780000,                                                  // Leading 1s:                                 /// 001c8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001cc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001d8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001dc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 001e4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001ec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 001f0
			 0x7fc00001,                                                  // signaling NaN                               /// 001f4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001f8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001fc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00204
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00208
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0020c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00210
			 0xff800000,                                                  // -inf                                        /// 00214
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0021c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00220
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00224
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0022c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00238
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00240
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00244
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00248
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0024c
			 0x4b000000,                                                  // 8388608.0                                   /// 00250
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00254
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00258
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0025c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00260
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00264
			 0xffc00001,                                                  // -signaling NaN                              /// 00268
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0026c
			 0xff800000,                                                  // -inf                                        /// 00270
			 0xbf028f5c,                                                  // -0.51                                       /// 00274
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0027c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00284
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00288
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0028c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00290
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00294
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00298
			 0x0e000007,                                                  // Trailing 1s:                                /// 0029c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002a0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002a4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002ac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002b0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002b4
			 0x55555555,                                                  // 4 random values                             /// 002b8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002c4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002c8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002cc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002d4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002dc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002e0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002e4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 002e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002ec
			 0x0c780000,                                                  // Leading 1s:                                 /// 002f0
			 0x0e000007,                                                  // Trailing 1s:                                /// 002f4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00304
			 0xcb000000,                                                  // -8388608.0                                  /// 00308
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0030c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00310
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00314
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00318
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0031c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00320
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00324
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00328
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00338
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0033c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00348
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0xffc00001,                                                  // -signaling NaN                              /// 00354
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00358
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0035c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00360
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00364
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00368
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0036c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00370
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00374
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00378
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00380
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00384
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00388
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0038c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00390
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00394
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00398
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0039c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003a0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003a4
			 0xbf028f5c,                                                  // -0.51                                       /// 003a8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003b0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003b4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003b8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003bc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003c0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 003c4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003c8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003cc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003d4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 003dc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x7f800000,                                                  // inf                                         /// 003e4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003e8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003ec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003f0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003f8
			 0x3f028f5c,                                                  // 0.51                                        /// 003fc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00400
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00408
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0040c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00414
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00418
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0041c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00420
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00428
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0042c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00434
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00438
			 0x00000000,                                                  // zero                                        /// 0043c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00440
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00444
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0044c
			 0xffc00001,                                                  // -signaling NaN                              /// 00450
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00458
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0045c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00460
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00464
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00468
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00474
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00478
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0047c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00480
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00484
			 0xffc00001,                                                  // -signaling NaN                              /// 00488
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0048c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00490
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00494
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00498
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0049c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004a0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004a8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004ac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004b0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004b4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 004bc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004c4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004c8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004d0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004d8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004dc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004e0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004e8
			 0x00011111,                                                  // 9.7958E-41                                  /// 004ec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004f4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00500
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00504
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00508
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0050c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00510
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00514
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00518
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0051c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00520
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00524
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0052c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00530
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0053c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0054c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00550
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00558
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00560
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00564
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0056c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00570
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0057c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00580
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0058c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00590
			 0x4b000000,                                                  // 8388608.0                                   /// 00594
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0059c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005a0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005ac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005b8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005bc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005c0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005c8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 005cc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005d0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005d8
			 0x0c700000,                                                  // Leading 1s:                                 /// 005dc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005e0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005e4
			 0xffc00001,                                                  // -signaling NaN                              /// 005e8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005f0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 005f4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005f8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005fc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x7f800000,                                                  // inf                                         /// 00604
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00608
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00610
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00614
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00618
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0061c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00620
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00628
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00634
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00638
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0063c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00640
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00648
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0064c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00650
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00654
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00658
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00660
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00664
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00668
			 0xff800000,                                                  // -inf                                        /// 0066c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00670
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0067c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00680
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00684
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00690
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00694
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00698
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0069c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006a0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006a4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006a8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006ac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006b8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006c0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x55555555,                                                  // 4 random values                             /// 006c8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006d0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006d4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006d8
			 0x0e000003,                                                  // Trailing 1s:                                /// 006dc
			 0x00011111,                                                  // 9.7958E-41                                  /// 006e0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 006e8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006ec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006f0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006f8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00708
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0070c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0071c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0072c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00730
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00734
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00738
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00740
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00744
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0074c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00750
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00754
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0075c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00760
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00764
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00768
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0076c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00770
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00774
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00778
			 0x0e000001,                                                  // Trailing 1s:                                /// 0077c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00788
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0078c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00798
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0079c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007a4
			 0xff800000,                                                  // -inf                                        /// 007a8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007ac
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007b0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007b4
			 0x0c700000,                                                  // Leading 1s:                                 /// 007b8
			 0xff800000,                                                  // -inf                                        /// 007bc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 007c0
			 0x33333333,                                                  // 4 random values                             /// 007c4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007c8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 007cc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007d0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007d4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007dc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 007e4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007e8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007f4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 007f8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 007fc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00800
			 0xff800000,                                                  // -inf                                        /// 00804
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00808
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0080c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00810
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0081c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x55555555,                                                  // 4 random values                             /// 00824
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00828
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0082c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00830
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00834
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00840
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00848
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0084c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00850
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00854
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00858
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0085c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00860
			 0x7fc00001,                                                  // signaling NaN                               /// 00864
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x00011111,                                                  // 9.7958E-41                                  /// 0086c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00870
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00878
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00880
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00884
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00888
			 0x0e000001,                                                  // Trailing 1s:                                /// 0088c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00890
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00894
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00898
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0089c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008a0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008a4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008ac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008b0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008b4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008bc
			 0x80000000,                                                  // -zero                                       /// 008c0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008c8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008cc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008d0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008d4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008d8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008dc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 008e0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008e8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008f0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008f4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008fc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00904
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00918
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0091c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00920
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0092c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00934
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00938
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0093c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00940
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00944
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00948
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0094c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00950
			 0x80000000,                                                  // -zero                                       /// 00954
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00958
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0095c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00968
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0096c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00970
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00974
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00978
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0097c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00980
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00984
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00988
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0098c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00990
			 0x3f028f5c,                                                  // 0.51                                        /// 00994
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00998
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0099c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 009a0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009ac
			 0x0c400000,                                                  // Leading 1s:                                 /// 009b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009b4
			 0xff800000,                                                  // -inf                                        /// 009b8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009c4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009c8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009cc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009d4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009d8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009dc
			 0x7f800000,                                                  // inf                                         /// 009e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009e8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009f0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009f4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009fc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a00
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a08
			 0x80000000,                                                  // -zero                                       /// 00a0c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a10
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a18
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a24
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a34
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a38
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a40
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a44
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a48
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a50
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a54
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a5c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a64
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a68
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a6c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a74
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a78
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a7c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a80
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a84
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a88
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a8c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a90
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a94
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a98
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a9c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00aa8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00aac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ab0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ab4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ab8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ac0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ac4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00acc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ad0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00adc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ae0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ae4
			 0x7fc00001,                                                  // signaling NaN                               /// 00ae8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00af0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00afc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b04
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b08
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b0c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b10
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b18
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b1c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b20
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b24
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b30
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b34
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b38
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b3c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b40
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b44
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b50
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0xffc00001,                                                  // -signaling NaN                              /// 00b58
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b5c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b60
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b68
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00b6c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b70
			 0xffc00001,                                                  // -signaling NaN                              /// 00b74
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b7c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b80
			 0xff800000,                                                  // -inf                                        /// 00b84
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b8c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b90
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b98
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b9c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bb0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bb8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bbc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bc0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bc8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0xffc00001,                                                  // -signaling NaN                              /// 00bd0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bd4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00bd8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bdc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00be0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00be4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00be8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bec
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bf8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bfc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c00
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c04
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c0c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c10
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c14
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c18
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c1c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c20
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c24
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c28
			 0xcb000000,                                                  // -8388608.0                                  /// 00c2c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c30
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c34
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c3c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c44
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c48
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c4c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c50
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c54
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c58
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c60
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0xcb000000,                                                  // -8388608.0                                  /// 00c68
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c6c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c74
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c78
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c7c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c80
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c84
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c88
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c8c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c90
			 0x00000000,                                                  // zero                                        /// 00c94
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c98
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c9c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ca0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ca4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ca8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00cac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cb0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cb4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00cb8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cbc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00cc0
			 0x7fc00001,                                                  // signaling NaN                               /// 00cc4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cd0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cdc
			 0x7fc00001,                                                  // signaling NaN                               /// 00ce0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ce4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ce8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00cf0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cf4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cf8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cfc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d0c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d10
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d14
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d20
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d24
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d28
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d2c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d30
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d34
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d38
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d40
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d44
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d4c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d50
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d54
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d5c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d64
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d68
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d74
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d78
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d7c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d84
			 0xbf028f5c,                                                  // -0.51                                       /// 00d88
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d90
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d94
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d98
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d9c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00da0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00da4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00da8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dbc
			 0x4b000000,                                                  // 8388608.0                                   /// 00dc0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00dc8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00dd4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00dd8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ddc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00de0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00de4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00dec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00df4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00dfc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e00
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e04
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e08
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e0c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e10
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e18
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e1c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e20
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e24
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e28
			 0x55555555,                                                  // 4 random values                             /// 00e2c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e30
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e34
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e38
			 0xff800000,                                                  // -inf                                        /// 00e3c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e40
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e44
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e48
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e4c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e50
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e58
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e5c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e64
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e68
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e78
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e7c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e84
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e88
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e8c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e90
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e94
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ea0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ea4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ea8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00eac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00eb0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00eb4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ec0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ec4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ecc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ed4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ed8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00edc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ee8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00eec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ef0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ef4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ef8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00efc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f00
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f04
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f08
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f0c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f10
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f1c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f20
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f24
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f28
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f2c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f34
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f48
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f4c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f50
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f54
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f5c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f60
			 0xcb000000,                                                  // -8388608.0                                  /// 00f64
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f68
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f6c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f70
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f74
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f7c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f80
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f84
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f8c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f90
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f98
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f9c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fa0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fa4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fa8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00fb0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fb4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fb8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fbc
			 0x80000000,                                                  // -zero                                       /// 00fc0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fc4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fc8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fcc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fdc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00fe4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fe8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ff0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ff4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ff8
			 0x0c7fe000                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xc82023c0, /// 0x0
			 0x59da9255, /// 0x4
			 0x37e270a2, /// 0x8
			 0xe8c9d5cd, /// 0xc
			 0x6a332a22, /// 0x10
			 0xcbf43382, /// 0x14
			 0x86d90285, /// 0x18
			 0x8122f938, /// 0x1c
			 0xaed5e865, /// 0x20
			 0xf6176049, /// 0x24
			 0x4d84c36a, /// 0x28
			 0x571505f9, /// 0x2c
			 0xafde2d72, /// 0x30
			 0xf998af0a, /// 0x34
			 0x91b913c8, /// 0x38
			 0x2f1c0eae, /// 0x3c
			 0x63e17a2c, /// 0x40
			 0x0d71d9d5, /// 0x44
			 0x1835e0be, /// 0x48
			 0xf71f9f2d, /// 0x4c
			 0x492f05ac, /// 0x50
			 0xe06a1663, /// 0x54
			 0x4aa3b272, /// 0x58
			 0x9d2f23f8, /// 0x5c
			 0xa6d33d84, /// 0x60
			 0x35e31aa4, /// 0x64
			 0xb640a6de, /// 0x68
			 0xff8f2613, /// 0x6c
			 0x25aee852, /// 0x70
			 0xab170322, /// 0x74
			 0x28d5cc73, /// 0x78
			 0x79493b1c, /// 0x7c
			 0xc4f6a607, /// 0x80
			 0xd4888e19, /// 0x84
			 0xb465c3d5, /// 0x88
			 0xef25676e, /// 0x8c
			 0xae87121f, /// 0x90
			 0x9f280db1, /// 0x94
			 0x84094096, /// 0x98
			 0x05a27c7a, /// 0x9c
			 0x0eb3d51b, /// 0xa0
			 0xa3045ad6, /// 0xa4
			 0xea7524a9, /// 0xa8
			 0xd70324dd, /// 0xac
			 0xe948ff77, /// 0xb0
			 0x1f7455c6, /// 0xb4
			 0x938a3ef6, /// 0xb8
			 0x3fea3b45, /// 0xbc
			 0x5b9751ec, /// 0xc0
			 0x4c028fb4, /// 0xc4
			 0x77cb8538, /// 0xc8
			 0x89e76c96, /// 0xcc
			 0xdb093d2e, /// 0xd0
			 0xd16a6746, /// 0xd4
			 0x0c4d3f5c, /// 0xd8
			 0x8790a5d1, /// 0xdc
			 0xee14a554, /// 0xe0
			 0x0ff0a389, /// 0xe4
			 0x6560f4a0, /// 0xe8
			 0xeb046ffa, /// 0xec
			 0x91a03eab, /// 0xf0
			 0xf287467d, /// 0xf4
			 0x8c2241b0, /// 0xf8
			 0xc891b236, /// 0xfc
			 0xa32591ee, /// 0x100
			 0x5f7f9c91, /// 0x104
			 0xe852e90c, /// 0x108
			 0x4eb8dd0d, /// 0x10c
			 0xaaa0e6ab, /// 0x110
			 0xd6f9aeba, /// 0x114
			 0x0778026c, /// 0x118
			 0xc31f6218, /// 0x11c
			 0xe21155b1, /// 0x120
			 0x04ec2461, /// 0x124
			 0x75b00424, /// 0x128
			 0x8a373f96, /// 0x12c
			 0x6e184a61, /// 0x130
			 0xd8152a38, /// 0x134
			 0xcdef0136, /// 0x138
			 0x711f9ea1, /// 0x13c
			 0x9ec66530, /// 0x140
			 0xa504f139, /// 0x144
			 0x9c779466, /// 0x148
			 0xe6d3a572, /// 0x14c
			 0x1ff6c25e, /// 0x150
			 0x409516c8, /// 0x154
			 0x1f900773, /// 0x158
			 0x7603e11e, /// 0x15c
			 0xbdd5c88d, /// 0x160
			 0x4f287105, /// 0x164
			 0x51ee1603, /// 0x168
			 0xb91d202a, /// 0x16c
			 0x90b5e67f, /// 0x170
			 0xdd6a46a7, /// 0x174
			 0x44af4593, /// 0x178
			 0x60f42f61, /// 0x17c
			 0x69fc4fed, /// 0x180
			 0xe5aa4214, /// 0x184
			 0x7ad40a23, /// 0x188
			 0xb7f42e22, /// 0x18c
			 0x3d3c8051, /// 0x190
			 0xab002267, /// 0x194
			 0x58cd8273, /// 0x198
			 0x5ba29391, /// 0x19c
			 0x87dd1fad, /// 0x1a0
			 0xaff27a02, /// 0x1a4
			 0x2cc73ab1, /// 0x1a8
			 0xa80c29bf, /// 0x1ac
			 0xaa75590e, /// 0x1b0
			 0x694b8c66, /// 0x1b4
			 0x20f7aea8, /// 0x1b8
			 0xecbddfc8, /// 0x1bc
			 0x51113154, /// 0x1c0
			 0x1febddee, /// 0x1c4
			 0x69e21d17, /// 0x1c8
			 0xdc1d0a5b, /// 0x1cc
			 0x478058af, /// 0x1d0
			 0x21f4bbdf, /// 0x1d4
			 0xe5c812af, /// 0x1d8
			 0x00ead8be, /// 0x1dc
			 0x443c92ea, /// 0x1e0
			 0x813b3904, /// 0x1e4
			 0xf2a7fba5, /// 0x1e8
			 0xaf01894e, /// 0x1ec
			 0x3fbd1073, /// 0x1f0
			 0xb430d533, /// 0x1f4
			 0x4e20ae3d, /// 0x1f8
			 0x4e14d3ae, /// 0x1fc
			 0x8efa283a, /// 0x200
			 0x0c92f73e, /// 0x204
			 0x3e999496, /// 0x208
			 0x085867eb, /// 0x20c
			 0xa36815de, /// 0x210
			 0x3b3f002b, /// 0x214
			 0x12a89f96, /// 0x218
			 0x42b0de97, /// 0x21c
			 0xed70777f, /// 0x220
			 0xc1947543, /// 0x224
			 0xb5f77240, /// 0x228
			 0x3e25e942, /// 0x22c
			 0xe773ec08, /// 0x230
			 0x205408fb, /// 0x234
			 0xdfdbaf85, /// 0x238
			 0x495b45bc, /// 0x23c
			 0x76839107, /// 0x240
			 0x54e31d15, /// 0x244
			 0xaa5b9135, /// 0x248
			 0xe0368461, /// 0x24c
			 0xac1b811b, /// 0x250
			 0x73424aa4, /// 0x254
			 0x83ba5591, /// 0x258
			 0xd0b77ff0, /// 0x25c
			 0x6fb4c0f2, /// 0x260
			 0x0c906b3d, /// 0x264
			 0x562ce7d6, /// 0x268
			 0x310a2bb2, /// 0x26c
			 0x9e1e9dae, /// 0x270
			 0x3680f8db, /// 0x274
			 0x40035a59, /// 0x278
			 0xce4975eb, /// 0x27c
			 0x086eb6ee, /// 0x280
			 0x84b8a1fc, /// 0x284
			 0xa2fb657d, /// 0x288
			 0x7b2aaee1, /// 0x28c
			 0x21c345ba, /// 0x290
			 0x2e39d40a, /// 0x294
			 0xf13d9d4b, /// 0x298
			 0xe6fbae3a, /// 0x29c
			 0x31246963, /// 0x2a0
			 0xc16e222b, /// 0x2a4
			 0xa6820af9, /// 0x2a8
			 0xf37370f9, /// 0x2ac
			 0x0e7b9270, /// 0x2b0
			 0xe695ee5b, /// 0x2b4
			 0x13ce8702, /// 0x2b8
			 0x961fa389, /// 0x2bc
			 0x7224d0a2, /// 0x2c0
			 0x7afcfaae, /// 0x2c4
			 0x85624c3f, /// 0x2c8
			 0xfe16ecd1, /// 0x2cc
			 0xe269b291, /// 0x2d0
			 0xfafe0002, /// 0x2d4
			 0x1484f05e, /// 0x2d8
			 0x38bf78ec, /// 0x2dc
			 0xe9716e21, /// 0x2e0
			 0x01c2ad1e, /// 0x2e4
			 0xb8b9a007, /// 0x2e8
			 0x8450e13f, /// 0x2ec
			 0x71a51bf9, /// 0x2f0
			 0x549a0ed7, /// 0x2f4
			 0x7a79cbc6, /// 0x2f8
			 0xddf2e696, /// 0x2fc
			 0x81ac5378, /// 0x300
			 0xaea6588d, /// 0x304
			 0x03827f68, /// 0x308
			 0x1f3f2fdf, /// 0x30c
			 0xd915b089, /// 0x310
			 0xa275a8aa, /// 0x314
			 0xe232e3d4, /// 0x318
			 0x0174ef38, /// 0x31c
			 0xadb8b9c3, /// 0x320
			 0x3f16809a, /// 0x324
			 0xf6fcff44, /// 0x328
			 0x32c96b92, /// 0x32c
			 0xd81b6767, /// 0x330
			 0x69de430a, /// 0x334
			 0x17d8a88c, /// 0x338
			 0xccec2aa1, /// 0x33c
			 0xe076a5c5, /// 0x340
			 0xc722e075, /// 0x344
			 0x4254c246, /// 0x348
			 0x36e9be58, /// 0x34c
			 0x007bf47f, /// 0x350
			 0x0e6ae2ee, /// 0x354
			 0x7d16d3e0, /// 0x358
			 0x20d20b49, /// 0x35c
			 0x327c5e00, /// 0x360
			 0xe7058513, /// 0x364
			 0xa8756d06, /// 0x368
			 0xc4ba81a1, /// 0x36c
			 0xd13004e0, /// 0x370
			 0x6b07a4b0, /// 0x374
			 0xa822b48a, /// 0x378
			 0x8a2fabf0, /// 0x37c
			 0xf1b30a65, /// 0x380
			 0xea9104e0, /// 0x384
			 0x7892468a, /// 0x388
			 0xe0e8b63f, /// 0x38c
			 0xa638658c, /// 0x390
			 0xe57fe6fa, /// 0x394
			 0x310188bb, /// 0x398
			 0x78eac6e8, /// 0x39c
			 0xdb0cd8ca, /// 0x3a0
			 0x1cbbb0c7, /// 0x3a4
			 0xadd6fd79, /// 0x3a8
			 0x9b06146f, /// 0x3ac
			 0x69e3f0f3, /// 0x3b0
			 0x30ce858c, /// 0x3b4
			 0xed9c141a, /// 0x3b8
			 0x00413fef, /// 0x3bc
			 0x0b783720, /// 0x3c0
			 0x75d3e9ac, /// 0x3c4
			 0xd8612a52, /// 0x3c8
			 0xf77a6a6c, /// 0x3cc
			 0xf5222f8c, /// 0x3d0
			 0x1822a700, /// 0x3d4
			 0x2e163fcb, /// 0x3d8
			 0xf9733f94, /// 0x3dc
			 0xed6e78d5, /// 0x3e0
			 0x0edc8d57, /// 0x3e4
			 0xd91f70f1, /// 0x3e8
			 0xd06122da, /// 0x3ec
			 0xf88df916, /// 0x3f0
			 0x620f8649, /// 0x3f4
			 0x04cb4266, /// 0x3f8
			 0x8c4a55fe, /// 0x3fc
			 0x17f0421b, /// 0x400
			 0x7ff0aaa1, /// 0x404
			 0xe2085f3f, /// 0x408
			 0x9156e634, /// 0x40c
			 0x22794560, /// 0x410
			 0x7012bd80, /// 0x414
			 0x75f5c1d0, /// 0x418
			 0xca89675d, /// 0x41c
			 0x8af54d79, /// 0x420
			 0xcd729b1f, /// 0x424
			 0x298dc5f0, /// 0x428
			 0x5d1e55db, /// 0x42c
			 0x277cf892, /// 0x430
			 0xbb305f05, /// 0x434
			 0xbc60bf50, /// 0x438
			 0x6bfb227e, /// 0x43c
			 0xfeea41b0, /// 0x440
			 0xf4a286e8, /// 0x444
			 0xf0943834, /// 0x448
			 0x6ff061a5, /// 0x44c
			 0x69e8548c, /// 0x450
			 0x985f1222, /// 0x454
			 0xcdc1b7fc, /// 0x458
			 0xd18a1b32, /// 0x45c
			 0x370d0aea, /// 0x460
			 0xadb7ac61, /// 0x464
			 0x26a10116, /// 0x468
			 0xad4ce88c, /// 0x46c
			 0xf6ef0edf, /// 0x470
			 0xef148f0b, /// 0x474
			 0x166eb4bf, /// 0x478
			 0x863d0402, /// 0x47c
			 0xf1e54a6c, /// 0x480
			 0x25fb770e, /// 0x484
			 0xfc406313, /// 0x488
			 0x18dab3cb, /// 0x48c
			 0x3933629c, /// 0x490
			 0xcdfc71b4, /// 0x494
			 0xbf581d16, /// 0x498
			 0x2ef11fb8, /// 0x49c
			 0xa8de27dc, /// 0x4a0
			 0xec138ab0, /// 0x4a4
			 0x0d6a8875, /// 0x4a8
			 0x0a852f6a, /// 0x4ac
			 0xa211cd21, /// 0x4b0
			 0x7bf2e6b6, /// 0x4b4
			 0xbbcb16ed, /// 0x4b8
			 0x3b95e177, /// 0x4bc
			 0x57b2c18d, /// 0x4c0
			 0x8ee2d5d2, /// 0x4c4
			 0x7b06bd30, /// 0x4c8
			 0x40e7ae12, /// 0x4cc
			 0x163a193c, /// 0x4d0
			 0x27cffa41, /// 0x4d4
			 0x81f9e0e5, /// 0x4d8
			 0xfdc1290e, /// 0x4dc
			 0xe0aabdba, /// 0x4e0
			 0x73161ac0, /// 0x4e4
			 0x221b1e07, /// 0x4e8
			 0xaf9254b1, /// 0x4ec
			 0xa336eae9, /// 0x4f0
			 0x7c452992, /// 0x4f4
			 0x6b8f1d80, /// 0x4f8
			 0xea33353f, /// 0x4fc
			 0x07b7ffdd, /// 0x500
			 0x8c6524e1, /// 0x504
			 0x577e96d4, /// 0x508
			 0xba6369da, /// 0x50c
			 0x646d316b, /// 0x510
			 0x9f44cb7e, /// 0x514
			 0xf3aae87f, /// 0x518
			 0x17828ad1, /// 0x51c
			 0x5398187d, /// 0x520
			 0xdeec54f6, /// 0x524
			 0x96f5f638, /// 0x528
			 0x4ccc2c07, /// 0x52c
			 0x28f613d3, /// 0x530
			 0x4779254e, /// 0x534
			 0x6f5451bb, /// 0x538
			 0x972d3e7e, /// 0x53c
			 0x34fa93c7, /// 0x540
			 0xd314767a, /// 0x544
			 0x9b40a0ef, /// 0x548
			 0xac1aa290, /// 0x54c
			 0x453a4227, /// 0x550
			 0xd5cc9be4, /// 0x554
			 0x5fe06cfb, /// 0x558
			 0x88636ec6, /// 0x55c
			 0x5ad7e58b, /// 0x560
			 0xa362b3a0, /// 0x564
			 0x4c5bd3e6, /// 0x568
			 0x95d272e8, /// 0x56c
			 0xf75d1d42, /// 0x570
			 0x4f48cbb1, /// 0x574
			 0x5e50f400, /// 0x578
			 0xdbd8fc3c, /// 0x57c
			 0x95947077, /// 0x580
			 0x259e1d94, /// 0x584
			 0x4adab834, /// 0x588
			 0x99990307, /// 0x58c
			 0xd6817b27, /// 0x590
			 0x7b41d0d5, /// 0x594
			 0x3e4f50b2, /// 0x598
			 0x744e4d30, /// 0x59c
			 0x3d7a5218, /// 0x5a0
			 0x8309f937, /// 0x5a4
			 0xb9173c08, /// 0x5a8
			 0x2e3ad46d, /// 0x5ac
			 0x46e5ad4f, /// 0x5b0
			 0x66715ec0, /// 0x5b4
			 0x966d720c, /// 0x5b8
			 0x00c0a5db, /// 0x5bc
			 0xcbfeb067, /// 0x5c0
			 0x34b859db, /// 0x5c4
			 0x8cff57be, /// 0x5c8
			 0x517f4efe, /// 0x5cc
			 0xf3af8653, /// 0x5d0
			 0x4f47e7dc, /// 0x5d4
			 0x82b74e34, /// 0x5d8
			 0xac141d28, /// 0x5dc
			 0xe47ce005, /// 0x5e0
			 0x813dada9, /// 0x5e4
			 0xf126d7a7, /// 0x5e8
			 0xa7c35d8d, /// 0x5ec
			 0x4a9b8baa, /// 0x5f0
			 0x9fa23d51, /// 0x5f4
			 0x9923bb2f, /// 0x5f8
			 0x25b40a9e, /// 0x5fc
			 0xdf8eff64, /// 0x600
			 0xe9a0a3cf, /// 0x604
			 0x42439145, /// 0x608
			 0x95a69680, /// 0x60c
			 0x04e9f0c4, /// 0x610
			 0x44d0c485, /// 0x614
			 0x5a5d69ce, /// 0x618
			 0xc891c5c8, /// 0x61c
			 0x5325a642, /// 0x620
			 0x313ae4a4, /// 0x624
			 0x4b66dcd8, /// 0x628
			 0xe2904be4, /// 0x62c
			 0xd53d7712, /// 0x630
			 0xcfc79400, /// 0x634
			 0x140133ac, /// 0x638
			 0xc2cc8599, /// 0x63c
			 0x50d80290, /// 0x640
			 0x1686c07d, /// 0x644
			 0xc6322e80, /// 0x648
			 0x9ad73b1b, /// 0x64c
			 0x57aa3549, /// 0x650
			 0x509ccc39, /// 0x654
			 0x0b9ff26f, /// 0x658
			 0xdd2f6ad1, /// 0x65c
			 0x6a1041e1, /// 0x660
			 0x76090b86, /// 0x664
			 0x39ef24a7, /// 0x668
			 0x89b6e636, /// 0x66c
			 0x0d53b44d, /// 0x670
			 0xa02e88d5, /// 0x674
			 0x9c7bd96b, /// 0x678
			 0x2803a0f6, /// 0x67c
			 0xc53c22ea, /// 0x680
			 0xb3866aef, /// 0x684
			 0x6efeafc3, /// 0x688
			 0xf279fa41, /// 0x68c
			 0xd49eee3a, /// 0x690
			 0x76883cf0, /// 0x694
			 0xd8a764b6, /// 0x698
			 0xecb839a7, /// 0x69c
			 0x6e72acdd, /// 0x6a0
			 0xbbff34a8, /// 0x6a4
			 0x9908e8ed, /// 0x6a8
			 0x5639a7ef, /// 0x6ac
			 0xb08090a4, /// 0x6b0
			 0xdcb5e468, /// 0x6b4
			 0x4f89c7cd, /// 0x6b8
			 0xeff630f1, /// 0x6bc
			 0x7ad98d13, /// 0x6c0
			 0x87656b2b, /// 0x6c4
			 0xd779b404, /// 0x6c8
			 0xc1c15479, /// 0x6cc
			 0x5d8e7a02, /// 0x6d0
			 0xf58217fc, /// 0x6d4
			 0x94e89f43, /// 0x6d8
			 0x519aee58, /// 0x6dc
			 0xaf1f6377, /// 0x6e0
			 0x2b3ef0ff, /// 0x6e4
			 0x8439b9d0, /// 0x6e8
			 0x9659e6ca, /// 0x6ec
			 0x83292b20, /// 0x6f0
			 0x02d5b96e, /// 0x6f4
			 0x80f2113c, /// 0x6f8
			 0x428d2fb8, /// 0x6fc
			 0xf223a552, /// 0x700
			 0x162aa237, /// 0x704
			 0x6d2ce62c, /// 0x708
			 0x4715adb7, /// 0x70c
			 0x746aefdd, /// 0x710
			 0x406db927, /// 0x714
			 0xad469838, /// 0x718
			 0x176cdadf, /// 0x71c
			 0xf22165d4, /// 0x720
			 0xc07d3c49, /// 0x724
			 0x0e42832f, /// 0x728
			 0x9644c217, /// 0x72c
			 0xcd70e281, /// 0x730
			 0x81fdaacd, /// 0x734
			 0xcf4f838f, /// 0x738
			 0x080f431a, /// 0x73c
			 0x1ac3a6de, /// 0x740
			 0x1826d31a, /// 0x744
			 0x56c17ab7, /// 0x748
			 0x0f429d87, /// 0x74c
			 0x1a132597, /// 0x750
			 0x8c0c52db, /// 0x754
			 0x7f08c51c, /// 0x758
			 0x97bda701, /// 0x75c
			 0x6c23ed16, /// 0x760
			 0x90d8a289, /// 0x764
			 0xeb190d46, /// 0x768
			 0x32ec8741, /// 0x76c
			 0xa368ce9f, /// 0x770
			 0x0007914e, /// 0x774
			 0x1357403a, /// 0x778
			 0x6aaad208, /// 0x77c
			 0x89453848, /// 0x780
			 0x8f96b83e, /// 0x784
			 0x5bab7a01, /// 0x788
			 0xf20e65bf, /// 0x78c
			 0x869990ee, /// 0x790
			 0xb83ff146, /// 0x794
			 0x2e112029, /// 0x798
			 0x8142d817, /// 0x79c
			 0x509bb209, /// 0x7a0
			 0x8f58349f, /// 0x7a4
			 0x358c3171, /// 0x7a8
			 0xe9f4f2a8, /// 0x7ac
			 0xc257d3c6, /// 0x7b0
			 0x83cb504b, /// 0x7b4
			 0x7c97d44f, /// 0x7b8
			 0x6a82ee16, /// 0x7bc
			 0x1263ee7d, /// 0x7c0
			 0x8ee6c39f, /// 0x7c4
			 0xbc24b20a, /// 0x7c8
			 0x3186cc2e, /// 0x7cc
			 0x9524b4af, /// 0x7d0
			 0xbf45047a, /// 0x7d4
			 0x2afd79ef, /// 0x7d8
			 0x10084294, /// 0x7dc
			 0x8dd987ab, /// 0x7e0
			 0x43201a29, /// 0x7e4
			 0x78038d37, /// 0x7e8
			 0xd5aa637c, /// 0x7ec
			 0x1911526e, /// 0x7f0
			 0xd3c4c4f3, /// 0x7f4
			 0xb918dc82, /// 0x7f8
			 0xa84e93ba, /// 0x7fc
			 0xccc4fb71, /// 0x800
			 0xc4df82ae, /// 0x804
			 0x850c5fae, /// 0x808
			 0x6d43425b, /// 0x80c
			 0xa60570d2, /// 0x810
			 0x5ad2fc6c, /// 0x814
			 0xa999d5d6, /// 0x818
			 0xf0ef9751, /// 0x81c
			 0x5c159ff9, /// 0x820
			 0xc29788d2, /// 0x824
			 0x29071b7f, /// 0x828
			 0xfc8af9be, /// 0x82c
			 0xfe07ba0e, /// 0x830
			 0x6c085433, /// 0x834
			 0xb86e8e3e, /// 0x838
			 0x86b57555, /// 0x83c
			 0x5335f14e, /// 0x840
			 0x033d2c0d, /// 0x844
			 0x1f944f49, /// 0x848
			 0xd1d0034d, /// 0x84c
			 0x1adf33b3, /// 0x850
			 0x9474f7f5, /// 0x854
			 0x2893126e, /// 0x858
			 0x3fa1d48e, /// 0x85c
			 0xd45c5289, /// 0x860
			 0x7e56d491, /// 0x864
			 0xb1b965b9, /// 0x868
			 0xe68059e7, /// 0x86c
			 0x1a36e27c, /// 0x870
			 0x12e588dc, /// 0x874
			 0xdc5abb4f, /// 0x878
			 0x991f9e53, /// 0x87c
			 0xa84db886, /// 0x880
			 0x3f19c44e, /// 0x884
			 0x31011697, /// 0x888
			 0x9056bea6, /// 0x88c
			 0xbbfdf783, /// 0x890
			 0xea008796, /// 0x894
			 0xaaad67c4, /// 0x898
			 0x9b472907, /// 0x89c
			 0x2bdc7d20, /// 0x8a0
			 0xa5951728, /// 0x8a4
			 0x7bbc377a, /// 0x8a8
			 0xe45bc018, /// 0x8ac
			 0xaa4e2edd, /// 0x8b0
			 0x49ddb61d, /// 0x8b4
			 0x812cb0e6, /// 0x8b8
			 0xdab5de37, /// 0x8bc
			 0xf07d73c6, /// 0x8c0
			 0x5b9ecb47, /// 0x8c4
			 0x391e3668, /// 0x8c8
			 0xbf54d3e1, /// 0x8cc
			 0x8af68440, /// 0x8d0
			 0xda9ae124, /// 0x8d4
			 0xec84e40d, /// 0x8d8
			 0xe6095a13, /// 0x8dc
			 0x6922c8d3, /// 0x8e0
			 0x0edffad6, /// 0x8e4
			 0x894bb0b5, /// 0x8e8
			 0x3d899028, /// 0x8ec
			 0xd14ae7b5, /// 0x8f0
			 0x745dc013, /// 0x8f4
			 0xa2761fa7, /// 0x8f8
			 0xa440bc81, /// 0x8fc
			 0xbec57a3b, /// 0x900
			 0x8b798723, /// 0x904
			 0xd28bd836, /// 0x908
			 0xd14cdadb, /// 0x90c
			 0x98368b29, /// 0x910
			 0xbd5a2c78, /// 0x914
			 0xfa9e691a, /// 0x918
			 0x4f46d616, /// 0x91c
			 0x305bb4fb, /// 0x920
			 0xc8bce5c5, /// 0x924
			 0xb2a902d2, /// 0x928
			 0x75b1c929, /// 0x92c
			 0x1ed6bb97, /// 0x930
			 0x52ecec66, /// 0x934
			 0x6dcb228e, /// 0x938
			 0xf530571a, /// 0x93c
			 0xd6951298, /// 0x940
			 0x994b1e76, /// 0x944
			 0x3cc594a0, /// 0x948
			 0x7e368f1b, /// 0x94c
			 0x8ff273f3, /// 0x950
			 0xb73ca5ed, /// 0x954
			 0x085c060e, /// 0x958
			 0xe27ffe34, /// 0x95c
			 0xfb1ae3e8, /// 0x960
			 0x46e85a22, /// 0x964
			 0x6f789ec1, /// 0x968
			 0x3a158d4e, /// 0x96c
			 0x2a923daf, /// 0x970
			 0x5e4d8b80, /// 0x974
			 0x879499f5, /// 0x978
			 0x69da1306, /// 0x97c
			 0x4b06f9e8, /// 0x980
			 0x53f9b938, /// 0x984
			 0x1874eb18, /// 0x988
			 0x701f17f6, /// 0x98c
			 0xb1d8674f, /// 0x990
			 0xe78c07f3, /// 0x994
			 0x51202a93, /// 0x998
			 0xf0088c01, /// 0x99c
			 0x2bd13814, /// 0x9a0
			 0x6d99e750, /// 0x9a4
			 0xb0454798, /// 0x9a8
			 0x57541b21, /// 0x9ac
			 0xccd35635, /// 0x9b0
			 0x09ea217b, /// 0x9b4
			 0xf33fb8c7, /// 0x9b8
			 0xabc496c2, /// 0x9bc
			 0x18744046, /// 0x9c0
			 0x18120a90, /// 0x9c4
			 0xf91d5152, /// 0x9c8
			 0x77824c86, /// 0x9cc
			 0xe92dd4b0, /// 0x9d0
			 0x499e2cb2, /// 0x9d4
			 0xc94f94f5, /// 0x9d8
			 0x7b9b3e1c, /// 0x9dc
			 0x70882ab9, /// 0x9e0
			 0x11d8b3a3, /// 0x9e4
			 0xaa5ac2fa, /// 0x9e8
			 0x36ecc092, /// 0x9ec
			 0xaa7d0079, /// 0x9f0
			 0xe72e04f5, /// 0x9f4
			 0x18d00024, /// 0x9f8
			 0x5d07923f, /// 0x9fc
			 0x7465900f, /// 0xa00
			 0x8ff2e652, /// 0xa04
			 0xb3169cee, /// 0xa08
			 0xbe241ac0, /// 0xa0c
			 0xf72546fd, /// 0xa10
			 0xbf8e8e5b, /// 0xa14
			 0xa482dd96, /// 0xa18
			 0xda2903be, /// 0xa1c
			 0x4545b2ec, /// 0xa20
			 0xb8b13dc5, /// 0xa24
			 0x647009f6, /// 0xa28
			 0x252d7321, /// 0xa2c
			 0xcb7497f3, /// 0xa30
			 0x5ab16f85, /// 0xa34
			 0xe003fa1a, /// 0xa38
			 0x8cb77398, /// 0xa3c
			 0x970edd73, /// 0xa40
			 0x1e615d96, /// 0xa44
			 0xc232e4a6, /// 0xa48
			 0x0387cedc, /// 0xa4c
			 0x2ee2ddd4, /// 0xa50
			 0x2caa636b, /// 0xa54
			 0x39f4ff71, /// 0xa58
			 0x301a3da5, /// 0xa5c
			 0xd0443dca, /// 0xa60
			 0xc82d8886, /// 0xa64
			 0x254729e9, /// 0xa68
			 0x5dbc557d, /// 0xa6c
			 0xe230b72a, /// 0xa70
			 0xc955148f, /// 0xa74
			 0xdbbf2a06, /// 0xa78
			 0x6ab43f62, /// 0xa7c
			 0x67889702, /// 0xa80
			 0x58391563, /// 0xa84
			 0xae324097, /// 0xa88
			 0xe3be958c, /// 0xa8c
			 0xf68956df, /// 0xa90
			 0xe2dbde20, /// 0xa94
			 0xb8468abf, /// 0xa98
			 0x16497e7f, /// 0xa9c
			 0x51ea29e1, /// 0xaa0
			 0x9f62fc98, /// 0xaa4
			 0xfb1a2a85, /// 0xaa8
			 0xa8bbb6ef, /// 0xaac
			 0xde7b85ea, /// 0xab0
			 0xcc51fdb2, /// 0xab4
			 0x1532f9b0, /// 0xab8
			 0x73e5ca6c, /// 0xabc
			 0x0e990400, /// 0xac0
			 0x3cb97181, /// 0xac4
			 0x211b23a1, /// 0xac8
			 0x47b9fd09, /// 0xacc
			 0x30fdb244, /// 0xad0
			 0x8ecc4b52, /// 0xad4
			 0xc1e77b52, /// 0xad8
			 0x8f9b91e1, /// 0xadc
			 0x6de29a2b, /// 0xae0
			 0xfbc928f3, /// 0xae4
			 0x6dc34ef1, /// 0xae8
			 0xf3577f78, /// 0xaec
			 0xb1c73d71, /// 0xaf0
			 0xca846307, /// 0xaf4
			 0x94328cdf, /// 0xaf8
			 0x1c2d386c, /// 0xafc
			 0xf2d84dd8, /// 0xb00
			 0xcda953e0, /// 0xb04
			 0xdabbc159, /// 0xb08
			 0x53082475, /// 0xb0c
			 0x383d32b0, /// 0xb10
			 0x427fdb3c, /// 0xb14
			 0x158105a0, /// 0xb18
			 0x7f82c9d7, /// 0xb1c
			 0x2e6c9a7e, /// 0xb20
			 0x46f7b295, /// 0xb24
			 0xf49d3c70, /// 0xb28
			 0xd3085b58, /// 0xb2c
			 0x1d1e6f90, /// 0xb30
			 0x5802ba8b, /// 0xb34
			 0x882fcdc8, /// 0xb38
			 0xc4e93a79, /// 0xb3c
			 0x4a8239c0, /// 0xb40
			 0x293dd325, /// 0xb44
			 0x57b40084, /// 0xb48
			 0xc91077c4, /// 0xb4c
			 0xac0f8097, /// 0xb50
			 0xaf04b2a3, /// 0xb54
			 0x2afbe4c2, /// 0xb58
			 0xa11394c9, /// 0xb5c
			 0x78e587cc, /// 0xb60
			 0xc530907e, /// 0xb64
			 0x543be1b6, /// 0xb68
			 0xbdf4394e, /// 0xb6c
			 0x685b146b, /// 0xb70
			 0x64ee5166, /// 0xb74
			 0xb8e5b2bd, /// 0xb78
			 0x503422d6, /// 0xb7c
			 0xc10cd96b, /// 0xb80
			 0xcd3ab468, /// 0xb84
			 0xc4c69347, /// 0xb88
			 0x37823eff, /// 0xb8c
			 0x26ed1c85, /// 0xb90
			 0x233c83c5, /// 0xb94
			 0xf1063064, /// 0xb98
			 0x67483601, /// 0xb9c
			 0xfde388bf, /// 0xba0
			 0xa1a13450, /// 0xba4
			 0x41a7f441, /// 0xba8
			 0x5dcdf965, /// 0xbac
			 0xc67c4ae4, /// 0xbb0
			 0x27a5874e, /// 0xbb4
			 0xd75275ce, /// 0xbb8
			 0x828e2ebe, /// 0xbbc
			 0xb23b93e6, /// 0xbc0
			 0x57d893e3, /// 0xbc4
			 0x07851199, /// 0xbc8
			 0x62c66e20, /// 0xbcc
			 0x35d0afe4, /// 0xbd0
			 0xc7b87fb1, /// 0xbd4
			 0x3f93e3ea, /// 0xbd8
			 0xe626b78f, /// 0xbdc
			 0xb501d38c, /// 0xbe0
			 0x782de508, /// 0xbe4
			 0x412180ee, /// 0xbe8
			 0x6b16ba63, /// 0xbec
			 0x41c17eb2, /// 0xbf0
			 0x2d5420ad, /// 0xbf4
			 0xb91f8eab, /// 0xbf8
			 0x786bb585, /// 0xbfc
			 0x5cc0b8fe, /// 0xc00
			 0x583709dc, /// 0xc04
			 0xb3a10fb2, /// 0xc08
			 0xa7e374fd, /// 0xc0c
			 0x7e77d832, /// 0xc10
			 0x882bb2bf, /// 0xc14
			 0xca6f536d, /// 0xc18
			 0xe7f25890, /// 0xc1c
			 0x7a94911b, /// 0xc20
			 0xe677ce50, /// 0xc24
			 0x07a0b82a, /// 0xc28
			 0x8b09a7c9, /// 0xc2c
			 0x0fffdbbc, /// 0xc30
			 0x3c4e44f3, /// 0xc34
			 0x4f2a6a35, /// 0xc38
			 0x8b6ee088, /// 0xc3c
			 0x2c58459d, /// 0xc40
			 0xde5b2f44, /// 0xc44
			 0x9bc2fd96, /// 0xc48
			 0xa5b25197, /// 0xc4c
			 0xe1bb0be3, /// 0xc50
			 0xc7a68647, /// 0xc54
			 0x9f39528b, /// 0xc58
			 0x341e5e68, /// 0xc5c
			 0x19e0e358, /// 0xc60
			 0x5522ef88, /// 0xc64
			 0x2469cde5, /// 0xc68
			 0x4f661128, /// 0xc6c
			 0xf3489e8d, /// 0xc70
			 0x73cecb17, /// 0xc74
			 0x5e502d70, /// 0xc78
			 0xdbe819be, /// 0xc7c
			 0x4ad1aa80, /// 0xc80
			 0xa6399a78, /// 0xc84
			 0xbd4c8370, /// 0xc88
			 0xc13fe412, /// 0xc8c
			 0x28df09e7, /// 0xc90
			 0x21902b8b, /// 0xc94
			 0xcae8f314, /// 0xc98
			 0xbfe5d7ac, /// 0xc9c
			 0xcf12263b, /// 0xca0
			 0x5d58628d, /// 0xca4
			 0x6e653ff9, /// 0xca8
			 0xb32d10aa, /// 0xcac
			 0xdd53b808, /// 0xcb0
			 0x6ba07f92, /// 0xcb4
			 0xcede6a94, /// 0xcb8
			 0x8878b9ce, /// 0xcbc
			 0x5742b1c4, /// 0xcc0
			 0xe48bc09e, /// 0xcc4
			 0xf3620a28, /// 0xcc8
			 0x1a96013c, /// 0xccc
			 0x9ff183d3, /// 0xcd0
			 0xf83e70ba, /// 0xcd4
			 0x8ba13216, /// 0xcd8
			 0xe11fd153, /// 0xcdc
			 0x7fbcf754, /// 0xce0
			 0xd753aceb, /// 0xce4
			 0xab1beed3, /// 0xce8
			 0x10396397, /// 0xcec
			 0xd62d9476, /// 0xcf0
			 0x0fa0cba6, /// 0xcf4
			 0xa13b2a60, /// 0xcf8
			 0xdd2278c4, /// 0xcfc
			 0x355e8740, /// 0xd00
			 0x52187c26, /// 0xd04
			 0xd1d22c8d, /// 0xd08
			 0x3141cf71, /// 0xd0c
			 0x2678eeb0, /// 0xd10
			 0xf04573ff, /// 0xd14
			 0xceace17e, /// 0xd18
			 0x3fd96296, /// 0xd1c
			 0x3c3aa3d7, /// 0xd20
			 0x6a2ffe3d, /// 0xd24
			 0xeae69766, /// 0xd28
			 0xe7bad8d7, /// 0xd2c
			 0xf6fe2c56, /// 0xd30
			 0x297f082c, /// 0xd34
			 0x691b546d, /// 0xd38
			 0x54cf3f7f, /// 0xd3c
			 0xfb9cf3f5, /// 0xd40
			 0x70ec6fab, /// 0xd44
			 0xbf69d552, /// 0xd48
			 0x4189b265, /// 0xd4c
			 0x1e29e7e0, /// 0xd50
			 0x0c50c8a7, /// 0xd54
			 0xd5812ef7, /// 0xd58
			 0xbf07be1b, /// 0xd5c
			 0x85c5bd30, /// 0xd60
			 0x2cd4fb57, /// 0xd64
			 0x58cccfe2, /// 0xd68
			 0x226813b2, /// 0xd6c
			 0xd4e4f14e, /// 0xd70
			 0xafd14f4b, /// 0xd74
			 0x6af3632c, /// 0xd78
			 0xfef10f9b, /// 0xd7c
			 0x7033a7e9, /// 0xd80
			 0xf9cb15ed, /// 0xd84
			 0x0d0cdae6, /// 0xd88
			 0xaefc83eb, /// 0xd8c
			 0x32081682, /// 0xd90
			 0x9586781e, /// 0xd94
			 0x216b532a, /// 0xd98
			 0x8ddd4f2d, /// 0xd9c
			 0xeba9822f, /// 0xda0
			 0xf373cf6b, /// 0xda4
			 0x626c2fa8, /// 0xda8
			 0x4b3899b3, /// 0xdac
			 0x888098a2, /// 0xdb0
			 0xcd1007d5, /// 0xdb4
			 0x1a6b314e, /// 0xdb8
			 0xb0a9cb06, /// 0xdbc
			 0x1025a1f8, /// 0xdc0
			 0x7f9fa847, /// 0xdc4
			 0x3dc4bc4b, /// 0xdc8
			 0x0a4595ef, /// 0xdcc
			 0x0a2796d8, /// 0xdd0
			 0x62bd87b9, /// 0xdd4
			 0x03728958, /// 0xdd8
			 0xf6c2faa6, /// 0xddc
			 0x97fff51e, /// 0xde0
			 0xc8d966c3, /// 0xde4
			 0x3006d991, /// 0xde8
			 0x4927f2b8, /// 0xdec
			 0xee0182df, /// 0xdf0
			 0x88daab12, /// 0xdf4
			 0xc2d90141, /// 0xdf8
			 0x5b40efff, /// 0xdfc
			 0x0380591c, /// 0xe00
			 0xb8c38f08, /// 0xe04
			 0x4f2e8b0d, /// 0xe08
			 0xab4a6710, /// 0xe0c
			 0x90bd33e7, /// 0xe10
			 0xeb19225e, /// 0xe14
			 0x4d8ada79, /// 0xe18
			 0xb48ba814, /// 0xe1c
			 0xb132ef28, /// 0xe20
			 0x2acd13f4, /// 0xe24
			 0x52433996, /// 0xe28
			 0xb72d4a16, /// 0xe2c
			 0xa107c3ed, /// 0xe30
			 0xb9187d83, /// 0xe34
			 0x59b38107, /// 0xe38
			 0xb43835cb, /// 0xe3c
			 0x98665df2, /// 0xe40
			 0x205fe6bd, /// 0xe44
			 0xadce11e1, /// 0xe48
			 0x92dc222b, /// 0xe4c
			 0x5a8c876b, /// 0xe50
			 0xa41bae9a, /// 0xe54
			 0xadf3660d, /// 0xe58
			 0x641b345c, /// 0xe5c
			 0xcee43540, /// 0xe60
			 0x1a86bdbe, /// 0xe64
			 0xd2531525, /// 0xe68
			 0x7f812e55, /// 0xe6c
			 0xf60b3d02, /// 0xe70
			 0xb97a9139, /// 0xe74
			 0x8faf52f5, /// 0xe78
			 0x02129540, /// 0xe7c
			 0xf7950ab2, /// 0xe80
			 0x062cea0f, /// 0xe84
			 0xb5b7339a, /// 0xe88
			 0x0bd96360, /// 0xe8c
			 0x38a73105, /// 0xe90
			 0x8c23f886, /// 0xe94
			 0x078576e8, /// 0xe98
			 0xcb31aaf8, /// 0xe9c
			 0x1aa6a8ec, /// 0xea0
			 0xb12bfc43, /// 0xea4
			 0xee427c5f, /// 0xea8
			 0x402a00bd, /// 0xeac
			 0xbfee252f, /// 0xeb0
			 0xc3eb90fa, /// 0xeb4
			 0x59f1ebca, /// 0xeb8
			 0xfca644d4, /// 0xebc
			 0x473d6e8c, /// 0xec0
			 0x3d88a171, /// 0xec4
			 0xcf51a5d6, /// 0xec8
			 0xfc351bab, /// 0xecc
			 0xa20d5672, /// 0xed0
			 0xfd717895, /// 0xed4
			 0x8c8e23b2, /// 0xed8
			 0x0ba1132e, /// 0xedc
			 0x12aa04c2, /// 0xee0
			 0x5d6a86ff, /// 0xee4
			 0x0f475c7e, /// 0xee8
			 0xf2f8890a, /// 0xeec
			 0x347df8ea, /// 0xef0
			 0xba2f276d, /// 0xef4
			 0x35817fd8, /// 0xef8
			 0xf59e87a2, /// 0xefc
			 0xb9098b7d, /// 0xf00
			 0x08b10584, /// 0xf04
			 0x06cd90b9, /// 0xf08
			 0x0f82b77f, /// 0xf0c
			 0xc716cf6d, /// 0xf10
			 0xc992b304, /// 0xf14
			 0x543d1271, /// 0xf18
			 0xd15f9493, /// 0xf1c
			 0x240bd940, /// 0xf20
			 0xac05badf, /// 0xf24
			 0x8fe3476c, /// 0xf28
			 0x614a152a, /// 0xf2c
			 0x8142eaec, /// 0xf30
			 0xc58b81f2, /// 0xf34
			 0x28d9f448, /// 0xf38
			 0x60d657b7, /// 0xf3c
			 0xf5788604, /// 0xf40
			 0x3ea1262a, /// 0xf44
			 0xfab6bcca, /// 0xf48
			 0x863cbeaa, /// 0xf4c
			 0x41a954e5, /// 0xf50
			 0x434e1566, /// 0xf54
			 0x8d0b89a6, /// 0xf58
			 0x73c0dfdf, /// 0xf5c
			 0x065f0b12, /// 0xf60
			 0x320034d0, /// 0xf64
			 0x6f8aa068, /// 0xf68
			 0x08060618, /// 0xf6c
			 0x37b46102, /// 0xf70
			 0x16376b17, /// 0xf74
			 0x77ae393c, /// 0xf78
			 0x4dbdf54f, /// 0xf7c
			 0x0743886d, /// 0xf80
			 0x21ce0ae3, /// 0xf84
			 0xe404b572, /// 0xf88
			 0x225ed513, /// 0xf8c
			 0x6ca95eed, /// 0xf90
			 0xf3036bb1, /// 0xf94
			 0xf6c2f5a1, /// 0xf98
			 0xa9e8f89c, /// 0xf9c
			 0x50af3a80, /// 0xfa0
			 0xa03fb7a7, /// 0xfa4
			 0x3507589d, /// 0xfa8
			 0x6d9e90d7, /// 0xfac
			 0x0b630867, /// 0xfb0
			 0x16a2e2a1, /// 0xfb4
			 0x18cd61d5, /// 0xfb8
			 0x6222178e, /// 0xfbc
			 0xe7e0ef4d, /// 0xfc0
			 0x688f286c, /// 0xfc4
			 0x177e63f4, /// 0xfc8
			 0x98966920, /// 0xfcc
			 0xf1aeec8d, /// 0xfd0
			 0x14df1b22, /// 0xfd4
			 0x2d7c7dd1, /// 0xfd8
			 0x9db4cdc3, /// 0xfdc
			 0x07db24f9, /// 0xfe0
			 0x8108348b, /// 0xfe4
			 0xc91c1eba, /// 0xfe8
			 0x6d368e41, /// 0xfec
			 0xa7374420, /// 0xff0
			 0xb174d522, /// 0xff4
			 0xa55ada89, /// 0xff8
			 0x9a45331d /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00004
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00008
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00010
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0001c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00024
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00028
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0002c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00030
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00034
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00038
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0003c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00040
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0004c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00050
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00054
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00058
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0005c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00060
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0xff800000,                                                  // -inf                                        /// 00068
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0006c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00070
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x4b000000,                                                  // 8388608.0                                   /// 0007c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00080
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0008c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00090
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00098
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0009c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000a0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000a4
			 0x0e000007,                                                  // Trailing 1s:                                /// 000a8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000ac
			 0xffc00001,                                                  // -signaling NaN                              /// 000b0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000b4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000b8
			 0x80000000,                                                  // -zero                                       /// 000bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000c0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000c4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000d0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000d4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000dc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x4b000000,                                                  // 8388608.0                                   /// 000e4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000e8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000f0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000f8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00100
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00104
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00108
			 0x00011111,                                                  // 9.7958E-41                                  /// 0010c
			 0x33333333,                                                  // 4 random values                             /// 00110
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00118
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0011c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00120
			 0x80000000,                                                  // -zero                                       /// 00124
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00128
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0012c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00134
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00138
			 0x80011111,                                                  // -9.7958E-41                                 /// 0013c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0xbf028f5c,                                                  // -0.51                                       /// 00144
			 0x00000000,                                                  // zero                                        /// 00148
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00150
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00154
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00158
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00160
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0016c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00170
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00180
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00184
			 0xcb000000,                                                  // -8388608.0                                  /// 00188
			 0x80000000,                                                  // -zero                                       /// 0018c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00190
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00194
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 001a0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 001a8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001b0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001b4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001b8
			 0x7fc00001,                                                  // signaling NaN                               /// 001bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001c0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001c4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001c8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001cc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001d0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001d4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001d8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001e0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001e8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001f0
			 0x0c600000,                                                  // Leading 1s:                                 /// 001f4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00200
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00204
			 0x33333333,                                                  // 4 random values                             /// 00208
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x3f028f5c,                                                  // 0.51                                        /// 00210
			 0x33333333,                                                  // 4 random values                             /// 00214
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00218
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00220
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00224
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0022c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00234
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00240
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00248
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0024c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00250
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00254
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00258
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0xcb000000,                                                  // -8388608.0                                  /// 00268
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00270
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00274
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00278
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0027c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00280
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00284
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x00000000,                                                  // zero                                        /// 0028c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00290
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00294
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00298
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0029c
			 0x4b000000,                                                  // 8388608.0                                   /// 002a0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x00011111,                                                  // 9.7958E-41                                  /// 002b0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002c0
			 0xbf028f5c,                                                  // -0.51                                       /// 002c4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 002c8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002cc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002d0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002d4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002d8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002dc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0xcb000000,                                                  // -8388608.0                                  /// 002e8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002ec
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002fc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00300
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00304
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00308
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0030c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00310
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00314
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00318
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00320
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00328
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0032c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00330
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00334
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00338
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0033c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00340
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00344
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00348
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0034c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00350
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00358
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0035c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00364
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00368
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0036c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00370
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00378
			 0x80000000,                                                  // -zero                                       /// 0037c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00380
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00384
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00388
			 0x0c780000,                                                  // Leading 1s:                                 /// 0038c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00390
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00398
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003a0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003a4
			 0x80011111,                                                  // -9.7958E-41                                 /// 003a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003b0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003b4
			 0xff800000,                                                  // -inf                                        /// 003b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003bc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003c8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003cc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 003d0
			 0x55555555,                                                  // 4 random values                             /// 003d4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003d8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003dc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003e8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003f4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003f8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00400
			 0x7f800000,                                                  // inf                                         /// 00404
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00408
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0040c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00414
			 0x0c600000,                                                  // Leading 1s:                                 /// 00418
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00424
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0042c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00430
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00434
			 0x4b000000,                                                  // 8388608.0                                   /// 00438
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00440
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00444
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0044c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00454
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00458
			 0x7fc00001,                                                  // signaling NaN                               /// 0045c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00460
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0046c
			 0x00000000,                                                  // zero                                        /// 00470
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00474
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00478
			 0x7f800000,                                                  // inf                                         /// 0047c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00480
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00484
			 0x3f028f5c,                                                  // 0.51                                        /// 00488
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0048c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00494
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00498
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004a0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004a4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004a8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004ac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004b0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004b4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004b8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004bc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 004c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004c8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004cc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004d0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004d8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004dc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004e0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004e4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004e8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004ec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004f4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004fc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00500
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00504
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00508
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00514
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00518
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0051c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00520
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00524
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00528
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0052c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00530
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00534
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00538
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0053c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00540
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00544
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00548
			 0x4b000000,                                                  // 8388608.0                                   /// 0054c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00554
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0055c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00560
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00564
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00568
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0056c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00570
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00578
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0057c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00580
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00584
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00588
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0058c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00594
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00598
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0059c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005a0
			 0xcb000000,                                                  // -8388608.0                                  /// 005a4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005ac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005b4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005b8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005bc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005c0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005c4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 005d0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005d8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005dc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0xffc00001,                                                  // -signaling NaN                              /// 005e4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005ec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005f0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005f8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005fc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00604
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00608
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0060c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00610
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00614
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00618
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0061c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00620
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0062c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00630
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x80011111,                                                  // -9.7958E-41                                 /// 00638
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00640
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00644
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00648
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00650
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00654
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00658
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0065c
			 0xbf028f5c,                                                  // -0.51                                       /// 00660
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00668
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0066c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00670
			 0xff800000,                                                  // -inf                                        /// 00674
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00678
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0067c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00680
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00684
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0068c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00690
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00694
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0069c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006a0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006a4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006b0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006b4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006b8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006bc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006c4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006c8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006d0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006dc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006e0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006e8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f0
			 0x7f800000,                                                  // inf                                         /// 006f4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006fc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00704
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00708
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0070c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00710
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00718
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00720
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00724
			 0xcb000000,                                                  // -8388608.0                                  /// 00728
			 0x0c780000,                                                  // Leading 1s:                                 /// 0072c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0xcb000000,                                                  // -8388608.0                                  /// 00738
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0073c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00740
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0074c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00750
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00754
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00758
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0075c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00764
			 0xbf028f5c,                                                  // -0.51                                       /// 00768
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0076c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00770
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00774
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00778
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0077c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00784
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00788
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0078c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00790
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00794
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00798
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0079c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007a0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a4
			 0x0e000007,                                                  // Trailing 1s:                                /// 007a8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007b8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007c0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007c4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007cc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007d0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007d4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007d8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007dc
			 0xbf028f5c,                                                  // -0.51                                       /// 007e0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007e4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007e8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007f4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007f8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00800
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00804
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00808
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00810
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00814
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00820
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00824
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00828
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0082c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0083c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x0c700000,                                                  // Leading 1s:                                 /// 00844
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00848
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0084c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00850
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00854
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00858
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0085c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00864
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00868
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0086c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00870
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00874
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00878
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0087c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00880
			 0xff800000,                                                  // -inf                                        /// 00884
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0088c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00890
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00894
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00898
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0089c
			 0x80000000,                                                  // -zero                                       /// 008a0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008a8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008ac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008b0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008b4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008b8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008bc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008cc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008d8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008e0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 008e4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008e8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008ec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008f0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008f4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00900
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00908
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0090c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00914
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00918
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0091c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00920
			 0x4b000000,                                                  // 8388608.0                                   /// 00924
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00928
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00930
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00938
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0093c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00940
			 0x00000000,                                                  // zero                                        /// 00944
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00948
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0094c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00954
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00958
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0095c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00960
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00964
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00970
			 0x0e000001,                                                  // Trailing 1s:                                /// 00974
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00978
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0097c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00980
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00984
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00988
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0098c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00990
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00994
			 0x00000000,                                                  // zero                                        /// 00998
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0099c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009a0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x7f800000,                                                  // inf                                         /// 009a8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009b4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x00000000,                                                  // zero                                        /// 009bc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009c8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 009d8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009dc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009e0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009e4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009e8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009ec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009f0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a00
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a04
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a08
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a0c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a10
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a14
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a18
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a1c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a20
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a24
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a2c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a34
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a38
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a3c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a40
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a44
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a4c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a50
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a5c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a60
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a64
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a68
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a6c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a70
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a74
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a78
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a7c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a80
			 0x3f028f5c,                                                  // 0.51                                        /// 00a84
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a88
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a90
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a94
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a98
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a9c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00aa0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00aa4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00aac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ab0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ab4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ab8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00abc
			 0x80000000,                                                  // -zero                                       /// 00ac0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ac8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00acc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0xbf028f5c,                                                  // -0.51                                       /// 00ad4
			 0x33333333,                                                  // 4 random values                             /// 00ad8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00adc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ae0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ae8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00aec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00af0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00af8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00afc
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b00
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b04
			 0xffc00001,                                                  // -signaling NaN                              /// 00b08
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b14
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b1c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b20
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b28
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b2c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b30
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b34
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b38
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b3c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b40
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b44
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b50
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b54
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b58
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b5c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b60
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b64
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x7fc00001,                                                  // signaling NaN                               /// 00b6c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b70
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b74
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b78
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b7c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b80
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b84
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b8c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b90
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b9c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ba0
			 0x33333333,                                                  // 4 random values                             /// 00ba4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ba8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bb0
			 0x7fc00001,                                                  // signaling NaN                               /// 00bb4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bb8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bbc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bc4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00bc8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00bcc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bd0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bd4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bd8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00be0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00be4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00be8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bfc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c00
			 0x7fc00001,                                                  // signaling NaN                               /// 00c04
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00c0c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c14
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c18
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c1c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c20
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c24
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0xff800000,                                                  // -inf                                        /// 00c2c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c34
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c40
			 0xff800000,                                                  // -inf                                        /// 00c44
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c48
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c4c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c50
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c54
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c60
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c64
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c68
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c70
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c78
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c7c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c80
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c84
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c88
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c90
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c94
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c9c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ca0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ca4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ca8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00cb0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cb4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00cb8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cbc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cc0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cc4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00cc8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ccc
			 0x00000000,                                                  // zero                                        /// 00cd0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cd8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cdc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ce4
			 0x7f800000,                                                  // inf                                         /// 00ce8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cf0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cf4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d04
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d08
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d0c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d14
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d18
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d1c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d20
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d24
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d28
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d2c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d30
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d34
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d38
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d3c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d40
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d4c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d54
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d5c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d64
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d68
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d6c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d70
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d74
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d78
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d7c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d80
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d84
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d8c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d90
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d94
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d98
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00d9c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00da0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00da4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00da8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x55555555,                                                  // 4 random values                             /// 00db0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00db8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00dc0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dc8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00dcc
			 0xffc00001,                                                  // -signaling NaN                              /// 00dd0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dd4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dd8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ddc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x00000000,                                                  // zero                                        /// 00de8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00df0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00df4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00df8
			 0x80000000,                                                  // -zero                                       /// 00dfc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e00
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e08
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e0c
			 0x33333333,                                                  // 4 random values                             /// 00e10
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e14
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e18
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e1c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e20
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e28
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e2c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e30
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e34
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e40
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e44
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e48
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e4c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e50
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e58
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e5c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e60
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e64
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e68
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e6c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e70
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e74
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e78
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e80
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e84
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e88
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0xcb000000,                                                  // -8388608.0                                  /// 00e90
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e94
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e98
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e9c
			 0x4b000000,                                                  // 8388608.0                                   /// 00ea0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ea4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ea8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00eb0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00eb4
			 0x80000000,                                                  // -zero                                       /// 00eb8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ebc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ec4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0xffc00001,                                                  // -signaling NaN                              /// 00ed0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ed4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x33333333,                                                  // 4 random values                             /// 00edc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ee0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ee4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ee8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ef0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ef4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f00
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f08
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f0c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x4b000000,                                                  // 8388608.0                                   /// 00f14
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f18
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f1c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f28
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f2c
			 0x4b000000,                                                  // 8388608.0                                   /// 00f30
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f34
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f38
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f3c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f44
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f48
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f54
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f58
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f60
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f64
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f6c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f70
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f78
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f7c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f84
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f94
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f98
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f9c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fa0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fa8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fb0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fb4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fbc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fc0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fc4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00fc8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fcc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fd0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fdc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fe0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00fe8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ff8
			 0x80000100                                                  // SP - 1 bit alone set in mantissa -ve        /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00000
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00008
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0000c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00010
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00014
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00018
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00020
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00024
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00028
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0002c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00030
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00034
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00038
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00044
			 0xff800000,                                                  // -inf                                        /// 00048
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0004c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0xff800000,                                                  // -inf                                        /// 00054
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00058
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x7f800000,                                                  // inf                                         /// 00060
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00068
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0006c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00070
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00074
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00078
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0008c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00094
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00098
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0009c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000a0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000a4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000ac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000b0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000b4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000bc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000c0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000c4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000d4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000dc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 000e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000e8
			 0x0c780000,                                                  // Leading 1s:                                 /// 000ec
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 000f8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00104
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00108
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0010c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00114
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00118
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0011c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00120
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00124
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0012c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00134
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00138
			 0x0c400000,                                                  // Leading 1s:                                 /// 0013c
			 0xcb000000,                                                  // -8388608.0                                  /// 00140
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00150
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00154
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00158
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00164
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00168
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00174
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00178
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0017c
			 0x4b000000,                                                  // 8388608.0                                   /// 00180
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00188
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0018c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00190
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00194
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00198
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0019c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001a0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001a8
			 0x00000000,                                                  // zero                                        /// 001ac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001b0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001b8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001bc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 001c0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001c4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001c8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001cc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001d0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001dc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001e4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001ec
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001f0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001fc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00200
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x00011111,                                                  // 9.7958E-41                                  /// 00208
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0020c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00210
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00214
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0021c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00220
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00224
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00230
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00234
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0023c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00240
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00244
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00248
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0024c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00250
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00254
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0025c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00264
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00268
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0026c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00270
			 0x33333333,                                                  // 4 random values                             /// 00274
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00278
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0027c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00284
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00288
			 0x33333333,                                                  // 4 random values                             /// 0028c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00290
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00294
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002a8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002ac
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002b0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002b8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002bc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002c0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002c8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 002cc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002d8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x0c600000,                                                  // Leading 1s:                                 /// 002e0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002e8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002f0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002f4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00304
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00308
			 0x55555555,                                                  // 4 random values                             /// 0030c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00310
			 0x00011111,                                                  // 9.7958E-41                                  /// 00314
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00318
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0031c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00320
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00324
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00328
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0032c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00330
			 0x0e000001,                                                  // Trailing 1s:                                /// 00334
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0033c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0034c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00350
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x0e000001,                                                  // Trailing 1s:                                /// 00358
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0035c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00360
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00364
			 0x7fc00001,                                                  // signaling NaN                               /// 00368
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00370
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00374
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00378
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0037c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00384
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00388
			 0xff800000,                                                  // -inf                                        /// 0038c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x0e000001,                                                  // Trailing 1s:                                /// 003a0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003ac
			 0xbf028f5c,                                                  // -0.51                                       /// 003b0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003bc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003c8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003cc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003d8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003dc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 003f0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003f4
			 0x80000000,                                                  // -zero                                       /// 003f8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003fc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00400
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00404
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00408
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0040c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0041c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00424
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00428
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0042c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00430
			 0x0c700000,                                                  // Leading 1s:                                 /// 00434
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00438
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0043c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00440
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00444
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x0c400000,                                                  // Leading 1s:                                 /// 0044c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x7fc00001,                                                  // signaling NaN                               /// 00458
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0045c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00460
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0046c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00470
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00474
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00478
			 0x80011111,                                                  // -9.7958E-41                                 /// 0047c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00480
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00488
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0048c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00494
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0049c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004a0
			 0x0c600000,                                                  // Leading 1s:                                 /// 004a4
			 0x0e000001,                                                  // Trailing 1s:                                /// 004a8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004ac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x7f800000,                                                  // inf                                         /// 004b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004c0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004c4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004d4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004d8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004dc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004e4
			 0x0c600000,                                                  // Leading 1s:                                 /// 004e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004ec
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 004f4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00500
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00504
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00508
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0050c
			 0xffc00001,                                                  // -signaling NaN                              /// 00510
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00514
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00520
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00524
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00528
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0052c
			 0x00000000,                                                  // zero                                        /// 00530
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00538
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00544
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0054c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00550
			 0x0c700000,                                                  // Leading 1s:                                 /// 00554
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00558
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0055c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00560
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00564
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00568
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0056c
			 0x33333333,                                                  // 4 random values                             /// 00570
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x00000000,                                                  // zero                                        /// 0057c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00580
			 0xbf028f5c,                                                  // -0.51                                       /// 00584
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00588
			 0x0e000007,                                                  // Trailing 1s:                                /// 0058c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00590
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00594
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00598
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005a0
			 0xff800000,                                                  // -inf                                        /// 005a4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005a8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005ac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005b0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005b4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005b8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005c0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005c4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005c8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 005d4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005d8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005dc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x0e000007,                                                  // Trailing 1s:                                /// 005e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005f0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005f8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0xcb000000,                                                  // -8388608.0                                  /// 00600
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x0e000003,                                                  // Trailing 1s:                                /// 0060c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00610
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00614
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00618
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0061c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00620
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00624
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00628
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0062c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00630
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00634
			 0x0c400000,                                                  // Leading 1s:                                 /// 00638
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0063c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00640
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0064c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00650
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00654
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00658
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0065c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00660
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00664
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00668
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0066c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00670
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00678
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0067c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00688
			 0x3f028f5c,                                                  // 0.51                                        /// 0068c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00690
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00694
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 006a0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006a4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006a8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006b4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 006bc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006c0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 006c8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006d0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006d4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006e4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006e8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006ec
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006f0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 006f8
			 0x0c600000,                                                  // Leading 1s:                                 /// 006fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00700
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00704
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0070c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00710
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00714
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00718
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00724
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x3f028f5c,                                                  // 0.51                                        /// 00734
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00738
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0073c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00740
			 0x3f028f5c,                                                  // 0.51                                        /// 00744
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00748
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00750
			 0x7f800000,                                                  // inf                                         /// 00754
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00758
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0075c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00764
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00768
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0076c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00770
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00774
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00778
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0077c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00780
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00784
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0078c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00790
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00794
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00798
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0079c
			 0x80011111,                                                  // -9.7958E-41                                 /// 007a0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007a8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007ac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 007b0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007c0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007c8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007cc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007d0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007d4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007e0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007e4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007e8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007ec
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007f4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00808
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0080c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00810
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00814
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00818
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00820
			 0x7f800000,                                                  // inf                                         /// 00824
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00828
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0082c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00830
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00834
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00838
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0083c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00840
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00844
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0084c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00854
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0085c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x0e000001,                                                  // Trailing 1s:                                /// 00864
			 0x0e000001,                                                  // Trailing 1s:                                /// 00868
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0086c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00870
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00874
			 0xbf028f5c,                                                  // -0.51                                       /// 00878
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0087c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00884
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00888
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0088c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00890
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00894
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00898
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0089c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008a4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008ac
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008bc
			 0x55555555,                                                  // 4 random values                             /// 008c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008c8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008cc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008dc
			 0xff800000,                                                  // -inf                                        /// 008e0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008e8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 008ec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008f8
			 0x7fc00001,                                                  // signaling NaN                               /// 008fc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00900
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0xff800000,                                                  // -inf                                        /// 0090c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00910
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x00011111,                                                  // 9.7958E-41                                  /// 00918
			 0xcb000000,                                                  // -8388608.0                                  /// 0091c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00924
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0092c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00930
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00934
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00938
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0093c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00940
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00944
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00948
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0094c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00950
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00954
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00960
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00968
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0096c
			 0x55555555,                                                  // 4 random values                             /// 00970
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00974
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00978
			 0x0c700000,                                                  // Leading 1s:                                 /// 0097c
			 0x80000000,                                                  // -zero                                       /// 00980
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00984
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00988
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0098c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00990
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00994
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00998
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0099c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009a0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009a4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009a8
			 0x0c400000,                                                  // Leading 1s:                                 /// 009ac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009b0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009b4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009b8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009bc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x7f800000,                                                  // inf                                         /// 009d0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009d4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009d8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009e8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009ec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 009f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009f4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009fc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a00
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a04
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a08
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a14
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a18
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a20
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a24
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a28
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a2c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a30
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a38
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a3c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a40
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a44
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x4b000000,                                                  // 8388608.0                                   /// 00a4c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a50
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a54
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a58
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a60
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a64
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a68
			 0xcb000000,                                                  // -8388608.0                                  /// 00a6c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a70
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a80
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a84
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a88
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a90
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a94
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a9c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00aa4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00aa8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ab0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ab4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ab8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00abc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ac4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ac8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ad0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ad8
			 0xcb000000,                                                  // -8388608.0                                  /// 00adc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00ae0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ae4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ae8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00af0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00af4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00af8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b00
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b04
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b0c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b10
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b14
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b18
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b1c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b20
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b24
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b28
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b2c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b30
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b34
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b3c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b40
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b44
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b48
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b4c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b50
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b54
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b5c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b68
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b6c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b70
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b74
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b78
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b80
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b88
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b8c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b98
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b9c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ba8
			 0x7fc00001,                                                  // signaling NaN                               /// 00bac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bb4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00bb8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00bbc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00bc0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bc4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bc8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bd0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bd4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bd8
			 0x55555555,                                                  // 4 random values                             /// 00bdc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00be0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00bec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bf0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bf4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bf8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bfc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c00
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c08
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c10
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c14
			 0x00000000,                                                  // zero                                        /// 00c18
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c1c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c20
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c24
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c28
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c2c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c30
			 0x7f800000,                                                  // inf                                         /// 00c34
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c38
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c40
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c4c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c50
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c54
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c58
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c64
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c68
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c6c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c70
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c74
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c78
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c80
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c84
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c88
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c8c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c90
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c94
			 0x33333333,                                                  // 4 random values                             /// 00c98
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c9c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ca0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ca4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cac
			 0x7f800000,                                                  // inf                                         /// 00cb0
			 0x33333333,                                                  // 4 random values                             /// 00cb4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cb8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x00000000,                                                  // zero                                        /// 00cc0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cc8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cd8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x7f800000,                                                  // inf                                         /// 00ce0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ce4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ce8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cf0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cf4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cfc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d00
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d04
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d08
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d0c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d10
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d14
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d18
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d1c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d24
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d28
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d30
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d38
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d40
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d44
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d48
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d4c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d50
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d58
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d64
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d6c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d70
			 0x3f028f5c,                                                  // 0.51                                        /// 00d74
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d78
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d7c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d80
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d84
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d88
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d8c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d90
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d94
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d98
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d9c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00da4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00da8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dac
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00db0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00db4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00db8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00dbc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00dc0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dc4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00dc8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dcc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00dd8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00de4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00dec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00df0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00df8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e04
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e08
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e14
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e18
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e1c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e20
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e28
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e2c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e30
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e34
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e38
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e3c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e40
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e48
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e4c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e50
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e54
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e5c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e60
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e64
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e68
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e6c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e74
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e78
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e7c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e80
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e84
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e88
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e98
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e9c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ea0
			 0xffc00001,                                                  // -signaling NaN                              /// 00ea4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ea8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00eb4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00eb8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ec0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ec4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ec8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ecc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ed0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ed4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00edc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ee8
			 0xbf028f5c,                                                  // -0.51                                       /// 00eec
			 0xcb000000,                                                  // -8388608.0                                  /// 00ef0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ef4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ef8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00efc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f00
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f04
			 0xffc00001,                                                  // -signaling NaN                              /// 00f08
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f10
			 0xffc00001,                                                  // -signaling NaN                              /// 00f14
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f18
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x4b000000,                                                  // 8388608.0                                   /// 00f20
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f24
			 0x55555555,                                                  // 4 random values                             /// 00f28
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f2c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f34
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f3c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f40
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f48
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f4c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f54
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f58
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f5c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f60
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f64
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f68
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f6c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f70
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f74
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f7c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f80
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f84
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f88
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f8c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f90
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f94
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fa0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fa4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0xff800000,                                                  // -inf                                        /// 00fb0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fb4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fb8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fbc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fc4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00fcc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fd0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fd4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fd8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fdc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fe0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fe4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fe8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ff0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x80800001                                                  // none of the mantissa set to +3ulp           /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00004
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00010
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00014
			 0x0c780000,                                                  // Leading 1s:                                 /// 00018
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0001c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00020
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00024
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00028
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0002c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00030
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00034
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00038
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0003c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00040
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00044
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00048
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0004c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00050
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00054
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0005c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00060
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00064
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00068
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00070
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00074
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0007c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00080
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00084
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00088
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00090
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00098
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0009c
			 0xffc00001,                                                  // -signaling NaN                              /// 000a0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000a8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000ac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000b4
			 0x0c700000,                                                  // Leading 1s:                                 /// 000b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000bc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000c0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000c4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000cc
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000d0
			 0x7f800000,                                                  // inf                                         /// 000d4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000d8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000dc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000e0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000e4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000e8
			 0x33333333,                                                  // 4 random values                             /// 000ec
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000f0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00108
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0010c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00110
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00114
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00118
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0011c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00120
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00128
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0012c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00130
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00134
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00138
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0013c
			 0x33333333,                                                  // 4 random values                             /// 00140
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00144
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00148
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x33333333,                                                  // 4 random values                             /// 00150
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00154
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00158
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0015c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00160
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00164
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00168
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0016c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00170
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00174
			 0x55555555,                                                  // 4 random values                             /// 00178
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0017c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00184
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00190
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00194
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00198
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0019c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001a0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001a4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001ac
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001b0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001b4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001b8
			 0x00000000,                                                  // zero                                        /// 001bc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001c0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001c8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001cc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001d0
			 0xff800000,                                                  // -inf                                        /// 001d4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001d8
			 0x00000000,                                                  // zero                                        /// 001dc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001e0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001e4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001e8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001f0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 001fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00208
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00210
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00214
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0021c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00228
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0022c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00230
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00240
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00244
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00248
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00250
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00254
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0025c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00260
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00264
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00268
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0026c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x0c600000,                                                  // Leading 1s:                                 /// 00274
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00278
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0027c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00280
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00284
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00288
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0028c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00290
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00294
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00298
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002a0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002ac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002b8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002c0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002c4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002c8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002cc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002d0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002d4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002d8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002dc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002e0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002e4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002e8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002f0
			 0xcb000000,                                                  // -8388608.0                                  /// 002f4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 002fc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00300
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00308
			 0x00000000,                                                  // zero                                        /// 0030c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00310
			 0xffc00001,                                                  // -signaling NaN                              /// 00314
			 0x3f028f5c,                                                  // 0.51                                        /// 00318
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00320
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00324
			 0x0e000001,                                                  // Trailing 1s:                                /// 00328
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0032c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00330
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00334
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00338
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00340
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00344
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00348
			 0x0e000001,                                                  // Trailing 1s:                                /// 0034c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00354
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00358
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00360
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00364
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0036c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00374
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0037c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00380
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0xff800000,                                                  // -inf                                        /// 00388
			 0x0e000003,                                                  // Trailing 1s:                                /// 0038c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00394
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00398
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0039c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003a0
			 0xcb000000,                                                  // -8388608.0                                  /// 003a4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003ac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003b4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 003b8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003c0
			 0x4b000000,                                                  // 8388608.0                                   /// 003c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003c8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003cc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003d0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003d8
			 0x80000000,                                                  // -zero                                       /// 003dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003e0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003e4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003e8
			 0x3f028f5c,                                                  // 0.51                                        /// 003ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003f0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 003f4
			 0x00000000,                                                  // zero                                        /// 003f8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00400
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00408
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0040c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00410
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00414
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00418
			 0x0e000001,                                                  // Trailing 1s:                                /// 0041c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00420
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00428
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0042c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00430
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00434
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00438
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0043c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0x7fc00001,                                                  // signaling NaN                               /// 00444
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00448
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0044c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00450
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00458
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0045c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00460
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0046c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00470
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00474
			 0x0c780000,                                                  // Leading 1s:                                 /// 00478
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0047c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00484
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0048c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00490
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00494
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00498
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0049c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 004a0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004ac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004b0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004b4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x80000000,                                                  // -zero                                       /// 004c4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004c8
			 0x00011111,                                                  // 9.7958E-41                                  /// 004cc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004d0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004d4
			 0xffc00001,                                                  // -signaling NaN                              /// 004d8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004e0
			 0x0c780000,                                                  // Leading 1s:                                 /// 004e4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x0e000007,                                                  // Trailing 1s:                                /// 004ec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004f4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004f8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004fc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00500
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00504
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00508
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0050c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00510
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00514
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00518
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0051c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00520
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00524
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00528
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0052c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00534
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00538
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0053c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00544
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00548
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0054c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00550
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00554
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00558
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0055c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00560
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00564
			 0x0e000001,                                                  // Trailing 1s:                                /// 00568
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0056c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00570
			 0x0e000003,                                                  // Trailing 1s:                                /// 00574
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00578
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0057c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00588
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00594
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00598
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0059c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005a4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005ac
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005b0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005b4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005b8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005bc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005c0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005c4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005c8
			 0x0e000001,                                                  // Trailing 1s:                                /// 005cc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005d4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005d8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005e0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x33333333,                                                  // 4 random values                             /// 005e8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x80000000,                                                  // -zero                                       /// 005f0
			 0x80000000,                                                  // -zero                                       /// 005f4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 005f8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005fc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00600
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00604
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00608
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0060c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00610
			 0x33333333,                                                  // 4 random values                             /// 00614
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00618
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00620
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x55555555,                                                  // 4 random values                             /// 00628
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00630
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00634
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00638
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0063c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00640
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00644
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00648
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0064c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0xcb000000,                                                  // -8388608.0                                  /// 00654
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00658
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00660
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00664
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00668
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0066c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00670
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00674
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00678
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x00000000,                                                  // zero                                        /// 00680
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00688
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0068c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00690
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00694
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00698
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0069c
			 0x33333333,                                                  // 4 random values                             /// 006a0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006a8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006ac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006b0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006b8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006bc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006c0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006c8
			 0x00000000,                                                  // zero                                        /// 006cc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 006d4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 006dc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006e4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006e8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006ec
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006f0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006f4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006fc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00708
			 0x3f028f5c,                                                  // 0.51                                        /// 0070c
			 0xff800000,                                                  // -inf                                        /// 00710
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x00011111,                                                  // 9.7958E-41                                  /// 00718
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0071c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00720
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00724
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x7fc00001,                                                  // signaling NaN                               /// 0072c
			 0x80000000,                                                  // -zero                                       /// 00730
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0073c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00740
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00748
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00750
			 0x80011111,                                                  // -9.7958E-41                                 /// 00754
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00758
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0075c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0xff800000,                                                  // -inf                                        /// 00768
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0076c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00774
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00778
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0077c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00780
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00784
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00788
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00790
			 0x80011111,                                                  // -9.7958E-41                                 /// 00794
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00798
			 0xbf028f5c,                                                  // -0.51                                       /// 0079c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007a0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007a4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 007a8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007ac
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007b4
			 0x7fc00001,                                                  // signaling NaN                               /// 007b8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007c0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007c8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007cc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007d0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007d4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007d8
			 0x33333333,                                                  // 4 random values                             /// 007dc
			 0x00000000,                                                  // zero                                        /// 007e0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007e8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007fc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x80011111,                                                  // -9.7958E-41                                 /// 00804
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00808
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0080c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00810
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00814
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00818
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00820
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00824
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00828
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0082c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00830
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00838
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0083c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x0c700000,                                                  // Leading 1s:                                 /// 00848
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0084c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00850
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00854
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00858
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0085c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00860
			 0x33333333,                                                  // 4 random values                             /// 00864
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0xcb000000,                                                  // -8388608.0                                  /// 0086c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00870
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00878
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0087c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00884
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00888
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00890
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00894
			 0x0c600000,                                                  // Leading 1s:                                 /// 00898
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0089c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 008a0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008a8
			 0x0c400000,                                                  // Leading 1s:                                 /// 008ac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008b4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008b8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008bc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008c4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008cc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008d0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008d8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 008dc
			 0x00000000,                                                  // zero                                        /// 008e0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008e4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008f0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008f4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008f8
			 0x80000000,                                                  // -zero                                       /// 008fc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00904
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00908
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0090c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00910
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00914
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00918
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0091c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00924
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00928
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0092c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00930
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0xbf028f5c,                                                  // -0.51                                       /// 00938
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0093c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00944
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00948
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00950
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00954
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00958
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0095c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00960
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00964
			 0x0c600000,                                                  // Leading 1s:                                 /// 00968
			 0x80011111,                                                  // -9.7958E-41                                 /// 0096c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00970
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00974
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0097c
			 0x00000000,                                                  // zero                                        /// 00980
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00984
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00988
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0098c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00990
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00994
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00998
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0099c
			 0x0e000001,                                                  // Trailing 1s:                                /// 009a0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009a4
			 0x0c600000,                                                  // Leading 1s:                                 /// 009a8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009ac
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009b0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009b4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009bc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009c0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 009c8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009cc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009d0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009d8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009dc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009e0
			 0x00011111,                                                  // 9.7958E-41                                  /// 009e4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009e8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009ec
			 0xcb000000,                                                  // -8388608.0                                  /// 009f0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009f4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009f8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009fc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a00
			 0xffc00001,                                                  // -signaling NaN                              /// 00a04
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a08
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a0c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a10
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a14
			 0xcb000000,                                                  // -8388608.0                                  /// 00a18
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a20
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a28
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a2c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a30
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a34
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a38
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a40
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a44
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a48
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a5c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a68
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a6c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a70
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a78
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a7c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a80
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a84
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a88
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a8c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a90
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a94
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a9c
			 0x7f800000,                                                  // inf                                         /// 00aa0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00aa4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00aa8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00aac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ab4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00abc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ac0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ac4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ac8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00acc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ad8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00adc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ae0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ae4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ae8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00af0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00af4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00af8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00afc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b00
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b08
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b14
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b18
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b1c
			 0xff800000,                                                  // -inf                                        /// 00b20
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b24
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b28
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b2c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b34
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b3c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b40
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b44
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b48
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b50
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b54
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b58
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b5c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b64
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b68
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b6c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b74
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b78
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b7c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b80
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b84
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b88
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b8c
			 0xff800000,                                                  // -inf                                        /// 00b90
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b94
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b98
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b9c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ba0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ba4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bb0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bb4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bb8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bbc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bc0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bc4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00bc8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bcc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bd0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00bd8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bdc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00be0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00be8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00bf0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bf4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bf8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c04
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c0c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c10
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c18
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c1c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c20
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c30
			 0xcb000000,                                                  // -8388608.0                                  /// 00c34
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c38
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c3c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c40
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c44
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c4c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c58
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c5c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c60
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00c68
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c6c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c74
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c7c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c80
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c88
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c8c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c90
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c98
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c9c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ca0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ca4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ca8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cac
			 0x00000000,                                                  // zero                                        /// 00cb0
			 0x4b000000,                                                  // 8388608.0                                   /// 00cb4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cb8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x4b000000,                                                  // 8388608.0                                   /// 00cc0
			 0xff800000,                                                  // -inf                                        /// 00cc4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cc8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ccc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00cd0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cdc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ce0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ce4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ce8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cf0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cf4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cf8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00cfc
			 0x00000000,                                                  // zero                                        /// 00d00
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d04
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d10
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d14
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d1c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d20
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d28
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d2c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d34
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d38
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d3c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d40
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d44
			 0x7f800000,                                                  // inf                                         /// 00d48
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d4c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d58
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d5c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d60
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d64
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d68
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d6c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d70
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d74
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d78
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d7c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d80
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x33333333,                                                  // 4 random values                             /// 00d88
			 0xffc00001,                                                  // -signaling NaN                              /// 00d8c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d94
			 0x33333333,                                                  // 4 random values                             /// 00d98
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00da0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00da8
			 0x7f800000,                                                  // inf                                         /// 00dac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00db0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00db4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00db8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dbc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00dcc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00dd4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00dd8
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ddc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00de0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00de4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00de8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00df0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00df4
			 0x00000000,                                                  // zero                                        /// 00df8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00dfc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e04
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e08
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e0c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e10
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e14
			 0xcb000000,                                                  // -8388608.0                                  /// 00e18
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e20
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e24
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e30
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e34
			 0xcb000000,                                                  // -8388608.0                                  /// 00e38
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e3c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e40
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e4c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e50
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e54
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e58
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e5c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e64
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x4b000000,                                                  // 8388608.0                                   /// 00e70
			 0x7f800000,                                                  // inf                                         /// 00e74
			 0x33333333,                                                  // 4 random values                             /// 00e78
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00e7c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e84
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e88
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e8c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e90
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e94
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e9c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ea0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ea4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ea8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00eac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00eb0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00eb4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00eb8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ebc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ec4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ec8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ed4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ed8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00edc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ee0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ee4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ee8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00eec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ef0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ef4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ef8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00efc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f00
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f04
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f08
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f0c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f10
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f18
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f1c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f20
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f24
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f28
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f2c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f30
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f34
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f40
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f44
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f48
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f4c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f54
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f58
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f60
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f68
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f6c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f74
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f78
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f7c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f80
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f84
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f88
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f8c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f90
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f94
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f98
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f9c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00fa0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fa8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fb0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fb4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0xff800000,                                                  // -inf                                        /// 00fbc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fc0
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fc4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fc8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fd0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fd8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fdc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fe0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ff0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ff4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x7f000000 // norm with max exp min mant                   // SP +ve numbers                              /// last
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
			 0x000003d4,
			 0x000000ec,
			 0x000003b8,
			 0x000005ec,
			 0x00000180,
			 0x00000030,
			 0x000005a0,
			 0x00000724,

			 /// vpu register f2
			 0x41880000,
			 0x40e00000,
			 0x41600000,
			 0x41600000,
			 0x41f80000,
			 0x41200000,
			 0x41800000,
			 0x41000000,

			 /// vpu register f3
			 0x3f800000,
			 0x41a80000,
			 0x41a80000,
			 0x41100000,
			 0x41c80000,
			 0x40c00000,
			 0x41200000,
			 0x41c00000,

			 /// vpu register f4
			 0x41e00000,
			 0x41c80000,
			 0x40400000,
			 0x41880000,
			 0x41880000,
			 0x40000000,
			 0x41700000,
			 0x41b80000,

			 /// vpu register f5
			 0x42000000,
			 0x41800000,
			 0x41f00000,
			 0x42000000,
			 0x41600000,
			 0x41e80000,
			 0x40c00000,
			 0x41c00000,

			 /// vpu register f6
			 0x41100000,
			 0x41500000,
			 0x41800000,
			 0x41800000,
			 0x40000000,
			 0x41c00000,
			 0x40400000,
			 0x41800000,

			 /// vpu register f7
			 0x41980000,
			 0x41980000,
			 0x40e00000,
			 0x41980000,
			 0x41c00000,
			 0x40800000,
			 0x40a00000,
			 0x41700000,

			 /// vpu register f8
			 0x41980000,
			 0x41a80000,
			 0x41400000,
			 0x40c00000,
			 0x41f80000,
			 0x41a00000,
			 0x41000000,
			 0x3f800000,

			 /// vpu register f9
			 0x40000000,
			 0x41600000,
			 0x40000000,
			 0x41f00000,
			 0x41d80000,
			 0x41f00000,
			 0x41b80000,
			 0x41f80000,

			 /// vpu register f10
			 0x40400000,
			 0x41100000,
			 0x41f80000,
			 0x41980000,
			 0x41880000,
			 0x41f00000,
			 0x41f80000,
			 0x41c80000,

			 /// vpu register f11
			 0x40400000,
			 0x40800000,
			 0x42000000,
			 0x41b00000,
			 0x41600000,
			 0x40c00000,
			 0x41c80000,
			 0x41e00000,

			 /// vpu register f12
			 0x41880000,
			 0x41700000,
			 0x41f80000,
			 0x40e00000,
			 0x41b00000,
			 0x41200000,
			 0x40400000,
			 0x40c00000,

			 /// vpu register f13
			 0x41a80000,
			 0x42000000,
			 0x41a80000,
			 0x41a80000,
			 0x41500000,
			 0x41100000,
			 0x41880000,
			 0x41d80000,

			 /// vpu register f14
			 0x41600000,
			 0x41880000,
			 0x40800000,
			 0x41800000,
			 0x41e00000,
			 0x41a80000,
			 0x41e80000,
			 0x41000000,

			 /// vpu register f15
			 0x41f00000,
			 0x41b00000,
			 0x41b80000,
			 0x40e00000,
			 0x41e00000,
			 0x41b00000,
			 0x41a00000,
			 0x41500000,

			 /// vpu register f16
			 0x41500000,
			 0x41b80000,
			 0x40e00000,
			 0x41e80000,
			 0x41e00000,
			 0x40400000,
			 0x41000000,
			 0x41a80000,

			 /// vpu register f17
			 0x41400000,
			 0x41700000,
			 0x41d80000,
			 0x41f00000,
			 0x40400000,
			 0x41b80000,
			 0x41e00000,
			 0x40c00000,

			 /// vpu register f18
			 0x41500000,
			 0x41980000,
			 0x41400000,
			 0x41f80000,
			 0x41e80000,
			 0x41a00000,
			 0x41c00000,
			 0x41a80000,

			 /// vpu register f19
			 0x42000000,
			 0x41e80000,
			 0x3f800000,
			 0x40c00000,
			 0x41a80000,
			 0x40a00000,
			 0x41e80000,
			 0x40e00000,

			 /// vpu register f20
			 0x42000000,
			 0x40e00000,
			 0x40a00000,
			 0x41600000,
			 0x41b80000,
			 0x41980000,
			 0x40000000,
			 0x41700000,

			 /// vpu register f21
			 0x40800000,
			 0x41300000,
			 0x41c80000,
			 0x41300000,
			 0x41900000,
			 0x3f800000,
			 0x41800000,
			 0x41f00000,

			 /// vpu register f22
			 0x40a00000,
			 0x41a00000,
			 0x41c00000,
			 0x41c00000,
			 0x41d00000,
			 0x41100000,
			 0x41f00000,
			 0x42000000,

			 /// vpu register f23
			 0x41980000,
			 0x40000000,
			 0x41100000,
			 0x41700000,
			 0x40800000,
			 0x41200000,
			 0x41000000,
			 0x41b80000,

			 /// vpu register f24
			 0x41c00000,
			 0x41400000,
			 0x41300000,
			 0x40e00000,
			 0x40000000,
			 0x40c00000,
			 0x41200000,
			 0x41f00000,

			 /// vpu register f25
			 0x41800000,
			 0x41600000,
			 0x42000000,
			 0x41980000,
			 0x41b80000,
			 0x41f00000,
			 0x41c80000,
			 0x40e00000,

			 /// vpu register f26
			 0x41000000,
			 0x41900000,
			 0x41880000,
			 0x41a00000,
			 0x41e80000,
			 0x41a00000,
			 0x41d00000,
			 0x41400000,

			 /// vpu register f27
			 0x41980000,
			 0x41c80000,
			 0x41b80000,
			 0x41e80000,
			 0x41600000,
			 0x41500000,
			 0x41f00000,
			 0x41880000,

			 /// vpu register f28
			 0x41100000,
			 0x41a00000,
			 0x40a00000,
			 0x41c00000,
			 0x41b80000,
			 0x41500000,
			 0x41800000,
			 0x40e00000,

			 /// vpu register f29
			 0x41500000,
			 0x41a00000,
			 0x41e00000,
			 0x41a80000,
			 0x41100000,
			 0x41c00000,
			 0x41b00000,
			 0x41600000,

			 /// vpu register f30
			 0x41300000,
			 0x41700000,
			 0x41d00000,
			 0x41100000,
			 0x41900000,
			 0x41b80000,
			 0x40000000,
			 0x41b80000,

			 /// vpu register f31
			 0x41800000,
			 0x41c00000,
			 0x41c80000,
			 0x41a80000,
			 0x41b80000,
			 0x41880000,
			 0x41500000,
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
		"ffrc.ps f2, f21\n"                                   ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f28, f12\n"                                  ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f5, f20\n"                                   ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f27, f19\n"                                  ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f24, f1\n"                                   ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f16, f14\n"                                  ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f3, f27\n"                                   ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f26, f29\n"                                  ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f25, f13\n"                                  ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f21, f30\n"                                  ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f4, f29\n"                                   ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f27, f21\n"                                  ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f17, f4\n"                                   ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f14, f14\n"                                  ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f5, f4\n"                                    ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f8, f26\n"                                   ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f9, f0\n"                                    ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f13, f21\n"                                  ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f16, f25\n"                                  ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f19, f25\n"                                  ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f27, f0\n"                                   ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f21, f0\n"                                   ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f17, f8\n"                                   ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f15, f4\n"                                   ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f26, f4\n"                                   ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f10, f13\n"                                  ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f8, f1\n"                                    ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f10, f16\n"                                  ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f11, f16\n"                                  ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f22, f12\n"                                  ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f29, f2\n"                                   ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f21, f23\n"                                  ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f3, f9\n"                                    ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f4, f8\n"                                    ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f3, f12\n"                                   ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f18, f19\n"                                  ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f14, f8\n"                                   ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f6, f24\n"                                   ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f22, f15\n"                                  ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f17, f14\n"                                  ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f2, f17\n"                                   ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f22, f24\n"                                  ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f17, f2\n"                                   ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f2, f20\n"                                   ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f12, f8\n"                                   ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f15, f27\n"                                  ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f7, f23\n"                                   ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f12, f25\n"                                  ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f24, f4\n"                                   ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f3, f8\n"                                    ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f26, f1\n"                                   ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f22, f29\n"                                  ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f11, f15\n"                                  ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f11, f21\n"                                  ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f9, f29\n"                                   ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f26, f6\n"                                   ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f13, f10\n"                                  ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f19, f20\n"                                  ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f18, f22\n"                                  ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f7, f7\n"                                    ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f25, f6\n"                                   ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f23, f16\n"                                  ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f18, f3\n"                                   ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f4, f9\n"                                    ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f20, f18\n"                                  ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f28, f7\n"                                   ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f5, f12\n"                                   ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f28, f19\n"                                  ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f6, f1\n"                                    ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f26, f26\n"                                  ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f27, f4\n"                                   ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f7, f13\n"                                   ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f16, f20\n"                                  ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f15, f6\n"                                   ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f23, f7\n"                                   ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f25, f20\n"                                  ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f16, f27\n"                                  ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f15, f30\n"                                  ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f18, f24\n"                                  ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f14, f1\n"                                   ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f6, f30\n"                                   ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f4, f3\n"                                    ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f26, f20\n"                                  ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f3, f18\n"                                   ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f7, f15\n"                                   ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f24, f4\n"                                   ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f13, f2\n"                                   ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f12, f12\n"                                  ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f4, f5\n"                                    ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f30, f1\n"                                   ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f28, f27\n"                                  ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f8, f21\n"                                   ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f13, f1\n"                                   ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f17, f28\n"                                  ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f30, f0\n"                                   ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f26, f29\n"                                  ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f3, f14\n"                                   ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f15, f19\n"                                  ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"ffrc.ps f26, f11\n"                                  ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
