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
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00000
			 0x80011111,                                                  // -9.7958E-41                                 /// 00004
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0000c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0001c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00020
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00024
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00028
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0002c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x80011111,                                                  // -9.7958E-41                                 /// 00034
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0003c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00040
			 0x33333333,                                                  // 4 random values                             /// 00044
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00048
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0004c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00050
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00054
			 0x33333333,                                                  // 4 random values                             /// 00058
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00060
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00064
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00068
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0006c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00074
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0007c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00080
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x33333333,                                                  // 4 random values                             /// 00088
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0008c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00090
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00094
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0009c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 000a4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000b0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000b4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000bc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000c0
			 0x0c700000,                                                  // Leading 1s:                                 /// 000c4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000c8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000d0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000d4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000d8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000dc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000e0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000e8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000ec
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000f0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000f4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000f8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000fc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00100
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00104
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0010c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00110
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00118
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0011c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00120
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00124
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00128
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0012c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00130
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00134
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00140
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00144
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00148
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00150
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00154
			 0x0c400000,                                                  // Leading 1s:                                 /// 00158
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00164
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00168
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0xcb000000,                                                  // -8388608.0                                  /// 00170
			 0x7fc00001,                                                  // signaling NaN                               /// 00174
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0017c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00184
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x33333333,                                                  // 4 random values                             /// 0018c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00190
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x00000000,                                                  // zero                                        /// 00198
			 0x0c600000,                                                  // Leading 1s:                                 /// 0019c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001a0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001a4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001a8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001ac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001b0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001b4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001bc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 001c4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001c8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 001cc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001d0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001d4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001d8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001dc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 001e4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 001e8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001ec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001f0
			 0x3f028f5c,                                                  // 0.51                                        /// 001f4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001fc
			 0xff800000,                                                  // -inf                                        /// 00200
			 0x0c780000,                                                  // Leading 1s:                                 /// 00204
			 0x0e000001,                                                  // Trailing 1s:                                /// 00208
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x4b000000,                                                  // 8388608.0                                   /// 00210
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00214
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00218
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0021c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00220
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00224
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00228
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0022c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00230
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00238
			 0x55555555,                                                  // 4 random values                             /// 0023c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00240
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00244
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00248
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0024c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00250
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00258
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0xbf028f5c,                                                  // -0.51                                       /// 00260
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00268
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0026c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00270
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00274
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00278
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0027c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00280
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00284
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0028c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00290
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00294
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00298
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 002a0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002a4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002ac
			 0x00000000,                                                  // zero                                        /// 002b0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 002b8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002c0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002c4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002c8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002cc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002d0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002d4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002dc
			 0xffc00001,                                                  // -signaling NaN                              /// 002e0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002e4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002e8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002ec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 002f4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 002f8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 002fc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00300
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00308
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0030c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00310
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00314
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00318
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00324
			 0x0e000001,                                                  // Trailing 1s:                                /// 00328
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0032c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00330
			 0xcb000000,                                                  // -8388608.0                                  /// 00334
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0033c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00340
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0034c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00350
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00358
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00360
			 0x0c400000,                                                  // Leading 1s:                                 /// 00364
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00368
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0036c
			 0x33333333,                                                  // 4 random values                             /// 00370
			 0x00000000,                                                  // zero                                        /// 00374
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00378
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0037c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00380
			 0x00000000,                                                  // zero                                        /// 00384
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00388
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0038c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00390
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00394
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0039c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003a0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003a4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003a8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003ac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003b0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003b4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003b8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003c0
			 0x0c700000,                                                  // Leading 1s:                                 /// 003c4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003c8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003cc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003d8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003dc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003e0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003e8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 003ec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003f0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003f4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003f8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00400
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00404
			 0x0c780000,                                                  // Leading 1s:                                 /// 00408
			 0x0e000007,                                                  // Trailing 1s:                                /// 0040c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00410
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0041c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00424
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00430
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00434
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00438
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0043c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00440
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00444
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00448
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00450
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00454
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00458
			 0x0e000001,                                                  // Trailing 1s:                                /// 0045c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00460
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00464
			 0xbf028f5c,                                                  // -0.51                                       /// 00468
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0046c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00470
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00474
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00478
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0047c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00480
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00484
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0048c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00490
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00494
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00498
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0049c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 004a4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004a8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004ac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004b0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004b8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004bc
			 0x0c600000,                                                  // Leading 1s:                                 /// 004c0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004c4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004c8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004cc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004d0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004d4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 004dc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x80000000,                                                  // -zero                                       /// 004e8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004f0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 004f4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004fc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00500
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00504
			 0x7fc00001,                                                  // signaling NaN                               /// 00508
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0050c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00510
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00514
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0051c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00520
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00524
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00528
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0052c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00530
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00534
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00538
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00540
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0054c
			 0x00000000,                                                  // zero                                        /// 00550
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00558
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0055c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00560
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00568
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0056c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00570
			 0x0e000007,                                                  // Trailing 1s:                                /// 00574
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00578
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0057c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00580
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00584
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0058c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00594
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00598
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0059c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005a0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005a4
			 0x3f028f5c,                                                  // 0.51                                        /// 005a8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005ac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005b4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005b8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005bc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005c0
			 0x0e000007,                                                  // Trailing 1s:                                /// 005c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005c8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 005d4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 005e4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005e8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005ec
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005f0
			 0x00000000,                                                  // zero                                        /// 005f4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005f8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005fc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00604
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0060c
			 0x7f800000,                                                  // inf                                         /// 00610
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00614
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00620
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00624
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00628
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00634
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00638
			 0x4b000000,                                                  // 8388608.0                                   /// 0063c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00640
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00644
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00648
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00650
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00654
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00658
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0065c
			 0x7f800000,                                                  // inf                                         /// 00660
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00664
			 0x0c400000,                                                  // Leading 1s:                                 /// 00668
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00670
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00674
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00678
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00684
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00688
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0068c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00694
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0069c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006a0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006a8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006b0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006b4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006b8
			 0x00000000,                                                  // zero                                        /// 006bc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006c0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006c4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006c8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x0c700000,                                                  // Leading 1s:                                 /// 006d0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006d4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006dc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006e0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0xff800000,                                                  // -inf                                        /// 006e8
			 0xffc00001,                                                  // -signaling NaN                              /// 006ec
			 0x00011111,                                                  // 9.7958E-41                                  /// 006f0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 006f4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006f8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006fc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00700
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00704
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0070c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00714
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00718
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00724
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00728
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00730
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00738
			 0xbf028f5c,                                                  // -0.51                                       /// 0073c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00740
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00744
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00748
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0074c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00750
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x3f028f5c,                                                  // 0.51                                        /// 00758
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0075c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00764
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00768
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0076c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00770
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00778
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0077c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00780
			 0x33333333,                                                  // 4 random values                             /// 00784
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x0e000001,                                                  // Trailing 1s:                                /// 0078c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00790
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00798
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0079c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007a0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007a4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007a8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 007b0
			 0x7f800000,                                                  // inf                                         /// 007b4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007b8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007c4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007c8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007cc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007d0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007e0
			 0x7fc00001,                                                  // signaling NaN                               /// 007e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007e8
			 0x0e000003,                                                  // Trailing 1s:                                /// 007ec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007f0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007fc
			 0xcb000000,                                                  // -8388608.0                                  /// 00800
			 0xbf028f5c,                                                  // -0.51                                       /// 00804
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00808
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0080c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00810
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00814
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00818
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0081c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00828
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0082c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00830
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00834
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00838
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0083c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00844
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00848
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00854
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00858
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0085c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00860
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00864
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00868
			 0x4b000000,                                                  // 8388608.0                                   /// 0086c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00874
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00878
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00880
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00884
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00888
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00890
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00894
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00898
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0089c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008a4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 008b0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008b4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008b8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008bc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008c0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008c8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008cc
			 0xff800000,                                                  // -inf                                        /// 008d0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008d4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008dc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008e0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008e4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008e8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008f4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008f8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008fc
			 0x7fc00001,                                                  // signaling NaN                               /// 00900
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00904
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0090c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00910
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00918
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0091c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00920
			 0x00000000,                                                  // zero                                        /// 00924
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00928
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0092c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00930
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00938
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0093c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00940
			 0x3f028f5c,                                                  // 0.51                                        /// 00944
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00948
			 0xff800000,                                                  // -inf                                        /// 0094c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00954
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00958
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0095c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00960
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00968
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00970
			 0x33333333,                                                  // 4 random values                             /// 00974
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00978
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0097c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00984
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00988
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0098c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00990
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00994
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00998
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0099c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 009a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009ac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009b0
			 0xbf028f5c,                                                  // -0.51                                       /// 009b4
			 0x0c700000,                                                  // Leading 1s:                                 /// 009b8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009bc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 009c0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009c4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009c8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009cc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009d4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 009d8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009dc
			 0x00000000,                                                  // zero                                        /// 009e0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009e8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 009ec
			 0x80011111,                                                  // -9.7958E-41                                 /// 009f0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a04
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a08
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a0c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a14
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a18
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a20
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a24
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a28
			 0x33333333,                                                  // 4 random values                             /// 00a2c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a30
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a34
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a38
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a3c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a40
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a44
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a48
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a4c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a5c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a60
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a68
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a6c
			 0x4b000000,                                                  // 8388608.0                                   /// 00a70
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a74
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a78
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a7c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a80
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a88
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a8c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a90
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a94
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a98
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a9c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00aa0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00aa8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00aac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ab0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00abc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ac0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ac4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ac8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00acc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ad0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ad4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ae0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ae4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ae8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00aec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00af0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00af8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00afc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b00
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x33333333,                                                  // 4 random values                             /// 00b08
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b0c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b10
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b14
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b18
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b1c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b20
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b24
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b28
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b2c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b30
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b34
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b38
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b3c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00b40
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b44
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b4c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b50
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b54
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b5c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b60
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b64
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b68
			 0xbf028f5c,                                                  // -0.51                                       /// 00b6c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b70
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b74
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b78
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b80
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b84
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b88
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b8c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b90
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b94
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b98
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b9c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ba4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ba8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bb0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bbc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bc0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bc8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bcc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bd0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bd4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bd8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bdc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00be0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00be4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bec
			 0x4b000000,                                                  // 8388608.0                                   /// 00bf0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bf4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bf8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c08
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c0c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x55555555,                                                  // 4 random values                             /// 00c14
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c18
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c1c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c20
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c24
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c2c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c30
			 0x7fc00001,                                                  // signaling NaN                               /// 00c34
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c38
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c44
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c48
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c50
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c54
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c58
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c5c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00c60
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c68
			 0xcb000000,                                                  // -8388608.0                                  /// 00c6c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c70
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c78
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c7c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c80
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c84
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00c88
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c8c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c90
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c98
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c9c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ca4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ca8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cbc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ccc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00cd4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cd8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cdc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ce0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ce8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cf0
			 0x80000000,                                                  // -zero                                       /// 00cf4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x55555555,                                                  // 4 random values                             /// 00cfc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d00
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d04
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d08
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d0c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d10
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d14
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d18
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d20
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d24
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d28
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d2c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d34
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d40
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d44
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d48
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d4c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d50
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d54
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d5c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d60
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0xff800000,                                                  // -inf                                        /// 00d68
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d70
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d74
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d7c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d80
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d84
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d88
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d98
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00d9c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00da0
			 0xff800000,                                                  // -inf                                        /// 00da4
			 0x7fc00001,                                                  // signaling NaN                               /// 00da8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00dac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00db0
			 0xffc00001,                                                  // -signaling NaN                              /// 00db4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00db8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00dbc
			 0xbf028f5c,                                                  // -0.51                                       /// 00dc0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00dc4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00dcc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00dd0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ddc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00de0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00dec
			 0x0c700000,                                                  // Leading 1s:                                 /// 00df0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00df4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00df8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dfc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e00
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e04
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e08
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e0c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00e10
			 0x55555555,                                                  // 4 random values                             /// 00e14
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e18
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e1c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e20
			 0xffc00001,                                                  // -signaling NaN                              /// 00e24
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e2c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e30
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e34
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e38
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e3c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e40
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e44
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e48
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e4c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e50
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e54
			 0x33333333,                                                  // 4 random values                             /// 00e58
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e60
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e64
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e68
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e6c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e70
			 0x3f028f5c,                                                  // 0.51                                        /// 00e74
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e78
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e7c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e80
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e84
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e8c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e90
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e94
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e98
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00e9c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ea0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ea4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00eac
			 0x7fc00001,                                                  // signaling NaN                               /// 00eb0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00eb4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ebc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ec0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ec4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ec8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ed0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00edc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ee0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ee4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ee8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ef0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ef4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x4b000000,                                                  // 8388608.0                                   /// 00efc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f00
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f04
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f08
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f0c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f10
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f14
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f1c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f20
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f2c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f34
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f38
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f3c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f40
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f44
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f48
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f4c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f50
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f54
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f5c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f60
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f68
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f6c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f78
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f7c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f84
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f8c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f94
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f98
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fa0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fac
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fb0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fb4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fbc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fc0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fc8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fcc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fdc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00fe4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fe8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ff8
			 0x0c7fff80                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x084bd1aa, /// 0x0
			 0x9773904b, /// 0x4
			 0x3afa60eb, /// 0x8
			 0x992ecac9, /// 0xc
			 0x3305e028, /// 0x10
			 0x4411995f, /// 0x14
			 0xdec1b6e9, /// 0x18
			 0xd0562187, /// 0x1c
			 0x9568acc9, /// 0x20
			 0x2e09b417, /// 0x24
			 0x22129301, /// 0x28
			 0x4317a145, /// 0x2c
			 0xeb7abf6e, /// 0x30
			 0xeaa1f881, /// 0x34
			 0x9bd9fb10, /// 0x38
			 0x7a5f64bc, /// 0x3c
			 0x3c87d315, /// 0x40
			 0x1b5d7e6c, /// 0x44
			 0x0468909f, /// 0x48
			 0xb4f12047, /// 0x4c
			 0x25d47614, /// 0x50
			 0x23255341, /// 0x54
			 0x17023bf0, /// 0x58
			 0xf64fa0ce, /// 0x5c
			 0xff8bcd1d, /// 0x60
			 0x606e16f4, /// 0x64
			 0xd264201e, /// 0x68
			 0x61832ed3, /// 0x6c
			 0xd986c731, /// 0x70
			 0x2aa256ea, /// 0x74
			 0x70ae3992, /// 0x78
			 0x81ce5085, /// 0x7c
			 0x694b80ec, /// 0x80
			 0x70d8c440, /// 0x84
			 0x8adf3a7b, /// 0x88
			 0xb360927b, /// 0x8c
			 0x976daba2, /// 0x90
			 0xd6fd06cf, /// 0x94
			 0xf63b45b2, /// 0x98
			 0x381085d1, /// 0x9c
			 0x9bb0adaa, /// 0xa0
			 0x4662274c, /// 0xa4
			 0x186eb714, /// 0xa8
			 0xf62bdbc6, /// 0xac
			 0x3242c4a3, /// 0xb0
			 0x23822067, /// 0xb4
			 0x4800e9c4, /// 0xb8
			 0xa049a170, /// 0xbc
			 0xa7f6c368, /// 0xc0
			 0x52c7ee8e, /// 0xc4
			 0x95a7c4f9, /// 0xc8
			 0x2aef21da, /// 0xcc
			 0x2d3a7a14, /// 0xd0
			 0x4f5b03cf, /// 0xd4
			 0xec7b4759, /// 0xd8
			 0x834ff8ee, /// 0xdc
			 0x01906ad6, /// 0xe0
			 0x0590e0b7, /// 0xe4
			 0xc913769e, /// 0xe8
			 0x8b019ce4, /// 0xec
			 0x92b5b1e8, /// 0xf0
			 0x4981d474, /// 0xf4
			 0x2ab0d3d3, /// 0xf8
			 0x75bca3e0, /// 0xfc
			 0xc867f634, /// 0x100
			 0xa8a44a54, /// 0x104
			 0x2026a4fd, /// 0x108
			 0x38c455b1, /// 0x10c
			 0x05bc56d7, /// 0x110
			 0x8c32e651, /// 0x114
			 0x73cb6a4e, /// 0x118
			 0x7be731b1, /// 0x11c
			 0x53fd24c3, /// 0x120
			 0x8d3f3385, /// 0x124
			 0x4ba77e87, /// 0x128
			 0x0afda4ac, /// 0x12c
			 0xcef36969, /// 0x130
			 0x2922327b, /// 0x134
			 0x009d7196, /// 0x138
			 0x1baa34fe, /// 0x13c
			 0x1aef337a, /// 0x140
			 0xc0ae26f7, /// 0x144
			 0x174cc413, /// 0x148
			 0x7ba1e4db, /// 0x14c
			 0xc377ea25, /// 0x150
			 0x1e9922c5, /// 0x154
			 0x0dca94fe, /// 0x158
			 0xa5082ae3, /// 0x15c
			 0x50aafc5f, /// 0x160
			 0x37fb6fcf, /// 0x164
			 0x6382a68d, /// 0x168
			 0x8903dc1c, /// 0x16c
			 0x1523ed19, /// 0x170
			 0x08427712, /// 0x174
			 0x7de5be4f, /// 0x178
			 0xd0a6e102, /// 0x17c
			 0xd1cd7aa1, /// 0x180
			 0xd355bbce, /// 0x184
			 0xa3de445a, /// 0x188
			 0x6c69462c, /// 0x18c
			 0xee9daed6, /// 0x190
			 0xdfd458e6, /// 0x194
			 0xc7843cf3, /// 0x198
			 0x867b318a, /// 0x19c
			 0x401e69f9, /// 0x1a0
			 0x0048d97f, /// 0x1a4
			 0x7edcfc16, /// 0x1a8
			 0xa974932a, /// 0x1ac
			 0x05a783dc, /// 0x1b0
			 0xdb68bdf6, /// 0x1b4
			 0x7a8fb57e, /// 0x1b8
			 0x55396a22, /// 0x1bc
			 0x5c8da5fa, /// 0x1c0
			 0xc30f9d1b, /// 0x1c4
			 0x2dd15d33, /// 0x1c8
			 0x3704fe68, /// 0x1cc
			 0xd821fc64, /// 0x1d0
			 0x25d37502, /// 0x1d4
			 0xe5d7c65e, /// 0x1d8
			 0x815977b1, /// 0x1dc
			 0xde0e9781, /// 0x1e0
			 0x5f9708be, /// 0x1e4
			 0xb9222cb7, /// 0x1e8
			 0x86048b12, /// 0x1ec
			 0x283bbbbd, /// 0x1f0
			 0x98ad2acf, /// 0x1f4
			 0x2bbf2d8a, /// 0x1f8
			 0x83503990, /// 0x1fc
			 0x23050ddd, /// 0x200
			 0xd57f1cd7, /// 0x204
			 0x571d5e51, /// 0x208
			 0x4414522b, /// 0x20c
			 0x4bf51efa, /// 0x210
			 0x5282bac7, /// 0x214
			 0xf855cf00, /// 0x218
			 0x9c45d7e7, /// 0x21c
			 0x58268905, /// 0x220
			 0xf54b104f, /// 0x224
			 0x05ca3321, /// 0x228
			 0x1a8e7f7d, /// 0x22c
			 0xb5baa9de, /// 0x230
			 0x50bc3093, /// 0x234
			 0x0559e724, /// 0x238
			 0x4b0aa283, /// 0x23c
			 0x141d8066, /// 0x240
			 0x5d17a7f8, /// 0x244
			 0xcb17e39a, /// 0x248
			 0xf8160478, /// 0x24c
			 0x718e5d32, /// 0x250
			 0x8742d529, /// 0x254
			 0xa80ffb52, /// 0x258
			 0xc048775d, /// 0x25c
			 0x7315c3d2, /// 0x260
			 0x4951f81f, /// 0x264
			 0x2a445c5c, /// 0x268
			 0xce5ef31b, /// 0x26c
			 0x389d8a67, /// 0x270
			 0xf9179080, /// 0x274
			 0xcca9332a, /// 0x278
			 0xbd9b087f, /// 0x27c
			 0x6c8a8cff, /// 0x280
			 0x61bce4e3, /// 0x284
			 0x2e3d6a0f, /// 0x288
			 0x64909bd8, /// 0x28c
			 0xfe22f53d, /// 0x290
			 0xd7ba9c5e, /// 0x294
			 0x09a57125, /// 0x298
			 0x4357fd44, /// 0x29c
			 0xb1cbf53e, /// 0x2a0
			 0x57c9b1c4, /// 0x2a4
			 0xc5bc85f5, /// 0x2a8
			 0x71e75d0d, /// 0x2ac
			 0x91692061, /// 0x2b0
			 0xe11fa099, /// 0x2b4
			 0x1e0cf34e, /// 0x2b8
			 0xdd0df43b, /// 0x2bc
			 0xb0367b0d, /// 0x2c0
			 0x02c03625, /// 0x2c4
			 0x0abe424b, /// 0x2c8
			 0x212051c3, /// 0x2cc
			 0x3a2b3584, /// 0x2d0
			 0x605ab0c8, /// 0x2d4
			 0x78cfa09c, /// 0x2d8
			 0x8964c50b, /// 0x2dc
			 0x7d764ba0, /// 0x2e0
			 0x0e4e6917, /// 0x2e4
			 0x371324dc, /// 0x2e8
			 0xa5bef248, /// 0x2ec
			 0x21566e4e, /// 0x2f0
			 0x24dd7453, /// 0x2f4
			 0x6c6ae91b, /// 0x2f8
			 0xcc740ce0, /// 0x2fc
			 0xd0eb676c, /// 0x300
			 0x398aad20, /// 0x304
			 0x65502d31, /// 0x308
			 0xf44c173c, /// 0x30c
			 0xc54169d5, /// 0x310
			 0xab42f9f1, /// 0x314
			 0xd90c6984, /// 0x318
			 0xb218f268, /// 0x31c
			 0x408c58d7, /// 0x320
			 0x1a070889, /// 0x324
			 0x553f8b33, /// 0x328
			 0x0b2330b1, /// 0x32c
			 0x63f4b950, /// 0x330
			 0x1fa15849, /// 0x334
			 0x3763280e, /// 0x338
			 0xbe607abb, /// 0x33c
			 0xe8fffcbc, /// 0x340
			 0x1d348a60, /// 0x344
			 0x5453ca6b, /// 0x348
			 0xbcb766c3, /// 0x34c
			 0xda043b7d, /// 0x350
			 0xaf01b79e, /// 0x354
			 0x11cccd09, /// 0x358
			 0x8c5229a8, /// 0x35c
			 0x392265ac, /// 0x360
			 0xe412e00f, /// 0x364
			 0x846a8211, /// 0x368
			 0x9c677da4, /// 0x36c
			 0x735076de, /// 0x370
			 0xa707a29c, /// 0x374
			 0x99448c70, /// 0x378
			 0xcdc1c57f, /// 0x37c
			 0x6cefac61, /// 0x380
			 0x5b9ac342, /// 0x384
			 0x2a5433d2, /// 0x388
			 0x65b5ec92, /// 0x38c
			 0xfb8918dd, /// 0x390
			 0x9a484020, /// 0x394
			 0xb4e7a572, /// 0x398
			 0x27ca9b6e, /// 0x39c
			 0x5d250c93, /// 0x3a0
			 0x51dd4b5d, /// 0x3a4
			 0x19c35805, /// 0x3a8
			 0xaab07862, /// 0x3ac
			 0xae649348, /// 0x3b0
			 0xef014418, /// 0x3b4
			 0x3c5f9906, /// 0x3b8
			 0x77100295, /// 0x3bc
			 0x11125a23, /// 0x3c0
			 0xe361af33, /// 0x3c4
			 0x77edbc50, /// 0x3c8
			 0x50d936d0, /// 0x3cc
			 0x5618c0ab, /// 0x3d0
			 0xc8f2191c, /// 0x3d4
			 0x4d61ee51, /// 0x3d8
			 0xc8a41a45, /// 0x3dc
			 0xcd842307, /// 0x3e0
			 0x5105bd17, /// 0x3e4
			 0x0d979d11, /// 0x3e8
			 0xa1c99d4f, /// 0x3ec
			 0xab4ead75, /// 0x3f0
			 0xe2e50c02, /// 0x3f4
			 0x9401e82d, /// 0x3f8
			 0x0e6ec580, /// 0x3fc
			 0x42e829c1, /// 0x400
			 0xda993c70, /// 0x404
			 0x57c81f1e, /// 0x408
			 0xc7fb7c29, /// 0x40c
			 0xe2ed3105, /// 0x410
			 0x95e78b5b, /// 0x414
			 0xeb32f963, /// 0x418
			 0x698d7cef, /// 0x41c
			 0xc88167b4, /// 0x420
			 0xce05fc8c, /// 0x424
			 0x608960a9, /// 0x428
			 0xc934fd75, /// 0x42c
			 0xb7d22998, /// 0x430
			 0x7475442b, /// 0x434
			 0x9d6e2dcf, /// 0x438
			 0xcc2eeaaa, /// 0x43c
			 0xa11be641, /// 0x440
			 0x6ebbf468, /// 0x444
			 0x2a3c4132, /// 0x448
			 0x30cb7711, /// 0x44c
			 0xbdddd286, /// 0x450
			 0x9d44a6cd, /// 0x454
			 0x11785313, /// 0x458
			 0xa249acf2, /// 0x45c
			 0x0c1eb736, /// 0x460
			 0xac708d0a, /// 0x464
			 0x61f3daf9, /// 0x468
			 0x5b443935, /// 0x46c
			 0xd447fb0a, /// 0x470
			 0xaaf9dec5, /// 0x474
			 0xcf299a5f, /// 0x478
			 0x2d6fd99a, /// 0x47c
			 0x3dcac854, /// 0x480
			 0x6a4c5f58, /// 0x484
			 0x0d63d1b1, /// 0x488
			 0x5423e7f3, /// 0x48c
			 0x2c455092, /// 0x490
			 0xebdf2252, /// 0x494
			 0xa185f668, /// 0x498
			 0x776d7012, /// 0x49c
			 0x26d8375d, /// 0x4a0
			 0x368b209d, /// 0x4a4
			 0x1b178d45, /// 0x4a8
			 0x8c9f21e0, /// 0x4ac
			 0xd395b153, /// 0x4b0
			 0x4a821d22, /// 0x4b4
			 0x18594314, /// 0x4b8
			 0x7a9086c5, /// 0x4bc
			 0xac0458a7, /// 0x4c0
			 0x192d113e, /// 0x4c4
			 0xafafda6e, /// 0x4c8
			 0x2739917b, /// 0x4cc
			 0xd54bf8e9, /// 0x4d0
			 0x53ffcf91, /// 0x4d4
			 0xadc219c4, /// 0x4d8
			 0x4c983352, /// 0x4dc
			 0x792b37ff, /// 0x4e0
			 0x73d65cb9, /// 0x4e4
			 0xc16c5331, /// 0x4e8
			 0x22ed7c87, /// 0x4ec
			 0xc09b3890, /// 0x4f0
			 0xa7605193, /// 0x4f4
			 0x90b6dd49, /// 0x4f8
			 0xd9fee4b0, /// 0x4fc
			 0x7c92c6b5, /// 0x500
			 0xd9e07502, /// 0x504
			 0x80a423e6, /// 0x508
			 0xb7b41f89, /// 0x50c
			 0xa2ef8d93, /// 0x510
			 0x59395b84, /// 0x514
			 0xd2f0b79c, /// 0x518
			 0x870175dc, /// 0x51c
			 0xe292d2f4, /// 0x520
			 0x2e69d6fb, /// 0x524
			 0x9c83301c, /// 0x528
			 0x41f98130, /// 0x52c
			 0x331a10f5, /// 0x530
			 0x7557f7ad, /// 0x534
			 0x22b48b81, /// 0x538
			 0x067107cc, /// 0x53c
			 0xdd4fbe25, /// 0x540
			 0x630c204b, /// 0x544
			 0xfe9513ef, /// 0x548
			 0x6710bf73, /// 0x54c
			 0x6bcb7ce1, /// 0x550
			 0xb5dc7a71, /// 0x554
			 0x522aef8f, /// 0x558
			 0x284c14a3, /// 0x55c
			 0xaeeae017, /// 0x560
			 0x05799c79, /// 0x564
			 0x2eef58cb, /// 0x568
			 0x3a6a6637, /// 0x56c
			 0xcab91b3b, /// 0x570
			 0xc1ca5958, /// 0x574
			 0x3eb64701, /// 0x578
			 0x5bbae3ef, /// 0x57c
			 0xaeee665d, /// 0x580
			 0x3f2303bc, /// 0x584
			 0x3cd9078b, /// 0x588
			 0x4a58a663, /// 0x58c
			 0x3f9a859f, /// 0x590
			 0xe272bf63, /// 0x594
			 0xfee4ec3d, /// 0x598
			 0x79eb331e, /// 0x59c
			 0xe17e4370, /// 0x5a0
			 0x32592301, /// 0x5a4
			 0x051504d1, /// 0x5a8
			 0x1726db0a, /// 0x5ac
			 0xb9d23a70, /// 0x5b0
			 0x023d4ed5, /// 0x5b4
			 0x3b7850a8, /// 0x5b8
			 0x0420d9c9, /// 0x5bc
			 0x6e29239f, /// 0x5c0
			 0x5717d996, /// 0x5c4
			 0x5155179e, /// 0x5c8
			 0xc1c0991a, /// 0x5cc
			 0x5a3f553a, /// 0x5d0
			 0x75950b6b, /// 0x5d4
			 0x9eafd9b7, /// 0x5d8
			 0xe909a711, /// 0x5dc
			 0x6bc8da4a, /// 0x5e0
			 0x527a70e7, /// 0x5e4
			 0xa57fe69c, /// 0x5e8
			 0x38550707, /// 0x5ec
			 0xd677c4d6, /// 0x5f0
			 0x1d95a33f, /// 0x5f4
			 0x1e236845, /// 0x5f8
			 0xf854fb8a, /// 0x5fc
			 0x8171535d, /// 0x600
			 0x1a623fc5, /// 0x604
			 0x36806f0d, /// 0x608
			 0xfd2dfb90, /// 0x60c
			 0x67cf981f, /// 0x610
			 0x2493ae49, /// 0x614
			 0x140398ad, /// 0x618
			 0x824bf09f, /// 0x61c
			 0x561c1491, /// 0x620
			 0xfed46b43, /// 0x624
			 0xb958b897, /// 0x628
			 0xe660e9af, /// 0x62c
			 0x0846d55b, /// 0x630
			 0x7bdb272b, /// 0x634
			 0x5bd4be60, /// 0x638
			 0x94b9145a, /// 0x63c
			 0x7b21766f, /// 0x640
			 0xf37b3aff, /// 0x644
			 0x8d2521bc, /// 0x648
			 0x1afb4549, /// 0x64c
			 0xf2af76a6, /// 0x650
			 0x9cf4dcfd, /// 0x654
			 0xc66b3771, /// 0x658
			 0x7011e002, /// 0x65c
			 0x210b276a, /// 0x660
			 0x51cebc34, /// 0x664
			 0xc202c6eb, /// 0x668
			 0x6cc92c1e, /// 0x66c
			 0x80112f8b, /// 0x670
			 0x9cc070e4, /// 0x674
			 0x98504dc9, /// 0x678
			 0x93215f6c, /// 0x67c
			 0x178721b0, /// 0x680
			 0x6c4e8c5e, /// 0x684
			 0x1571e49d, /// 0x688
			 0xdd406a32, /// 0x68c
			 0xb71d3bd6, /// 0x690
			 0x87d50ed5, /// 0x694
			 0xadbbb3c5, /// 0x698
			 0x8c15630d, /// 0x69c
			 0xb0632c8e, /// 0x6a0
			 0xebc0d5ae, /// 0x6a4
			 0x9f3f2fe9, /// 0x6a8
			 0x9f7c3873, /// 0x6ac
			 0x89fe979c, /// 0x6b0
			 0x1172e7bb, /// 0x6b4
			 0x01a477e7, /// 0x6b8
			 0x6d1ec506, /// 0x6bc
			 0x1787e830, /// 0x6c0
			 0x59c58b5f, /// 0x6c4
			 0x9d5c67e9, /// 0x6c8
			 0x83da3860, /// 0x6cc
			 0xba0f68d0, /// 0x6d0
			 0xd92b4768, /// 0x6d4
			 0x863a1932, /// 0x6d8
			 0x24f33c76, /// 0x6dc
			 0x525b2c7c, /// 0x6e0
			 0x7b2bfa91, /// 0x6e4
			 0x6cd85017, /// 0x6e8
			 0xeab56e47, /// 0x6ec
			 0xa8a390a7, /// 0x6f0
			 0x6a1f15cb, /// 0x6f4
			 0xc8296550, /// 0x6f8
			 0xe5ae77b3, /// 0x6fc
			 0x728ebfc1, /// 0x700
			 0x4b0eafac, /// 0x704
			 0xe866fcbc, /// 0x708
			 0x5749e69a, /// 0x70c
			 0xbbd50ea4, /// 0x710
			 0xcfdda2d2, /// 0x714
			 0xd8d6bf01, /// 0x718
			 0xb330d82a, /// 0x71c
			 0x642a3770, /// 0x720
			 0x0a7bb595, /// 0x724
			 0x2cd440de, /// 0x728
			 0x9e294016, /// 0x72c
			 0x294d3bf5, /// 0x730
			 0x44371426, /// 0x734
			 0x862f9894, /// 0x738
			 0x1878e53b, /// 0x73c
			 0x0cae585d, /// 0x740
			 0x5c364e36, /// 0x744
			 0x0f196384, /// 0x748
			 0xefb5b034, /// 0x74c
			 0xe3467b8d, /// 0x750
			 0x286ee2ba, /// 0x754
			 0xe87a767b, /// 0x758
			 0xe9a8290d, /// 0x75c
			 0x98e26b89, /// 0x760
			 0x04db8d95, /// 0x764
			 0xc372fd08, /// 0x768
			 0x6c2c107f, /// 0x76c
			 0x69342da3, /// 0x770
			 0x5797dfd6, /// 0x774
			 0x281ccf99, /// 0x778
			 0x2d188f75, /// 0x77c
			 0xde849909, /// 0x780
			 0x25c1eb62, /// 0x784
			 0x38959a20, /// 0x788
			 0xf724c540, /// 0x78c
			 0x29a6d045, /// 0x790
			 0x7f3d1f4a, /// 0x794
			 0x4bd9d64e, /// 0x798
			 0xed0fbf29, /// 0x79c
			 0xb9d71d91, /// 0x7a0
			 0x4a7bad28, /// 0x7a4
			 0x353a25d1, /// 0x7a8
			 0x28adaf7a, /// 0x7ac
			 0x56d7785f, /// 0x7b0
			 0x60c89398, /// 0x7b4
			 0x7c2cfa66, /// 0x7b8
			 0xd76fe80e, /// 0x7bc
			 0x7344588b, /// 0x7c0
			 0x7c1e7227, /// 0x7c4
			 0x1a6c9825, /// 0x7c8
			 0xc15c3399, /// 0x7cc
			 0x358bccd3, /// 0x7d0
			 0x2254c728, /// 0x7d4
			 0x36fd8364, /// 0x7d8
			 0xb518674c, /// 0x7dc
			 0xf1319ad2, /// 0x7e0
			 0x86a13a26, /// 0x7e4
			 0xe551b8c1, /// 0x7e8
			 0x6bb1ec5b, /// 0x7ec
			 0xb87d8805, /// 0x7f0
			 0x5ac6e170, /// 0x7f4
			 0x59783d88, /// 0x7f8
			 0x6f84031c, /// 0x7fc
			 0xd2f3cda1, /// 0x800
			 0xcc9168cf, /// 0x804
			 0xcc05dbdb, /// 0x808
			 0x0fdf84cc, /// 0x80c
			 0xfeb14d3a, /// 0x810
			 0xa059a1df, /// 0x814
			 0x8dda9369, /// 0x818
			 0x862b63cb, /// 0x81c
			 0xd3f30d2f, /// 0x820
			 0x85a400af, /// 0x824
			 0xcec23eb3, /// 0x828
			 0x8334f940, /// 0x82c
			 0x84750b52, /// 0x830
			 0x5a23641b, /// 0x834
			 0x91d8023b, /// 0x838
			 0x1034fc2f, /// 0x83c
			 0xd69a0d4e, /// 0x840
			 0x46fc7048, /// 0x844
			 0x335f0cb4, /// 0x848
			 0x6db7dca6, /// 0x84c
			 0xd6a6bf03, /// 0x850
			 0xe059204d, /// 0x854
			 0x5e5b62e4, /// 0x858
			 0x63547eaf, /// 0x85c
			 0xac55bad7, /// 0x860
			 0x7dd28ff8, /// 0x864
			 0x497ac0c1, /// 0x868
			 0xcb79e401, /// 0x86c
			 0x721e7e2d, /// 0x870
			 0x8e27cde5, /// 0x874
			 0xa40397c6, /// 0x878
			 0x298c850c, /// 0x87c
			 0xa6cc7837, /// 0x880
			 0x7969c359, /// 0x884
			 0xe02efa49, /// 0x888
			 0xa0f2fd29, /// 0x88c
			 0x29aed3db, /// 0x890
			 0x63689f25, /// 0x894
			 0x153cb538, /// 0x898
			 0x95fe08df, /// 0x89c
			 0x409a26cf, /// 0x8a0
			 0xf66fcbcd, /// 0x8a4
			 0x3634c87e, /// 0x8a8
			 0xa71c1029, /// 0x8ac
			 0xdb81e394, /// 0x8b0
			 0xc397121e, /// 0x8b4
			 0x474b869e, /// 0x8b8
			 0x9d2c3ce6, /// 0x8bc
			 0x64c51935, /// 0x8c0
			 0xc185dfbf, /// 0x8c4
			 0x9f0f7a8d, /// 0x8c8
			 0xcb2d001f, /// 0x8cc
			 0xb6ebb07e, /// 0x8d0
			 0x0d3e5524, /// 0x8d4
			 0xe640452b, /// 0x8d8
			 0xe5709468, /// 0x8dc
			 0xe22812d1, /// 0x8e0
			 0x61aa329a, /// 0x8e4
			 0x0bef94d8, /// 0x8e8
			 0x6f223921, /// 0x8ec
			 0x85f46dd8, /// 0x8f0
			 0xeae93e7c, /// 0x8f4
			 0xf23c6f2e, /// 0x8f8
			 0x076dbde8, /// 0x8fc
			 0x6c3ebb4d, /// 0x900
			 0xab294d1e, /// 0x904
			 0x77e917d7, /// 0x908
			 0x450814f9, /// 0x90c
			 0x9660f2f1, /// 0x910
			 0x7f3b571b, /// 0x914
			 0x2aa6a57f, /// 0x918
			 0xc22605d5, /// 0x91c
			 0x8dd5cde7, /// 0x920
			 0x1cdd72e8, /// 0x924
			 0xe1663c02, /// 0x928
			 0x39a99374, /// 0x92c
			 0x78b3e02b, /// 0x930
			 0xb1649a6c, /// 0x934
			 0xbb9d9555, /// 0x938
			 0x502e945e, /// 0x93c
			 0x1ac1dbed, /// 0x940
			 0x3f246a8f, /// 0x944
			 0x3a0d264c, /// 0x948
			 0xcfd40bd3, /// 0x94c
			 0x956dd5e2, /// 0x950
			 0x1444fdd2, /// 0x954
			 0x00295289, /// 0x958
			 0x02643aac, /// 0x95c
			 0x4ee77a4c, /// 0x960
			 0xff60708c, /// 0x964
			 0xf4799033, /// 0x968
			 0x383a0f32, /// 0x96c
			 0xede89b80, /// 0x970
			 0x510158fe, /// 0x974
			 0xfb8daebc, /// 0x978
			 0xae9178db, /// 0x97c
			 0xaa9252d4, /// 0x980
			 0x291db90c, /// 0x984
			 0xfec4d369, /// 0x988
			 0x7239f402, /// 0x98c
			 0x0b28831b, /// 0x990
			 0x03db8f00, /// 0x994
			 0x55932104, /// 0x998
			 0x7e4efc8a, /// 0x99c
			 0x7e8cd5eb, /// 0x9a0
			 0xee905989, /// 0x9a4
			 0x930ba7db, /// 0x9a8
			 0x80d45931, /// 0x9ac
			 0x994a241d, /// 0x9b0
			 0x50b96673, /// 0x9b4
			 0xd8e0d125, /// 0x9b8
			 0x8eb0ad63, /// 0x9bc
			 0x3cbeff95, /// 0x9c0
			 0x093582d9, /// 0x9c4
			 0x3f633251, /// 0x9c8
			 0x4f069bd8, /// 0x9cc
			 0x68f7dd51, /// 0x9d0
			 0x91e9df47, /// 0x9d4
			 0xfcd34282, /// 0x9d8
			 0x61957141, /// 0x9dc
			 0xc5292eb4, /// 0x9e0
			 0x9f1a79f7, /// 0x9e4
			 0xfe2034d8, /// 0x9e8
			 0x997fe4bc, /// 0x9ec
			 0x87ae4c5d, /// 0x9f0
			 0x80e42200, /// 0x9f4
			 0x1aed0bc7, /// 0x9f8
			 0x977aebd1, /// 0x9fc
			 0x346a1cca, /// 0xa00
			 0x9bee2477, /// 0xa04
			 0x9d121fb5, /// 0xa08
			 0x5574e5c9, /// 0xa0c
			 0xaf937fb1, /// 0xa10
			 0x6b6aef49, /// 0xa14
			 0x7b4e2470, /// 0xa18
			 0x2cbd3fe2, /// 0xa1c
			 0x7ebb6450, /// 0xa20
			 0xf82bc213, /// 0xa24
			 0xcb18e5f5, /// 0xa28
			 0xb602e3ad, /// 0xa2c
			 0x20cb3c53, /// 0xa30
			 0x506da043, /// 0xa34
			 0x0b088fb5, /// 0xa38
			 0xe589530f, /// 0xa3c
			 0x02b54901, /// 0xa40
			 0x080666f0, /// 0xa44
			 0x54a12b6f, /// 0xa48
			 0x375ef947, /// 0xa4c
			 0x6300efa6, /// 0xa50
			 0x3e25057d, /// 0xa54
			 0xdb0b8eb7, /// 0xa58
			 0xb62d1b66, /// 0xa5c
			 0x28189ddc, /// 0xa60
			 0x8c90a0f0, /// 0xa64
			 0x01481389, /// 0xa68
			 0x007698ee, /// 0xa6c
			 0xfb736a84, /// 0xa70
			 0xb6484f4f, /// 0xa74
			 0x7a018640, /// 0xa78
			 0xb17ddeaa, /// 0xa7c
			 0x1103f413, /// 0xa80
			 0x49f8fa8c, /// 0xa84
			 0xcaa2f429, /// 0xa88
			 0xaa5bf6af, /// 0xa8c
			 0x7b20f551, /// 0xa90
			 0xb9fdcda3, /// 0xa94
			 0xd5ca75a9, /// 0xa98
			 0x4b6703a9, /// 0xa9c
			 0x8b2ee4de, /// 0xaa0
			 0x235faded, /// 0xaa4
			 0x1138945c, /// 0xaa8
			 0x08e529f5, /// 0xaac
			 0x86f41680, /// 0xab0
			 0x885b03b9, /// 0xab4
			 0xab7b6a5e, /// 0xab8
			 0x61503846, /// 0xabc
			 0x748f8dff, /// 0xac0
			 0xe6e557ff, /// 0xac4
			 0xd4e1888c, /// 0xac8
			 0x0d8daecc, /// 0xacc
			 0xcf77e87e, /// 0xad0
			 0x85586be9, /// 0xad4
			 0x32687c95, /// 0xad8
			 0x63cd505c, /// 0xadc
			 0x06937416, /// 0xae0
			 0xc9ef1349, /// 0xae4
			 0x258acd23, /// 0xae8
			 0xfd90ce12, /// 0xaec
			 0xaf7b07ee, /// 0xaf0
			 0xddbdbf38, /// 0xaf4
			 0xb93a7876, /// 0xaf8
			 0xa503f7f3, /// 0xafc
			 0x40121f7e, /// 0xb00
			 0x5108ac25, /// 0xb04
			 0x9c6aea64, /// 0xb08
			 0xed37adde, /// 0xb0c
			 0x23d07f70, /// 0xb10
			 0x502f4b46, /// 0xb14
			 0xe4f1c5ed, /// 0xb18
			 0x2cc2fc41, /// 0xb1c
			 0x8decebed, /// 0xb20
			 0xd44be357, /// 0xb24
			 0xdbe97869, /// 0xb28
			 0x291246b3, /// 0xb2c
			 0x59e50e75, /// 0xb30
			 0xf2db9634, /// 0xb34
			 0xedf37f23, /// 0xb38
			 0xcf11c1d3, /// 0xb3c
			 0x389b1f79, /// 0xb40
			 0xbe9e7b34, /// 0xb44
			 0xefea1469, /// 0xb48
			 0x824af927, /// 0xb4c
			 0xffc1e537, /// 0xb50
			 0x850cacbb, /// 0xb54
			 0x2cc50731, /// 0xb58
			 0x42ee9f08, /// 0xb5c
			 0xa3957063, /// 0xb60
			 0x98104d58, /// 0xb64
			 0x85e3a2f2, /// 0xb68
			 0x0142c3f9, /// 0xb6c
			 0x99e05ae6, /// 0xb70
			 0xaa7bf15a, /// 0xb74
			 0xeb790725, /// 0xb78
			 0x56c25780, /// 0xb7c
			 0xdb3120e8, /// 0xb80
			 0xbd2810ee, /// 0xb84
			 0x4c21f1f4, /// 0xb88
			 0xe28de790, /// 0xb8c
			 0xc4d265a5, /// 0xb90
			 0x08c0c670, /// 0xb94
			 0x9d87bd38, /// 0xb98
			 0x307d1c37, /// 0xb9c
			 0xe132eeb8, /// 0xba0
			 0x32f0c61b, /// 0xba4
			 0x0f123418, /// 0xba8
			 0x70c2232d, /// 0xbac
			 0xa5355f73, /// 0xbb0
			 0x1189ca70, /// 0xbb4
			 0xe02fec71, /// 0xbb8
			 0xcdc57256, /// 0xbbc
			 0xb8eb8833, /// 0xbc0
			 0x1745ab55, /// 0xbc4
			 0x76907b34, /// 0xbc8
			 0x80537fbc, /// 0xbcc
			 0x403c7923, /// 0xbd0
			 0x35d657cd, /// 0xbd4
			 0xe3c8011e, /// 0xbd8
			 0xf0b3d8ac, /// 0xbdc
			 0xbe986717, /// 0xbe0
			 0xeb4337a6, /// 0xbe4
			 0x0ecb465e, /// 0xbe8
			 0x09615b94, /// 0xbec
			 0xc282e549, /// 0xbf0
			 0xe203e36b, /// 0xbf4
			 0x6b5a3aae, /// 0xbf8
			 0x440e34fd, /// 0xbfc
			 0x3353e6fd, /// 0xc00
			 0x853ad398, /// 0xc04
			 0xbedf6555, /// 0xc08
			 0x28b97d8c, /// 0xc0c
			 0x34eccd0e, /// 0xc10
			 0x3adf9123, /// 0xc14
			 0x171d87e4, /// 0xc18
			 0x5bf010f5, /// 0xc1c
			 0x4e54ed6f, /// 0xc20
			 0x76cf3f20, /// 0xc24
			 0xaa6a974c, /// 0xc28
			 0xee269402, /// 0xc2c
			 0xc78a71f0, /// 0xc30
			 0xb4168a9e, /// 0xc34
			 0x152dea32, /// 0xc38
			 0xdb9649e2, /// 0xc3c
			 0x38859caf, /// 0xc40
			 0x092a7dbb, /// 0xc44
			 0xcd316995, /// 0xc48
			 0x532e3503, /// 0xc4c
			 0x07480cd6, /// 0xc50
			 0x0e584fbc, /// 0xc54
			 0x2dcf1e73, /// 0xc58
			 0x7dacce01, /// 0xc5c
			 0x3de9f292, /// 0xc60
			 0x314ae9a5, /// 0xc64
			 0x94db5529, /// 0xc68
			 0x01204138, /// 0xc6c
			 0x0cf7bced, /// 0xc70
			 0xe55f1b72, /// 0xc74
			 0x3d05d527, /// 0xc78
			 0xd23b60c4, /// 0xc7c
			 0xd8c50de6, /// 0xc80
			 0xfba4e750, /// 0xc84
			 0xb487976a, /// 0xc88
			 0x82ccb35f, /// 0xc8c
			 0xa291a6f2, /// 0xc90
			 0xa15450ee, /// 0xc94
			 0x1b659381, /// 0xc98
			 0x6b472d29, /// 0xc9c
			 0x976e1876, /// 0xca0
			 0x4b6636b7, /// 0xca4
			 0xa3d48bf7, /// 0xca8
			 0x8bfb8d2d, /// 0xcac
			 0xa0726db4, /// 0xcb0
			 0x9b6b3d17, /// 0xcb4
			 0x0a20f2a2, /// 0xcb8
			 0xf90eca49, /// 0xcbc
			 0x76ba8745, /// 0xcc0
			 0x161e6392, /// 0xcc4
			 0xb4acdd0d, /// 0xcc8
			 0xc7760bfd, /// 0xccc
			 0x7b0e0586, /// 0xcd0
			 0x139aa42f, /// 0xcd4
			 0x8f908684, /// 0xcd8
			 0x489eb84d, /// 0xcdc
			 0x1105f254, /// 0xce0
			 0x8d0be1fd, /// 0xce4
			 0x4db63e5f, /// 0xce8
			 0xec2d4e64, /// 0xcec
			 0xbd404797, /// 0xcf0
			 0xd43a5945, /// 0xcf4
			 0xdba765a3, /// 0xcf8
			 0x787abac4, /// 0xcfc
			 0xe7611363, /// 0xd00
			 0x36638751, /// 0xd04
			 0x1d48fbe3, /// 0xd08
			 0xa69fafdb, /// 0xd0c
			 0xebe8e536, /// 0xd10
			 0x3e6c71cb, /// 0xd14
			 0xa9b25375, /// 0xd18
			 0x37e5caa0, /// 0xd1c
			 0x50acfaa1, /// 0xd20
			 0x3837b787, /// 0xd24
			 0x2b887f36, /// 0xd28
			 0x645faf57, /// 0xd2c
			 0x576a4d3a, /// 0xd30
			 0x855dd876, /// 0xd34
			 0x88203cef, /// 0xd38
			 0x47dbec90, /// 0xd3c
			 0x97d98146, /// 0xd40
			 0xbe9f7c4b, /// 0xd44
			 0x82f56672, /// 0xd48
			 0x7f5ab8c7, /// 0xd4c
			 0x4610077d, /// 0xd50
			 0x3e7de5ad, /// 0xd54
			 0xc5c76a9b, /// 0xd58
			 0x0f719e99, /// 0xd5c
			 0x3b626899, /// 0xd60
			 0x1992ddb7, /// 0xd64
			 0x4d6aed9a, /// 0xd68
			 0x5660dfcd, /// 0xd6c
			 0xdf1eb490, /// 0xd70
			 0xbc4738f7, /// 0xd74
			 0x1255cda4, /// 0xd78
			 0x4fd09688, /// 0xd7c
			 0x8d72e50c, /// 0xd80
			 0x7678a613, /// 0xd84
			 0x51cadbf2, /// 0xd88
			 0xe532b7a9, /// 0xd8c
			 0xdd047dab, /// 0xd90
			 0xade5858a, /// 0xd94
			 0xcf6a5e2f, /// 0xd98
			 0x7fc4e667, /// 0xd9c
			 0x5749c313, /// 0xda0
			 0x1fe9308d, /// 0xda4
			 0xfbba01cf, /// 0xda8
			 0x2bcd2c75, /// 0xdac
			 0xa6a8f16b, /// 0xdb0
			 0x47f7e966, /// 0xdb4
			 0x058ce72d, /// 0xdb8
			 0x3e43529b, /// 0xdbc
			 0xd2100c27, /// 0xdc0
			 0xeba30ff5, /// 0xdc4
			 0x96840883, /// 0xdc8
			 0x4bc6ae3d, /// 0xdcc
			 0xf884d85d, /// 0xdd0
			 0x0cc3bd30, /// 0xdd4
			 0x7e361557, /// 0xdd8
			 0x014f0354, /// 0xddc
			 0x2514ca7f, /// 0xde0
			 0x78221466, /// 0xde4
			 0xd01fe2f7, /// 0xde8
			 0x09426ce5, /// 0xdec
			 0xa69e65ec, /// 0xdf0
			 0x1421c5dd, /// 0xdf4
			 0xc9e85d52, /// 0xdf8
			 0x0c6a1592, /// 0xdfc
			 0x393a8639, /// 0xe00
			 0x81526b2c, /// 0xe04
			 0x98bba235, /// 0xe08
			 0xb8b20f26, /// 0xe0c
			 0x58186aaa, /// 0xe10
			 0xda68220d, /// 0xe14
			 0x8c14a57f, /// 0xe18
			 0x32a1fc03, /// 0xe1c
			 0x6c78ac84, /// 0xe20
			 0xfbb139a9, /// 0xe24
			 0x1735724b, /// 0xe28
			 0x294ff613, /// 0xe2c
			 0xcdd5b172, /// 0xe30
			 0x5639743c, /// 0xe34
			 0x0abaa148, /// 0xe38
			 0x152896e7, /// 0xe3c
			 0xc025a8ec, /// 0xe40
			 0xdd77f52f, /// 0xe44
			 0x70221dec, /// 0xe48
			 0x8dfe5f26, /// 0xe4c
			 0xf7589f41, /// 0xe50
			 0x2d36f922, /// 0xe54
			 0xb5a42f91, /// 0xe58
			 0x7633da38, /// 0xe5c
			 0x0c2cac29, /// 0xe60
			 0xa98315e2, /// 0xe64
			 0xae9dd4fb, /// 0xe68
			 0x5e239ff2, /// 0xe6c
			 0x1f6236f1, /// 0xe70
			 0x2416afb9, /// 0xe74
			 0x80e5e513, /// 0xe78
			 0x5b01dc3a, /// 0xe7c
			 0x32dca830, /// 0xe80
			 0xf4b10d90, /// 0xe84
			 0x86e06ef7, /// 0xe88
			 0x0632468f, /// 0xe8c
			 0x3d183683, /// 0xe90
			 0x76acfaed, /// 0xe94
			 0x94fb91d2, /// 0xe98
			 0x94f05059, /// 0xe9c
			 0x0712e511, /// 0xea0
			 0xc99974a5, /// 0xea4
			 0x8792a86b, /// 0xea8
			 0x2aea252c, /// 0xeac
			 0xb42dc736, /// 0xeb0
			 0xa81fd6ce, /// 0xeb4
			 0x9cf1e1c5, /// 0xeb8
			 0xfe1af778, /// 0xebc
			 0xc7603ebd, /// 0xec0
			 0xdc230fd8, /// 0xec4
			 0xfee81f27, /// 0xec8
			 0x308b1170, /// 0xecc
			 0x0c6667be, /// 0xed0
			 0xbb529da6, /// 0xed4
			 0xecc87313, /// 0xed8
			 0x90dccd19, /// 0xedc
			 0xfa285c44, /// 0xee0
			 0x69ea2b54, /// 0xee4
			 0x1e531f36, /// 0xee8
			 0x80cf84c0, /// 0xeec
			 0x0eaebddf, /// 0xef0
			 0xad050614, /// 0xef4
			 0x4adcc9dd, /// 0xef8
			 0xf8bbf2fa, /// 0xefc
			 0x8b16169a, /// 0xf00
			 0x326b4b46, /// 0xf04
			 0xbfbc1fae, /// 0xf08
			 0x6f45c565, /// 0xf0c
			 0x8ae7b146, /// 0xf10
			 0xb1605bd3, /// 0xf14
			 0x52a86282, /// 0xf18
			 0x788c698e, /// 0xf1c
			 0x41ccef69, /// 0xf20
			 0xfbf60ea6, /// 0xf24
			 0x93a3b967, /// 0xf28
			 0x446d0c66, /// 0xf2c
			 0x8ef5e491, /// 0xf30
			 0x7eee2186, /// 0xf34
			 0xbb22b359, /// 0xf38
			 0x5af71b6a, /// 0xf3c
			 0xf2608e56, /// 0xf40
			 0x47da0ee0, /// 0xf44
			 0x35815079, /// 0xf48
			 0xbda93a70, /// 0xf4c
			 0x22b0e2b0, /// 0xf50
			 0x1274bde2, /// 0xf54
			 0x49900878, /// 0xf58
			 0xdbdcdca2, /// 0xf5c
			 0xc25ea2dc, /// 0xf60
			 0xb37f1934, /// 0xf64
			 0x4e3dc8aa, /// 0xf68
			 0xfdb9cd42, /// 0xf6c
			 0xdcf05295, /// 0xf70
			 0x5a5432bd, /// 0xf74
			 0xb6304782, /// 0xf78
			 0x315dd044, /// 0xf7c
			 0xb620f497, /// 0xf80
			 0xc6692ea5, /// 0xf84
			 0xc7553d96, /// 0xf88
			 0xc4ab04f7, /// 0xf8c
			 0xa764becd, /// 0xf90
			 0x5f905b0d, /// 0xf94
			 0x1c6690d5, /// 0xf98
			 0xf177915d, /// 0xf9c
			 0xc809e962, /// 0xfa0
			 0x9ccee4e4, /// 0xfa4
			 0xeea20273, /// 0xfa8
			 0xc15d04e9, /// 0xfac
			 0x958d9d6e, /// 0xfb0
			 0xa9bb49d1, /// 0xfb4
			 0x90f82fcc, /// 0xfb8
			 0xa8293056, /// 0xfbc
			 0xe0b5eded, /// 0xfc0
			 0xb7b0e5c6, /// 0xfc4
			 0x8290e4a7, /// 0xfc8
			 0x52dce41d, /// 0xfcc
			 0x09544a84, /// 0xfd0
			 0x87d3b265, /// 0xfd4
			 0xb2ecaa0a, /// 0xfd8
			 0xfec66e48, /// 0xfdc
			 0xee07276e, /// 0xfe0
			 0x02d6f8b6, /// 0xfe4
			 0xc5723435, /// 0xfe8
			 0xad247d1b, /// 0xfec
			 0xdfd7b85c, /// 0xff0
			 0xf0c5d0b2, /// 0xff4
			 0xa9956c9e, /// 0xff8
			 0x75b42af6 /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0c600000,                                                  // Leading 1s:                                 /// 00000
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00008
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0000c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00010
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00014
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00018
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00024
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00028
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0002c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00030
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00034
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00038
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0003c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00048
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0004c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00050
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00054
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00058
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0005c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00060
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00064
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00068
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0006c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00078
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0007c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00080
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00088
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00090
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0xff800000,                                                  // -inf                                        /// 00098
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0009c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 000a0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 000a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000ac
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000b0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000b8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 000bc
			 0x0c600000,                                                  // Leading 1s:                                 /// 000c0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000c4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 000d0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000d8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000dc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 000e0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000e4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000e8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000f0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000fc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00104
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00108
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00110
			 0x80000000,                                                  // -zero                                       /// 00114
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0011c
			 0x4b000000,                                                  // 8388608.0                                   /// 00120
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00124
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00128
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0012c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00138
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0013c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00140
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00144
			 0xcb000000,                                                  // -8388608.0                                  /// 00148
			 0x0e000007,                                                  // Trailing 1s:                                /// 0014c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00154
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00158
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0015c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00160
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00168
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0016c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00174
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00178
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0017c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00180
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00184
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00188
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0018c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00190
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00194
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0019c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 001a0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 001a4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001a8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001ac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001b0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001b4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001bc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001c4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001c8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001cc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001d4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001d8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001dc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001e8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 001f0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 001f4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001fc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00204
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00208
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0020c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00214
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00218
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0021c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00220
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00224
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0022c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00230
			 0x0e000003,                                                  // Trailing 1s:                                /// 00234
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0023c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00240
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00244
			 0x00011111,                                                  // 9.7958E-41                                  /// 00248
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00250
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00258
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0025c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00260
			 0x0e000001,                                                  // Trailing 1s:                                /// 00264
			 0x3f028f5c,                                                  // 0.51                                        /// 00268
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00270
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00274
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00280
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00284
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00288
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0028c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00294
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00298
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002a4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002a8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002ac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002b0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002b4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 002b8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002bc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002c0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002c8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002d0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002d4
			 0xcb000000,                                                  // -8388608.0                                  /// 002d8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002e0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 002e4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002ec
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002f4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002fc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00300
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00304
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00308
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00310
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00318
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0031c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00320
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00324
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00328
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00330
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00334
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0033c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00340
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00344
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00348
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00350
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00354
			 0xbf028f5c,                                                  // -0.51                                       /// 00358
			 0x0e000001,                                                  // Trailing 1s:                                /// 0035c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00360
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00364
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00368
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0036c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00370
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00378
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0037c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x0e000001,                                                  // Trailing 1s:                                /// 00384
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00388
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00390
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00394
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0039c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x0c700000,                                                  // Leading 1s:                                 /// 003a4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 003a8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x33333333,                                                  // 4 random values                             /// 003b4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 003b8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 003bc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003c0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 003c4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003d8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003dc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 003e0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003e4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0xffc00001,                                                  // -signaling NaN                              /// 003ec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003f0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003f4
			 0x0e000001,                                                  // Trailing 1s:                                /// 003f8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003fc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00400
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00404
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00408
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0040c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00410
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0041c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00428
			 0x4b000000,                                                  // 8388608.0                                   /// 0042c
			 0x4b000000,                                                  // 8388608.0                                   /// 00430
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00434
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00438
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0043c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00444
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00450
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00454
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00458
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0045c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00464
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00468
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0046c
			 0x33333333,                                                  // 4 random values                             /// 00470
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00474
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0047c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00480
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00484
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00488
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00490
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00498
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0049c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004a0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004b0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 004b8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004bc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004c4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004c8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004cc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004d0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004d8
			 0x80000000,                                                  // -zero                                       /// 004dc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004ec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004f0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004f4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00500
			 0x0e000003,                                                  // Trailing 1s:                                /// 00504
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00508
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0050c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00510
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00514
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00518
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0051c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00520
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00524
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00528
			 0x0e000007,                                                  // Trailing 1s:                                /// 0052c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00530
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00534
			 0x7f800000,                                                  // inf                                         /// 00538
			 0x0e000003,                                                  // Trailing 1s:                                /// 0053c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00540
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00544
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00548
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0054c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00550
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0xcb000000,                                                  // -8388608.0                                  /// 00558
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00560
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00564
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00568
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0056c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00570
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00578
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0057c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00580
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00588
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0058c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00590
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00594
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00598
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0059c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005a0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 005a8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005ac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005b4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005b8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005bc
			 0x0e000001,                                                  // Trailing 1s:                                /// 005c0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005cc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005d4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005d8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005dc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005e0
			 0x0e000007,                                                  // Trailing 1s:                                /// 005e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0xffc00001,                                                  // -signaling NaN                              /// 005ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005f0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005f4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005f8
			 0x0e000003,                                                  // Trailing 1s:                                /// 005fc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00600
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00604
			 0x33333333,                                                  // 4 random values                             /// 00608
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0060c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00610
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00614
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00618
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0061c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00620
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00624
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0062c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00630
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00634
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x80000000,                                                  // -zero                                       /// 0063c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00644
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00648
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0064c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00654
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0065c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00664
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00668
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0066c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00670
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00674
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0067c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00680
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00684
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00688
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0068c
			 0x00000000,                                                  // zero                                        /// 00690
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00698
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0069c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006a0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 006ac
			 0xCCCCCCCC,                                                  // 4 random values                             /// 006b0
			 0xcb000000,                                                  // -8388608.0                                  /// 006b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006b8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006bc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006c0
			 0x4b000000,                                                  // 8388608.0                                   /// 006c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006c8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006d0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006d4
			 0xffc00001,                                                  // -signaling NaN                              /// 006d8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006dc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006e0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006e4
			 0x0e000003,                                                  // Trailing 1s:                                /// 006e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006ec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006f0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006f4
			 0x0c780000,                                                  // Leading 1s:                                 /// 006f8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x00000000,                                                  // zero                                        /// 00704
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0070c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00710
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00714
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00718
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0071c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00720
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00728
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0072c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00730
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00734
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00738
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0073c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00740
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00744
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00748
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00750
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00758
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0075c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00760
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00768
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0076c
			 0x33333333,                                                  // 4 random values                             /// 00770
			 0x55555555,                                                  // 4 random values                             /// 00774
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00778
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0077c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00780
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00784
			 0x0e000007,                                                  // Trailing 1s:                                /// 00788
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0078c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00790
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00794
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00798
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007a4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007ac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007b8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x0c780000,                                                  // Leading 1s:                                 /// 007cc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007d0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007d4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007d8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007e0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 007e4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007e8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 007ec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007f0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007f4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007f8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00800
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00804
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00808
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0080c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00810
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00814
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00818
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0081c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00820
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x0c600000,                                                  // Leading 1s:                                 /// 00828
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0082c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00830
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00834
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00838
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0083c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00844
			 0xff800000,                                                  // -inf                                        /// 00848
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0084c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00850
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00854
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0085c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00860
			 0xcb000000,                                                  // -8388608.0                                  /// 00864
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00868
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0086c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00878
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0087c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00880
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00884
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00888
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0088c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00890
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00898
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0089c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008a0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008a4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008b4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008b8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 008c0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008c4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008c8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008d4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008d8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008dc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008e0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 008f0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008f4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00900
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00904
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00908
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0090c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00914
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00918
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00920
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00924
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00928
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0092c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00930
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00934
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00938
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0093c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00940
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00944
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00948
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0094c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00950
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00954
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0095c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00964
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0096c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00970
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00974
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00978
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0097c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00980
			 0x80011111,                                                  // -9.7958E-41                                 /// 00984
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0098c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00994
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0099c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009a0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009a4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 009a8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009ac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009b0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009b8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009bc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x80000000,                                                  // -zero                                       /// 009d0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x0c780000,                                                  // Leading 1s:                                 /// 009d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009dc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009e0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009e4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009e8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009ec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 009f0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009f8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009fc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a00
			 0x7fc00001,                                                  // signaling NaN                               /// 00a04
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a08
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a0c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a10
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a1c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a20
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a2c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a34
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a38
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a3c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a44
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a48
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a4c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a50
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a54
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a58
			 0x00000000,                                                  // zero                                        /// 00a5c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a60
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a64
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a68
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x3f028f5c,                                                  // 0.51                                        /// 00a74
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a84
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a88
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a8c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a90
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a94
			 0x80000000,                                                  // -zero                                       /// 00a98
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a9c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00aa0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00aa4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00aa8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00aac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ab0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ab4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ab8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00abc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ac0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ac4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ac8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ad4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ad8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00adc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ae0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ae4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00aec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00af0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00af4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00af8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00afc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b00
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b04
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b08
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b0c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b10
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b14
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b1c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b20
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b24
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b28
			 0xff800000,                                                  // -inf                                        /// 00b2c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b30
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b34
			 0x00000000,                                                  // zero                                        /// 00b38
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b3c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b40
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b48
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b50
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b54
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b58
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b5c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b64
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b70
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x80000000,                                                  // -zero                                       /// 00b78
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b80
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b88
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b90
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b98
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00b9c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ba0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ba4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ba8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bac
			 0x00000000,                                                  // zero                                        /// 00bb0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bb4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bb8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bbc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bc0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bc4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00bc8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bd4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bd8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bdc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00be0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00be4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00be8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bf4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bf8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bfc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c04
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00c08
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c10
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c14
			 0x3f028f5c,                                                  // 0.51                                        /// 00c18
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c1c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c20
			 0x33333333,                                                  // 4 random values                             /// 00c24
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c30
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c34
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c3c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c44
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c48
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c4c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c54
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c58
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c60
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c68
			 0xff800000,                                                  // -inf                                        /// 00c6c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c74
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c78
			 0xbf028f5c,                                                  // -0.51                                       /// 00c7c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00c80
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c84
			 0x55555555,                                                  // 4 random values                             /// 00c88
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c8c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c90
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c98
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c9c
			 0xff800000,                                                  // -inf                                        /// 00ca0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ca4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ca8
			 0x80000000,                                                  // -zero                                       /// 00cac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00cb0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cb4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cbc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00cc0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cc4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00cc8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cd0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cd8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cdc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ce0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ce8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00cec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cf0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cf4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00cf8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d00
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d04
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d0c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d14
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d1c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d24
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d28
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d2c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d34
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d38
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00d3c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d40
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d44
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d4c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d50
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d58
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d5c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d68
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d6c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d70
			 0xffc00001,                                                  // -signaling NaN                              /// 00d74
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d78
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d7c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d84
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d88
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00d90
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d94
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d98
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d9c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00da0
			 0x33333333,                                                  // 4 random values                             /// 00da4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00da8
			 0xbf028f5c,                                                  // -0.51                                       /// 00dac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00db0
			 0x00000000,                                                  // zero                                        /// 00db4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00db8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dbc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00dc0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00dc4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00dc8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00dcc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00dd0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00dd4
			 0xffc00001,                                                  // -signaling NaN                              /// 00dd8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ddc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00de0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00de4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00de8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00dec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00df0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00df4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00df8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dfc
			 0x33333333,                                                  // 4 random values                             /// 00e00
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e04
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e0c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00e10
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e18
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e1c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e20
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e24
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e2c
			 0xffc00001,                                                  // -signaling NaN                              /// 00e30
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e34
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e38
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e3c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e48
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e4c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e50
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e58
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e5c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e60
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e64
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e68
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e74
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e78
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e7c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e80
			 0xcb000000,                                                  // -8388608.0                                  /// 00e84
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e90
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e98
			 0xff800000,                                                  // -inf                                        /// 00e9c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ea4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ea8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00eac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00eb0
			 0x7f800000,                                                  // inf                                         /// 00eb4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00eb8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ebc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ec4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ec8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ecc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00ed4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ed8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ee0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ee4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00eec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ef0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ef4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ef8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00efc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f00
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f04
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f08
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f0c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x4b000000,                                                  // 8388608.0                                   /// 00f14
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f18
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00f1c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f20
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f24
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f2c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f30
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f34
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f38
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f40
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f44
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f4c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f50
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f5c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f60
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f64
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f68
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f6c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f70
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f74
			 0x55555555,                                                  // 4 random values                             /// 00f78
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f7c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f84
			 0x7f800000,                                                  // inf                                         /// 00f88
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f8c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f90
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f94
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f98
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f9c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fa0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00fa4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fa8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00fac
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fb4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00fb8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fbc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fc4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fc8
			 0x80000000,                                                  // -zero                                       /// 00fcc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fd0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fd4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fdc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fe0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fe4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fe8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fec
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ff4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ff8
			 0x00000002 // min subnorm + 1 ulp                           // SP +ve numbers                              /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00004
			 0x4b000000,                                                  // 8388608.0                                   /// 00008
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0000c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00010
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00014
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00018
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0001c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00020
			 0x7fc00001,                                                  // signaling NaN                               /// 00024
			 0x0c600000,                                                  // Leading 1s:                                 /// 00028
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0002c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00030
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00038
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0003c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00040
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00044
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00048
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0004c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00058
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0005c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00060
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00064
			 0x0c600000,                                                  // Leading 1s:                                 /// 00068
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0006c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00074
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00078
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0007c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00084
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0008c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00090
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00094
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00098
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0009c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000a0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000a4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000a8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000b8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000bc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 000c4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000cc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 000d0
			 0x55555555,                                                  // 4 random values                             /// 000d4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000dc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000e0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000e4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 000e8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000ec
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 000f0
			 0xcb000000,                                                  // -8388608.0                                  /// 000f4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 000fc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00100
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00108
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0010c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00110
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00114
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00118
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00120
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00124
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00128
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0012c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00130
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00134
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00138
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0013c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00144
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00148
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0014c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00150
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00154
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00158
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0015c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00164
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00170
			 0xbf028f5c,                                                  // -0.51                                       /// 00174
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00178
			 0x0c400000,                                                  // Leading 1s:                                 /// 0017c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00180
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00184
			 0x0c400000,                                                  // Leading 1s:                                 /// 00188
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00190
			 0x80000000,                                                  // -zero                                       /// 00194
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00198
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0019c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 001a4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001b0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001b4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001b8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001c0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001c4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001c8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001cc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001d0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 001d4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001d8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001e0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001e4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 001f0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001f8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001fc
			 0x3f028f5c,                                                  // 0.51                                        /// 00200
			 0xff800000,                                                  // -inf                                        /// 00204
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00208
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00210
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00214
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00218
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0021c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00220
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00224
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0022c
			 0x00000000,                                                  // zero                                        /// 00230
			 0x4b000000,                                                  // 8388608.0                                   /// 00234
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00238
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0023c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00244
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00248
			 0x00011111,                                                  // 9.7958E-41                                  /// 0024c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00250
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00254
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00258
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0025c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00260
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00264
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00268
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0026c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x0c600000,                                                  // Leading 1s:                                 /// 00274
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0027c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00280
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00288
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0028c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00290
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00294
			 0x0e000007,                                                  // Trailing 1s:                                /// 00298
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0029c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002a0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002a4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002a8
			 0xcb000000,                                                  // -8388608.0                                  /// 002ac
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002b0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 002b4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002bc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002c4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002d0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002d8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 002dc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002e4
			 0x0e000001,                                                  // Trailing 1s:                                /// 002e8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002ec
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002f0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002f8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002fc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00300
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00304
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00308
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0030c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00310
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00314
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00318
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0031c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00320
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00324
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0032c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x55555555,                                                  // 4 random values                             /// 00334
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0033c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00340
			 0x0e000001,                                                  // Trailing 1s:                                /// 00344
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00348
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00350
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00354
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00358
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00360
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00364
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0036c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00370
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00374
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0037c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00380
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00384
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00388
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0038c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00390
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00394
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00398
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003a0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b8
			 0x33333333,                                                  // 4 random values                             /// 003bc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003c8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 003cc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003d0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003dc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 003e0
			 0x3f028f5c,                                                  // 0.51                                        /// 003e4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003f4
			 0x0c780000,                                                  // Leading 1s:                                 /// 003f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 003fc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00400
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00404
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00408
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0040c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00410
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00414
			 0x0c700000,                                                  // Leading 1s:                                 /// 00418
			 0x0e000007,                                                  // Trailing 1s:                                /// 0041c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00420
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00428
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00430
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00434
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x00011111,                                                  // 9.7958E-41                                  /// 0043c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00440
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00444
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00448
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00454
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00458
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0045c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00464
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00468
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00474
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00478
			 0xffc00001,                                                  // -signaling NaN                              /// 0047c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00480
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00484
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00488
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00490
			 0x33333333,                                                  // 4 random values                             /// 00494
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00498
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x33333333,                                                  // 4 random values                             /// 004a8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004b0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004bc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004c0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004c4
			 0x0c700000,                                                  // Leading 1s:                                 /// 004c8
			 0x80011111,                                                  // -9.7958E-41                                 /// 004cc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004d4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004dc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004e0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004e8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 004ec
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004f4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004f8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004fc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00500
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00504
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00508
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0050c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00510
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0051c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00520
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00524
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00528
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0052c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00530
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00534
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00538
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0053c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00540
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00544
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00548
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0054c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00554
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00558
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00560
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00564
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0xffc00001,                                                  // -signaling NaN                              /// 0056c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00574
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0057c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00584
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00588
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0058c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00590
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00594
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00598
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0059c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005a4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005a8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005ac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005b0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005b4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005b8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005c4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005c8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005cc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005d0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005d4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005d8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005dc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005e8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005f0
			 0x55555555,                                                  // 4 random values                             /// 005f4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005f8
			 0xffc00001,                                                  // -signaling NaN                              /// 005fc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00600
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0060c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00610
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00614
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0061c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00628
			 0x0c600000,                                                  // Leading 1s:                                 /// 0062c
			 0x7f800000,                                                  // inf                                         /// 00630
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x80011111,                                                  // -9.7958E-41                                 /// 00638
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0063c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00640
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00644
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00648
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0064c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00650
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00654
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00658
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0065c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00660
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00664
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0066c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00674
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00678
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0067c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00680
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00684
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0068c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00690
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00694
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00698
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0069c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006a0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006a4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006a8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006b4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006b8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006c0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0xff800000,                                                  // -inf                                        /// 006c8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006cc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006d0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006d4
			 0xffc00001,                                                  // -signaling NaN                              /// 006d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 006dc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 006e0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006e8
			 0x80011111,                                                  // -9.7958E-41                                 /// 006ec
			 0xbf028f5c,                                                  // -0.51                                       /// 006f0
			 0x00000000,                                                  // zero                                        /// 006f4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 006fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x0e000001,                                                  // Trailing 1s:                                /// 00708
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0070c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00718
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0071c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00724
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00728
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0072c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00730
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00738
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0x33333333,                                                  // 4 random values                             /// 00740
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0074c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00750
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00754
			 0xbf028f5c,                                                  // -0.51                                       /// 00758
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0075c
			 0x00000000,                                                  // zero                                        /// 00760
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00764
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00768
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0076c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00774
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00778
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00784
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00788
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0078c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00790
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00794
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00798
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0079c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007a0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007a8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007ac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007b0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007b4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007b8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007bc
			 0x7fc00001,                                                  // signaling NaN                               /// 007c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007c8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 007cc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x00011111,                                                  // 9.7958E-41                                  /// 007d8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007e0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007e4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 007e8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007ec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 007f0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 007f8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0xbf028f5c,                                                  // -0.51                                       /// 00804
			 0x33333333,                                                  // 4 random values                             /// 00808
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0080c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00810
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00818
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0081c
			 0x33333333,                                                  // 4 random values                             /// 00820
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00824
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00828
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00834
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00838
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0083c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00840
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0084c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00850
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00858
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0085c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00860
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00864
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00870
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00874
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0087c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00880
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00884
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0088c
			 0xff800000,                                                  // -inf                                        /// 00890
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00894
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00898
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0089c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008a8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008ac
			 0x33333333,                                                  // 4 random values                             /// 008b0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008b4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008b8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008bc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x0e000007,                                                  // Trailing 1s:                                /// 008c4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008c8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008cc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008d8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008dc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008e8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 008ec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008f4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008fc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00900
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00904
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00908
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x33333333,                                                  // 4 random values                             /// 00910
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00918
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0091c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00920
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00924
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0092c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00930
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00934
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00938
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00940
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00944
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00948
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0094c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00954
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00958
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0095c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00960
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00964
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0096c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00970
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00974
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0097c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00980
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0098c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00990
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00994
			 0xbf028f5c,                                                  // -0.51                                       /// 00998
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0099c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 009a4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009ac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009b0
			 0x7fc00001,                                                  // signaling NaN                               /// 009b4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x00000000,                                                  // zero                                        /// 009bc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009c0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009c4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 009c8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009cc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009d0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009d8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009e0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009e4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009e8
			 0x0c600000,                                                  // Leading 1s:                                 /// 009ec
			 0xffc00001,                                                  // -signaling NaN                              /// 009f0
			 0x80011111,                                                  // -9.7958E-41                                 /// 009f4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009fc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a04
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a08
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a0c
			 0xcb000000,                                                  // -8388608.0                                  /// 00a10
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a14
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a18
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a1c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a20
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a2c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a30
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a34
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a38
			 0x00000000,                                                  // zero                                        /// 00a3c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a40
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a48
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a4c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a50
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a54
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a58
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a5c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a60
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a64
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a68
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a6c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a70
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a74
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a78
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a7c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a80
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a84
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a88
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a8c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a90
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a94
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a98
			 0xff800000,                                                  // -inf                                        /// 00a9c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00aa0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00aa4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00aac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ab4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ab8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ac0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ac4
			 0xbf028f5c,                                                  // -0.51                                       /// 00ac8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00acc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ad0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ad4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ad8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00adc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ae0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ae4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ae8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00aec
			 0x0c780000,                                                  // Leading 1s:                                 /// 00af0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00af4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00af8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00afc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b00
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b0c
			 0x7f800000,                                                  // inf                                         /// 00b10
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b14
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b18
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b1c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b20
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x55555555,                                                  // 4 random values                             /// 00b2c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b30
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b34
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b38
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b3c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b40
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b44
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b4c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b50
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b54
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b5c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b64
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b68
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b6c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00b70
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b74
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b78
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b7c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b88
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b8c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b90
			 0x00000000,                                                  // zero                                        /// 00b94
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b98
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b9c
			 0x55555555,                                                  // 4 random values                             /// 00ba0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ba4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ba8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bb0
			 0x33333333,                                                  // 4 random values                             /// 00bb4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bb8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bbc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bc0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00bc4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bc8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bcc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x3f028f5c,                                                  // 0.51                                        /// 00bd4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bd8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bdc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00be0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00bec
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bf0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bf4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bf8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c10
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c18
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c1c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c20
			 0x00000000,                                                  // zero                                        /// 00c24
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c28
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c2c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c30
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c38
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c40
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c44
			 0x00000000,                                                  // zero                                        /// 00c48
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c4c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c54
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c58
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c5c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c64
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00c68
			 0xff800000,                                                  // -inf                                        /// 00c6c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c74
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c78
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c7c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c80
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c84
			 0x55555555,                                                  // 4 random values                             /// 00c88
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c8c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c90
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c94
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c98
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c9c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ca0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ca4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cb4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cb8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00cbc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cc0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00cc4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cc8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cd0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cd4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00cd8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cdc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ce0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ce4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ce8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cf0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cf4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00cf8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d00
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d08
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d0c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d10
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d14
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d18
			 0x00000000,                                                  // zero                                        /// 00d1c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d20
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d24
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d28
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d30
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d34
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d38
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d3c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d44
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d48
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d54
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d58
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d60
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d68
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d70
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d7c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d80
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d84
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d8c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d90
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d98
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00d9c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00da0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00da4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00dac
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00db0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00db4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00db8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00dbc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00dc0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dc4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dc8
			 0xbf028f5c,                                                  // -0.51                                       /// 00dcc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00de0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00de4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00de8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00df0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00df4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00df8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00dfc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e00
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e04
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e08
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x4b000000,                                                  // 8388608.0                                   /// 00e10
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e1c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e20
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e24
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0xcb000000,                                                  // -8388608.0                                  /// 00e2c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e30
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e38
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e40
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e4c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e50
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e5c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e60
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e64
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e68
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e6c
			 0xffc00001,                                                  // -signaling NaN                              /// 00e70
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e74
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e78
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e7c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e80
			 0x3f028f5c,                                                  // 0.51                                        /// 00e84
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e88
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e8c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e90
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e94
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e98
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e9c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ea4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ea8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00eac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00eb0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00eb4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00eb8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ebc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec0
			 0x55555555,                                                  // 4 random values                             /// 00ec4
			 0xcb000000,                                                  // -8388608.0                                  /// 00ec8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ed0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ed4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ed8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00edc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ee4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ee8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00eec
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ef0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ef4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00f00
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f04
			 0xbf028f5c,                                                  // -0.51                                       /// 00f08
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f10
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f14
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f18
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f1c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f20
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f28
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f2c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f30
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f34
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f38
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f44
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f48
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f4c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f54
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f58
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f5c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f60
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f64
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f6c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f70
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f78
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f7c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f80
			 0xbf028f5c,                                                  // -0.51                                       /// 00f84
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f88
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f8c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f90
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f94
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f98
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f9c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fa0
			 0x33333333,                                                  // 4 random values                             /// 00fa4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fa8
			 0x33333333,                                                  // 4 random values                             /// 00fac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fb0
			 0x7f800000,                                                  // inf                                         /// 00fb4
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fb8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fbc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00fc4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fc8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fcc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00fd0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00fd4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00fd8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fe0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fe4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fe8
			 0x7fc00001,                                                  // signaling NaN                               /// 00fec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ff0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ff4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ff8
			 0xff800000 // infinity                                      // SP - ve numbers                             /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x0c700000,                                                  // Leading 1s:                                 /// 00000
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00010
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00014
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00018
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0001c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00024
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00028
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0002c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00030
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00034
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00038
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0003c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00044
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00048
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0004c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00050
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00054
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00058
			 0x0e000001,                                                  // Trailing 1s:                                /// 0005c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00060
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00064
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00068
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0006c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00070
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00074
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x00000000,                                                  // zero                                        /// 0007c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00084
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0008c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00090
			 0x80011111,                                                  // -9.7958E-41                                 /// 00094
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00098
			 0x4b000000,                                                  // 8388608.0                                   /// 0009c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ac
			 0x33333333,                                                  // 4 random values                             /// 000b0
			 0x7f800000,                                                  // inf                                         /// 000b4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000b8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000c0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000c4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000c8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000cc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000d0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 000d4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000dc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000e0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000e8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000ec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000f0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x4b000000,                                                  // 8388608.0                                   /// 000f8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000fc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00100
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00104
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0010c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00114
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0011c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00120
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00128
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x4b000000,                                                  // 8388608.0                                   /// 00134
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00138
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0013c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00144
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00148
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00150
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00154
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00158
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00160
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00164
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0016c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00170
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00174
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00178
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0017c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00180
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00184
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00188
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00190
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00194
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00198
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0019c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001a0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001a4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001a8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001ac
			 0x0c700000,                                                  // Leading 1s:                                 /// 001b0
			 0x0c600000,                                                  // Leading 1s:                                 /// 001b4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001b8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001bc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001c0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001c4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001c8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 001cc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001d0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001d4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001d8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001dc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001e0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001e4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001e8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001ec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001f0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001f4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 001fc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00204
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00208
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00210
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00214
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0021c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00220
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00224
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0022c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00230
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00234
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x00011111,                                                  // 9.7958E-41                                  /// 0023c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00240
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00244
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0xffc00001,                                                  // -signaling NaN                              /// 0024c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00250
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00254
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00258
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0025c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00264
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00270
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00274
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00278
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0027c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00280
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00284
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00288
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0028c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00294
			 0x0c780000,                                                  // Leading 1s:                                 /// 00298
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0029c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002a0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 002a8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002ac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002b0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002b4
			 0x80000000,                                                  // -zero                                       /// 002b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 002bc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002c0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 002c4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002c8
			 0xbf028f5c,                                                  // -0.51                                       /// 002cc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002d0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002dc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002e0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002e4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 002f0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 002f8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002fc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00300
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00304
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00308
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0030c
			 0xffc00001,                                                  // -signaling NaN                              /// 00310
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00314
			 0x33333333,                                                  // 4 random values                             /// 00318
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0031c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00320
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00324
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00328
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0032c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00330
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0033c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00340
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00344
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00348
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0034c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00350
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00354
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00358
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0035c
			 0x80000000,                                                  // -zero                                       /// 00360
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00368
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0036c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00370
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00374
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0037c
			 0x4b000000,                                                  // 8388608.0                                   /// 00380
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00388
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00390
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00394
			 0x0c700000,                                                  // Leading 1s:                                 /// 00398
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a0
			 0x00000000,                                                  // zero                                        /// 003a4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003a8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003ac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003b8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003c0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003c4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003cc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003d0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003dc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003e0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003e4
			 0x0e000007,                                                  // Trailing 1s:                                /// 003e8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003f8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00404
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00408
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0040c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00410
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00414
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00418
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0041c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00420
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00424
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00428
			 0x0c780000,                                                  // Leading 1s:                                 /// 0042c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00430
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00434
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00438
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00440
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00444
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00448
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0044c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00450
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00454
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00458
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0045c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00460
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00464
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00468
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0046c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00470
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00474
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00478
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0047c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00484
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00488
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00490
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004a0
			 0x0c600000,                                                  // Leading 1s:                                 /// 004a4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004ac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 004b8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004bc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004c0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004cc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004d4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0xffc00001,                                                  // -signaling NaN                              /// 004dc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004e0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004e4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 004e8
			 0x0e000001,                                                  // Trailing 1s:                                /// 004ec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004f0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004f4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 004f8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004fc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00500
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00504
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0050c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x55555555,                                                  // 4 random values                             /// 00514
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00518
			 0xcb000000,                                                  // -8388608.0                                  /// 0051c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00520
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00524
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00528
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0052c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00530
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00534
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00540
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00544
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00548
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0054c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00550
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00558
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0055c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00560
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00564
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00568
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0056c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00570
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00574
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00578
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00580
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00590
			 0x0e000001,                                                  // Trailing 1s:                                /// 00594
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0059c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005a0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005a8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 005bc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005c0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 005c4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005c8
			 0x7f800000,                                                  // inf                                         /// 005cc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005d0
			 0x0e000007,                                                  // Trailing 1s:                                /// 005d4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0xbf028f5c,                                                  // -0.51                                       /// 005dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005e0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 005e4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0xff800000,                                                  // -inf                                        /// 005ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 005f0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 005f4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005fc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00600
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00608
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0060c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00610
			 0x0c700000,                                                  // Leading 1s:                                 /// 00614
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00618
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0061c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00624
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00628
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00630
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00638
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0063c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00640
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00644
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00648
			 0xffc00001,                                                  // -signaling NaN                              /// 0064c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00654
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00658
			 0x4b000000,                                                  // 8388608.0                                   /// 0065c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00660
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0066c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00678
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00680
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00684
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00688
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00694
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00698
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0x0c700000,                                                  // Leading 1s:                                 /// 006a0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006ac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006b0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006b4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006b8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006bc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006c8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006cc
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006d0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006dc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006e0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006e8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006ec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006f0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 006f8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00708
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0070c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00710
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00714
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00718
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0071c
			 0x80000000,                                                  // -zero                                       /// 00720
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00724
			 0x80011111,                                                  // -9.7958E-41                                 /// 00728
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00730
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00734
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0073c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00740
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00744
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00748
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0074c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00750
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00758
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00760
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00764
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00770
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00774
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00778
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0077c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00780
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00784
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00788
			 0x7fc00001,                                                  // signaling NaN                               /// 0078c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00790
			 0x55555555,                                                  // 4 random values                             /// 00794
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00798
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0079c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007a4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007a8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007ac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007b0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007b8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007c0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007c4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007cc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007d8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007e4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007e8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x3f028f5c,                                                  // 0.51                                        /// 007f0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007f4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007f8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007fc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00800
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00804
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00808
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0080c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00814
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00818
			 0xff800000,                                                  // -inf                                        /// 0081c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00820
			 0x80011111,                                                  // -9.7958E-41                                 /// 00824
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00828
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0082c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00830
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00834
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00838
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0083c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00844
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0084c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00850
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00858
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0085c
			 0xff800000,                                                  // -inf                                        /// 00860
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00864
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00868
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0086c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00870
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00874
			 0x0c400000,                                                  // Leading 1s:                                 /// 00878
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0087c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00880
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00884
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0088c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00890
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00894
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00898
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008a4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008a8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008ac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008b0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008b4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008b8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008bc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008c4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008c8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 008cc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008d4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008d8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008dc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008e0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 008e4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 008e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008ec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008f4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008fc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00904
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00908
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0090c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00910
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00914
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00918
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0091c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00920
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00924
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00928
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0092c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00930
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00934
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00938
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0093c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00940
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00944
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00948
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0094c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00950
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00954
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00958
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0095c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00960
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00968
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0096c
			 0xffc00001,                                                  // -signaling NaN                              /// 00970
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00974
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00978
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00980
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00984
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00988
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0098c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00994
			 0x80000000,                                                  // -zero                                       /// 00998
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0099c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009ac
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009b0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009b8
			 0x33333333,                                                  // 4 random values                             /// 009bc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009c0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009c4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009d0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009d4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009d8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009dc
			 0x0e000007,                                                  // Trailing 1s:                                /// 009e0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 009e4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 009ec
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009f0
			 0xcb000000,                                                  // -8388608.0                                  /// 009f4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 009f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a00
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a04
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a08
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a0c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a14
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a18
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a1c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a20
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a24
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a28
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a2c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a30
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a38
			 0x55555555,                                                  // 4 random values                             /// 00a3c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a40
			 0x33333333,                                                  // 4 random values                             /// 00a44
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a48
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a4c
			 0x00000000,                                                  // zero                                        /// 00a50
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a54
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a58
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a5c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x4b000000,                                                  // 8388608.0                                   /// 00a68
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a6c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a74
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a78
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a7c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a80
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a84
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a88
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a8c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a90
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a94
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00a98
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a9c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00aa0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00aa4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00aa8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ab0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00ab4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00abc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ac0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ac4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00acc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ad0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ad8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00adc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00ae4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ae8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00aec
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00af0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00af8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00afc
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b00
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b04
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b08
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b0c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b10
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b14
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b1c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b20
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b24
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b28
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b30
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b34
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b38
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b3c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b40
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b44
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b48
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b4c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b54
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b58
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b60
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b64
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b68
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b6c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b70
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b78
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b7c
			 0xcb000000,                                                  // -8388608.0                                  /// 00b80
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b84
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b88
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b8c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b90
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b94
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b98
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b9c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ba4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bb8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bbc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bc0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bc8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00bcc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bd0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bd8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00be0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00be4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00bec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bf0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00bf4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bf8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bfc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c00
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c04
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c08
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c10
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c14
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c1c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c24
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c28
			 0x00000000,                                                  // zero                                        /// 00c2c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c30
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c34
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c38
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c3c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c40
			 0x33333333,                                                  // 4 random values                             /// 00c44
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c48
			 0xcb000000,                                                  // -8388608.0                                  /// 00c4c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c50
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c58
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c60
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c64
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c68
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c70
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c78
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c7c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c84
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c88
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c94
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c9c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ca0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ca4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ca8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00cac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cb8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cbc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cc4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cc8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ccc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00cd0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cd4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00cd8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cdc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ce0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ce8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cec
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00cf4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00cf8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00cfc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00d00
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d0c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d18
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d1c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d28
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d2c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d30
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d34
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d38
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d3c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d40
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d44
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d48
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d4c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d50
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d54
			 0xffc00001,                                                  // -signaling NaN                              /// 00d58
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d5c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d60
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d64
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d68
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d6c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d74
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d7c
			 0xbf028f5c,                                                  // -0.51                                       /// 00d80
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d84
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d88
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d90
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d94
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00d98
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d9c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00da0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00da4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00da8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00dac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00db0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00db8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dc0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00dc4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00dc8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00dcc
			 0xcb000000,                                                  // -8388608.0                                  /// 00dd0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ddc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00de8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00dec
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00df0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00df4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00df8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dfc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00e00
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e0c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00e10
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e14
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e18
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e20
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e24
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e2c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e30
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e34
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e38
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e3c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e40
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e44
			 0x80000000,                                                  // -zero                                       /// 00e48
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e50
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e54
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e58
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e5c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e60
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e64
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e68
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e6c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e70
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e74
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e78
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e80
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e84
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e88
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e8c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00e90
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e94
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e9c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ea0
			 0x3f028f5c,                                                  // 0.51                                        /// 00ea4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ea8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00eac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00eb0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00eb8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ebc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ec0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ec4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x00000000,                                                  // zero                                        /// 00ecc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ed0
			 0x7f800000,                                                  // inf                                         /// 00ed4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00edc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ee0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ee4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00eec
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ef0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ef4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00ef8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00efc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f00
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f04
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f08
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f0c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f10
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f14
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f18
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f1c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f20
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00f24
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f28
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f2c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0xcb000000,                                                  // -8388608.0                                  /// 00f34
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x00000000,                                                  // zero                                        /// 00f48
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f4c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f54
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f58
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f5c
			 0xffc00001,                                                  // -signaling NaN                              /// 00f60
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f64
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f68
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f6c
			 0x7f800000,                                                  // inf                                         /// 00f70
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f80
			 0x33333333,                                                  // 4 random values                             /// 00f84
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f88
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f90
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f94
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f98
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f9c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fa0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fa4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fa8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00fac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fb0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00fb8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fbc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fc0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00fc4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fc8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fcc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fd0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fd4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00fd8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fdc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fe0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fe4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fe8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ff4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ff8
			 0xffc00000                                                  // -cquiet NaN                                 /// last
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
			 0x00000344,
			 0x000002b0,
			 0x00000768,
			 0x0000018c,
			 0x00000038,
			 0x000004a8,
			 0x0000025c,
			 0x00000578,

			 /// vpu register f2
			 0x41600000,
			 0x41c00000,
			 0x41900000,
			 0x41200000,
			 0x41600000,
			 0x41d00000,
			 0x41000000,
			 0x41900000,

			 /// vpu register f3
			 0x41000000,
			 0x41900000,
			 0x40800000,
			 0x41a80000,
			 0x41e00000,
			 0x41b80000,
			 0x41c80000,
			 0x41700000,

			 /// vpu register f4
			 0x41100000,
			 0x41600000,
			 0x40e00000,
			 0x41980000,
			 0x40400000,
			 0x41600000,
			 0x41a00000,
			 0x41400000,

			 /// vpu register f5
			 0x41c00000,
			 0x41200000,
			 0x41c00000,
			 0x41000000,
			 0x40400000,
			 0x40c00000,
			 0x41800000,
			 0x41980000,

			 /// vpu register f6
			 0x41000000,
			 0x40800000,
			 0x41980000,
			 0x41e00000,
			 0x41a00000,
			 0x41300000,
			 0x40400000,
			 0x41d80000,

			 /// vpu register f7
			 0x41300000,
			 0x41a80000,
			 0x41800000,
			 0x41500000,
			 0x42000000,
			 0x41000000,
			 0x41b80000,
			 0x42000000,

			 /// vpu register f8
			 0x41600000,
			 0x41f00000,
			 0x40400000,
			 0x40e00000,
			 0x41800000,
			 0x40c00000,
			 0x41700000,
			 0x41b00000,

			 /// vpu register f9
			 0x41b80000,
			 0x41600000,
			 0x41e80000,
			 0x41980000,
			 0x41b00000,
			 0x41300000,
			 0x40c00000,
			 0x41100000,

			 /// vpu register f10
			 0x41500000,
			 0x41f00000,
			 0x41c00000,
			 0x40a00000,
			 0x41d80000,
			 0x41e80000,
			 0x41f80000,
			 0x41500000,

			 /// vpu register f11
			 0x41200000,
			 0x40800000,
			 0x41b80000,
			 0x41300000,
			 0x40a00000,
			 0x40400000,
			 0x41300000,
			 0x42000000,

			 /// vpu register f12
			 0x41600000,
			 0x41900000,
			 0x41d00000,
			 0x41980000,
			 0x40a00000,
			 0x41d00000,
			 0x41400000,
			 0x40400000,

			 /// vpu register f13
			 0x40800000,
			 0x41b00000,
			 0x41d80000,
			 0x40e00000,
			 0x41a00000,
			 0x41a00000,
			 0x41a00000,
			 0x40400000,

			 /// vpu register f14
			 0x41f80000,
			 0x41b80000,
			 0x41c80000,
			 0x41e80000,
			 0x40000000,
			 0x40a00000,
			 0x41a80000,
			 0x41900000,

			 /// vpu register f15
			 0x40e00000,
			 0x41f00000,
			 0x40a00000,
			 0x41e80000,
			 0x41800000,
			 0x41e00000,
			 0x42000000,
			 0x41900000,

			 /// vpu register f16
			 0x41d80000,
			 0x41400000,
			 0x40000000,
			 0x40400000,
			 0x41f00000,
			 0x42000000,
			 0x41d80000,
			 0x41000000,

			 /// vpu register f17
			 0x41800000,
			 0x41c80000,
			 0x41500000,
			 0x41100000,
			 0x41e80000,
			 0x41e80000,
			 0x41c00000,
			 0x41b80000,

			 /// vpu register f18
			 0x40c00000,
			 0x40000000,
			 0x41500000,
			 0x40e00000,
			 0x42000000,
			 0x41100000,
			 0x40e00000,
			 0x40a00000,

			 /// vpu register f19
			 0x40000000,
			 0x40400000,
			 0x41980000,
			 0x41800000,
			 0x41d80000,
			 0x40e00000,
			 0x41500000,
			 0x41d80000,

			 /// vpu register f20
			 0x41800000,
			 0x41200000,
			 0x41e00000,
			 0x41e00000,
			 0x41980000,
			 0x41700000,
			 0x41900000,
			 0x42000000,

			 /// vpu register f21
			 0x40e00000,
			 0x41e80000,
			 0x41600000,
			 0x41c00000,
			 0x42000000,
			 0x40a00000,
			 0x41100000,
			 0x41100000,

			 /// vpu register f22
			 0x40400000,
			 0x41600000,
			 0x41700000,
			 0x40c00000,
			 0x41e00000,
			 0x41500000,
			 0x41980000,
			 0x40e00000,

			 /// vpu register f23
			 0x41b00000,
			 0x41a80000,
			 0x41f00000,
			 0x41700000,
			 0x41e00000,
			 0x41a80000,
			 0x41980000,
			 0x41d80000,

			 /// vpu register f24
			 0x41c80000,
			 0x41a80000,
			 0x42000000,
			 0x41d80000,
			 0x41e00000,
			 0x40e00000,
			 0x41000000,
			 0x41e80000,

			 /// vpu register f25
			 0x41d80000,
			 0x41400000,
			 0x41d00000,
			 0x41b80000,
			 0x41c00000,
			 0x41f80000,
			 0x41b00000,
			 0x40000000,

			 /// vpu register f26
			 0x41e80000,
			 0x41a00000,
			 0x3f800000,
			 0x40a00000,
			 0x41a80000,
			 0x41400000,
			 0x3f800000,
			 0x41500000,

			 /// vpu register f27
			 0x41900000,
			 0x41400000,
			 0x41600000,
			 0x40800000,
			 0x41e80000,
			 0x41800000,
			 0x41800000,
			 0x40c00000,

			 /// vpu register f28
			 0x41e00000,
			 0x40000000,
			 0x41300000,
			 0x41a80000,
			 0x41d00000,
			 0x41d00000,
			 0x40e00000,
			 0x41f80000,

			 /// vpu register f29
			 0x41e80000,
			 0x3f800000,
			 0x41d80000,
			 0x41d00000,
			 0x40a00000,
			 0x41800000,
			 0x40800000,
			 0x41100000,

			 /// vpu register f30
			 0x40c00000,
			 0x41100000,
			 0x41c00000,
			 0x41e00000,
			 0x40c00000,
			 0x41400000,
			 0x41800000,
			 0x41980000,

			 /// vpu register f31
			 0x41000000,
			 0x42000000,
			 0x41f80000,
			 0x41d00000,
			 0x41b00000,
			 0x41200000,
			 0x40a00000,
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
		"fsrli.pi f22, f25, 9\n"                              ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f8, f6, 11\n"                               ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f6, f26, 2\n"                               ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f24, f3, 8\n"                               ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f24, f26, 15\n"                             ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f18, f11, 2\n"                              ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f9, f4, 10\n"                               ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f4, f1, 14\n"                               ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f13, f14, 6\n"                              ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f3, f16, 7\n"                               ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f22, f23, 5\n"                              ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f27, f9, 5\n"                               ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f22, f26, 11\n"                             ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f6, f0, 14\n"                               ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f13, f7, 14\n"                              ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f23, f24, 9\n"                              ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f29, f19, 2\n"                              ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f18, f30, 14\n"                             ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f17, f21, 4\n"                              ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f26, f22, 6\n"                              ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f4, f23, 10\n"                              ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f12, f11, 4\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f7, f25, 10\n"                              ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f8, f18, 8\n"                               ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f2, f17, 8\n"                               ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f29, f27, 8\n"                              ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f21, f2, 9\n"                               ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f14, f7, 3\n"                               ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f9, f13, 11\n"                              ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f23, f2, 7\n"                               ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f16, f8, 11\n"                              ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f15, f22, 3\n"                              ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f17, f21, 9\n"                              ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f8, f22, 15\n"                              ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f28, f9, 4\n"                               ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f4, f29, 13\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f22, f17, 14\n"                             ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f17, f12, 6\n"                              ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f25, f27, 0\n"                              ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f16, f29, 9\n"                              ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f9, f5, 9\n"                                ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f17, f13, 13\n"                             ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f9, f24, 7\n"                               ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f4, f5, 4\n"                                ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f21, f23, 10\n"                             ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f29, f28, 15\n"                             ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f22, f6, 10\n"                              ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f8, f21, 7\n"                               ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f14, f25, 4\n"                              ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f3, f18, 5\n"                               ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f27, f4, 9\n"                               ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f21, f5, 1\n"                               ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f30, f0, 6\n"                               ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f24, f8, 3\n"                               ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f23, f20, 0\n"                              ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f24, f1, 9\n"                               ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f7, f18, 14\n"                              ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f26, f6, 9\n"                               ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f20, f7, 4\n"                               ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f4, f11, 5\n"                               ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f22, f30, 2\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f8, f1, 4\n"                                ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f17, f30, 3\n"                              ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f5, f7, 1\n"                                ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f2, f20, 0\n"                               ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f23, f5, 14\n"                              ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f26, f26, 13\n"                             ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f13, f20, 1\n"                              ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f5, f4, 8\n"                                ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f21, f24, 14\n"                             ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f19, f17, 7\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f21, f24, 0\n"                              ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f2, f14, 14\n"                              ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f25, f8, 11\n"                              ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f8, f20, 0\n"                               ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f24, f29, 0\n"                              ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f24, f27, 14\n"                             ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f2, f2, 8\n"                                ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f3, f16, 2\n"                               ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f27, f12, 9\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f18, f17, 8\n"                              ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f8, f22, 4\n"                               ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f5, f10, 10\n"                              ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f14, f26, 0\n"                              ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f6, f16, 13\n"                              ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f29, f14, 12\n"                             ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f14, f29, 0\n"                              ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f18, f20, 6\n"                              ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f2, f22, 6\n"                               ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f27, f30, 6\n"                              ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f27, f7, 10\n"                              ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f23, f5, 11\n"                              ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f6, f27, 10\n"                              ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f17, f9, 5\n"                               ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f16, f5, 7\n"                               ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f20, f20, 13\n"                             ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f30, f1, 12\n"                              ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f22, f9, 15\n"                              ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fsrli.pi f8, f1, 14\n"                               ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
