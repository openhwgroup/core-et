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
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00000
			 0x0e000007,                                                  // Trailing 1s:                                /// 00004
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00008
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0000c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00010
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00014
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00018
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00020
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00024
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00028
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0002c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00030
			 0x0e000003,                                                  // Trailing 1s:                                /// 00034
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00038
			 0x0c700000,                                                  // Leading 1s:                                 /// 0003c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00040
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00044
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00048
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0004c
			 0x7fc00001,                                                  // signaling NaN                               /// 00050
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00054
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00058
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0005c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00060
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00064
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00068
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0006c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00070
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00074
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00078
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0007c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00080
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00084
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00088
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0008c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00090
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00094
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0009c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 000a0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000a4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000b0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000b4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000bc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000c0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000c4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000c8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000d0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 000d4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000d8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000dc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000e0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000e4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000ec
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000f0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000f8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000fc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00100
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00104
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00108
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0010c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00114
			 0x3f028f5c,                                                  // 0.51                                        /// 00118
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0011c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00120
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00124
			 0x80000000,                                                  // -zero                                       /// 00128
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0012c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00130
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00134
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00138
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0013c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00144
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0014c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00154
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00158
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00160
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00168
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0016c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00170
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00174
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00178
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0017c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00180
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00184
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00188
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0xff800000,                                                  // -inf                                        /// 00190
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00194
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0019c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001a0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 001a8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001ac
			 0x0e000003,                                                  // Trailing 1s:                                /// 001b0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001b4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001b8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001bc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 001c0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001c4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001c8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001cc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x33333333,                                                  // 4 random values                             /// 001d4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001d8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001e0
			 0xcb000000,                                                  // -8388608.0                                  /// 001e4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001e8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001ec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001f4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001f8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001fc
			 0x55555555,                                                  // 4 random values                             /// 00200
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00204
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00208
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0020c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00210
			 0xffc00001,                                                  // -signaling NaN                              /// 00214
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00218
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0021c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00220
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00224
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0022c
			 0xbf028f5c,                                                  // -0.51                                       /// 00230
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00238
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00240
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00244
			 0x0e000001,                                                  // Trailing 1s:                                /// 00248
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0024c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00250
			 0x0c700000,                                                  // Leading 1s:                                 /// 00254
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00258
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0025c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x55555555,                                                  // 4 random values                             /// 00264
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0026c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00270
			 0x00011111,                                                  // 9.7958E-41                                  /// 00274
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00278
			 0x0e000001,                                                  // Trailing 1s:                                /// 0027c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00280
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00284
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00288
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00290
			 0xbf028f5c,                                                  // -0.51                                       /// 00294
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00298
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0029c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 002a0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002a4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002a8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002ac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 002b0
			 0x4b000000,                                                  // 8388608.0                                   /// 002b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002c8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002cc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0xff800000,                                                  // -inf                                        /// 002d4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002e0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002ec
			 0x00000000,                                                  // zero                                        /// 002f0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002f4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002f8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002fc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00300
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00304
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00308
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00314
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00318
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0031c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00320
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00324
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0032c
			 0x00000000,                                                  // zero                                        /// 00330
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00334
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00338
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0033c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x0e000001,                                                  // Trailing 1s:                                /// 00348
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0xbf028f5c,                                                  // -0.51                                       /// 00350
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00354
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00358
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0035c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0xbf028f5c,                                                  // -0.51                                       /// 00364
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00368
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0036c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00370
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00374
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00378
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0037c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00380
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00384
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00388
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0038c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00390
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00394
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00398
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0039c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003a0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003a4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003a8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003ac
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003b4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003b8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003bc
			 0x0e000003,                                                  // Trailing 1s:                                /// 003c0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003c4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 003c8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003cc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003d0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003dc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 003e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 003e4
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 003e8
			 0x00000000,                                                  // zero                                        /// 003ec
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003f0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00400
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00404
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00408
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0040c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00410
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00418
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0041c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00420
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00424
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00430
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00434
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00438
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0043c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00440
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00444
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0044c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00450
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00454
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00460
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00468
			 0xcb000000,                                                  // -8388608.0                                  /// 0046c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00470
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00474
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0047c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00480
			 0x55555555,                                                  // 4 random values                             /// 00484
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00488
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0048c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00498
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0049c
			 0x7f800000,                                                  // inf                                         /// 004a0
			 0x80000000,                                                  // -zero                                       /// 004a4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 004a8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004b4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004b8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004bc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 004c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004c4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004c8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004cc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004d0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004d4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 004d8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004dc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004e0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004e4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 004e8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 004f0
			 0x0c600000,                                                  // Leading 1s:                                 /// 004f4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004fc
			 0x0c780000,                                                  // Leading 1s:                                 /// 00500
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00504
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00508
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0050c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00510
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00514
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x3f028f5c,                                                  // 0.51                                        /// 00520
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00524
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00528
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0052c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00530
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00534
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0053c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00540
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00544
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00548
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0054c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00550
			 0x80011111,                                                  // -9.7958E-41                                 /// 00554
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00558
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0055c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00560
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00564
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00568
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0056c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00574
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00578
			 0x80000000,                                                  // -zero                                       /// 0057c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00580
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00584
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00590
			 0x3f028f5c,                                                  // 0.51                                        /// 00594
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0059c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005a0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005a4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005b0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005bc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005c0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005c4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005c8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005cc
			 0xffc00001,                                                  // -signaling NaN                              /// 005d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005d4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 005d8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005dc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005e4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005f4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005f8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005fc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00600
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00604
			 0x55555555,                                                  // 4 random values                             /// 00608
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00610
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00614
			 0x0c400000,                                                  // Leading 1s:                                 /// 00618
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0061c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00620
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0062c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00630
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00634
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00638
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0063c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00640
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00644
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00648
			 0xff800000,                                                  // -inf                                        /// 0064c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00650
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00654
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00658
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0065c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00660
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00668
			 0x00000000,                                                  // zero                                        /// 0066c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00674
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00678
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0067c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00680
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00688
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0068c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00690
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x0e000001,                                                  // Trailing 1s:                                /// 00698
			 0x80011111,                                                  // -9.7958E-41                                 /// 0069c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006a8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006b8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006c0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006c8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 006cc
			 0x00000000,                                                  // zero                                        /// 006d0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006d4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 006dc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006e0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 006e8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006f4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00700
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00704
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00714
			 0x0c400000,                                                  // Leading 1s:                                 /// 00718
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00720
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00724
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00728
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0072c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x0c780000,                                                  // Leading 1s:                                 /// 00734
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00738
			 0x80011111,                                                  // -9.7958E-41                                 /// 0073c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00740
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00748
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0074c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00750
			 0xff800000,                                                  // -inf                                        /// 00754
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00758
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0075c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00760
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00764
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00768
			 0x7f800000,                                                  // inf                                         /// 0076c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x0c600000,                                                  // Leading 1s:                                 /// 00774
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00778
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0077c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00780
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00784
			 0x0c600000,                                                  // Leading 1s:                                 /// 00788
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0078c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00790
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00794
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00798
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x33333333,                                                  // 4 random values                             /// 007a0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007a4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007a8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007ac
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007b0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007bc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x55555555,                                                  // 4 random values                             /// 007c4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007c8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007d0
			 0x00000000,                                                  // zero                                        /// 007d4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007d8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007dc
			 0x80011111,                                                  // -9.7958E-41                                 /// 007e0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007e4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007e8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007ec
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007f4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007f8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007fc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00800
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00808
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0080c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00810
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00818
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00824
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00828
			 0x55555555,                                                  // 4 random values                             /// 0082c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00830
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00834
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0083c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x0e000003,                                                  // Trailing 1s:                                /// 00844
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0084c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00850
			 0xffc00001,                                                  // -signaling NaN                              /// 00854
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00858
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00860
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00864
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0086c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x0e000003,                                                  // Trailing 1s:                                /// 00874
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00878
			 0xbf028f5c,                                                  // -0.51                                       /// 0087c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00880
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00884
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00888
			 0x0c700000,                                                  // Leading 1s:                                 /// 0088c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00890
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00894
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00898
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0089c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008a0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 008b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008b8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008bc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 008c0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008c4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 008cc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008d0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 008d8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008dc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008e4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008e8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 008f0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00900
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00904
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00908
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0090c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00914
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0091c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00920
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00924
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00928
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0092c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00934
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00940
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00944
			 0x00011111,                                                  // 9.7958E-41                                  /// 00948
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0094c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00954
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00958
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0095c
			 0x4b000000,                                                  // 8388608.0                                   /// 00960
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00964
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00968
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0096c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00974
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00978
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00984
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00994
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00998
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0099c
			 0x3f028f5c,                                                  // 0.51                                        /// 009a0
			 0x0c780000,                                                  // Leading 1s:                                 /// 009a4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009a8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009b0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009b4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 009b8
			 0xffc00001,                                                  // -signaling NaN                              /// 009bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009c0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009c4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009c8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 009cc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009d0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009d4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009dc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x0c600000,                                                  // Leading 1s:                                 /// 009e4
			 0x7f800000,                                                  // inf                                         /// 009e8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009ec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009f0
			 0x4b000000,                                                  // 8388608.0                                   /// 009f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 009f8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009fc
			 0xbf028f5c,                                                  // -0.51                                       /// 00a00
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a04
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a08
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a0c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a10
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a14
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a18
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a1c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a20
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a24
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a28
			 0x80011111,                                                  // -9.7958E-41                                 /// 00a2c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a38
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a40
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00a44
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a48
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a50
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a54
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a64
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a68
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a70
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a78
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a80
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a84
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a88
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a8c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a90
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a94
			 0xbf028f5c,                                                  // -0.51                                       /// 00a98
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a9c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00aa0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00aa4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00aac
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ab0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ab4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ab8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ac0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ac4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ac8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0xffc00001,                                                  // -signaling NaN                              /// 00ad4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ad8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00adc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ae0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ae4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00aec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00af0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00af4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00af8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00afc
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00b00
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b04
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b08
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b0c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b10
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b14
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00b18
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b1c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b24
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b28
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b30
			 0x7f800000,                                                  // inf                                         /// 00b34
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b38
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b3c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b40
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b44
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b4c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b50
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b54
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b5c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00b60
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b64
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b70
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b74
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b78
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b7c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b80
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b84
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b88
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b8c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b90
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b94
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b9c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ba4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ba8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00bb0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bb4
			 0xbf028f5c,                                                  // -0.51                                       /// 00bb8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bc4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bcc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bd0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00bd4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bd8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bdc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00be0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00be4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00be8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bec
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bf0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c00
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c04
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c08
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c0c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c10
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c14
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c18
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c1c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c20
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c24
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c28
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c2c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c30
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c34
			 0x80000000,                                                  // -zero                                       /// 00c38
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c3c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c40
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c44
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c4c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c50
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c54
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c58
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c5c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c60
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c64
			 0x4b000000,                                                  // 8388608.0                                   /// 00c68
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x3f028f5c,                                                  // 0.51                                        /// 00c70
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c74
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c78
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c88
			 0x7fc00001,                                                  // signaling NaN                               /// 00c8c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c94
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c9c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ca0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ca4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ca8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00cb4
			 0x7fc00001,                                                  // signaling NaN                               /// 00cb8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cbc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cc4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00cd0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cd8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00cdc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ce0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ce4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ce8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cec
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cf4
			 0xcb000000,                                                  // -8388608.0                                  /// 00cf8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d04
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d08
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d14
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d1c
			 0x4b000000,                                                  // 8388608.0                                   /// 00d20
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d24
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d28
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d2c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d30
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d34
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d38
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d40
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d48
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d4c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d50
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00d54
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d58
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d5c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d64
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d68
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d6c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d70
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d74
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d78
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d84
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d88
			 0x00000000,                                                  // zero                                        /// 00d8c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d90
			 0x7f800000,                                                  // inf                                         /// 00d94
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d98
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d9c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00da0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00da8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00db0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00db4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00dbc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00dc0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00dc4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dc8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00dd0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00dd4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00dd8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ddc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00de4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00de8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00dec
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00df4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dfc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e00
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e04
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e08
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e0c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e10
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e14
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e18
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e1c
			 0x00000000,                                                  // zero                                        /// 00e20
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e24
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e2c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e34
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e40
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e48
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e54
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e58
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e5c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e60
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e64
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e68
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e70
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e78
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e7c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e80
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e84
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e8c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e90
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e94
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e98
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e9c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ea0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00eb4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00eb8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ebc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ec0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ec4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ec8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ecc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ed0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00ed4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00ed8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x4b000000,                                                  // 8388608.0                                   /// 00ee4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee8
			 0x7f800000,                                                  // inf                                         /// 00eec
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ef4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ef8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00efc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f00
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f0c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f10
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f14
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f18
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f1c
			 0xffc00001,                                                  // -signaling NaN                              /// 00f20
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f28
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f2c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f30
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00f34
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00f38
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f3c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f40
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f44
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f48
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f4c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f50
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f54
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00f58
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f60
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f64
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f6c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f74
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f78
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f7c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f80
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f84
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00f88
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f90
			 0x55555555,                                                  // 4 random values                             /// 00f94
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f98
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fa0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fa4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fa8
			 0x80000000,                                                  // -zero                                       /// 00fac
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00fb0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fb4
			 0x80000000,                                                  // -zero                                       /// 00fb8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fc0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fc4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fc8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00fcc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fd0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00fd4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fd8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x80000000,                                                  // -zero                                       /// 00fe0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fe4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fec
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ff0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ff8
			 0x0c7fffff                                                  // Leading 1s:                                 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00000
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00008
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0000c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x55555555,                                                  // 4 random values                             /// 00014
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0001c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x80000000,                                                  // -zero                                       /// 00024
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00028
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0002c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00034
			 0x0c400000,                                                  // Leading 1s:                                 /// 00038
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0003c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00040
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00044
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00048
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0004c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00054
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00058
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00060
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00064
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00068
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0006c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00070
			 0x0c780000,                                                  // Leading 1s:                                 /// 00074
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00078
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0007c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00084
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00088
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0008c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00094
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00098
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0009c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 000a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 000a4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 000ac
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 000b0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000b8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000bc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000c0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000cc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 000d0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000d8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000e0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000e4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000e8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 000f4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 000f8
			 0x0c780000,                                                  // Leading 1s:                                 /// 000fc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00100
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00104
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00108
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0010c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00110
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00114
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00118
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0011c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00120
			 0x80011111,                                                  // -9.7958E-41                                 /// 00124
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00128
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0012c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00130
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00134
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00138
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0013c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00144
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00148
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0014c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00150
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00154
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00158
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0015c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00168
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00170
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00174
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00178
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0017c
			 0xffc00001,                                                  // -signaling NaN                              /// 00180
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00184
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00188
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0018c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00194
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00198
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0019c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001a0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 001a8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x7f800000,                                                  // inf                                         /// 001b0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 001b8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001c0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001c8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001cc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 001d0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001d4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001dc
			 0xffc00001,                                                  // -signaling NaN                              /// 001e0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001e4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001e8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001ec
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001f0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0xcb000000,                                                  // -8388608.0                                  /// 001f8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x33333333,                                                  // 4 random values                             /// 00204
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00208
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0020c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00210
			 0xcb000000,                                                  // -8388608.0                                  /// 00214
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x55555555,                                                  // 4 random values                             /// 00220
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00224
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0022c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00230
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00234
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00238
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0023c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00240
			 0xcb000000,                                                  // -8388608.0                                  /// 00244
			 0x55555555,                                                  // 4 random values                             /// 00248
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0024c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00250
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00254
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00258
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0025c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00260
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00264
			 0x0c700000,                                                  // Leading 1s:                                 /// 00268
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0x33333333,                                                  // 4 random values                             /// 00270
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00278
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00280
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00288
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0028c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00294
			 0x00000000,                                                  // zero                                        /// 00298
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0029c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002a0
			 0xcb000000,                                                  // -8388608.0                                  /// 002a4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002ac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002b8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002bc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002c0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002c4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0xcb000000,                                                  // -8388608.0                                  /// 002cc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 002d4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002d8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002dc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002e0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x0c600000,                                                  // Leading 1s:                                 /// 002e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002ec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 002f0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002f4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002f8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002fc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00304
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00308
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00314
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00318
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00320
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00324
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00328
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0032c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00330
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00334
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00338
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00344
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00348
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0034c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00354
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00358
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0035c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00360
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00364
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00368
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00370
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0037c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00384
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00388
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00390
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00394
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x33333333,                                                  // 4 random values                             /// 0039c
			 0x33333333,                                                  // 4 random values                             /// 003a0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003a4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003a8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003ac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003bc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003c0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003c4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003c8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003cc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003d0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 003d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003e0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 003e4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003ec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003f0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003f8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003fc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00400
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00404
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00408
			 0x80000000,                                                  // -zero                                       /// 0040c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00410
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00414
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00418
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0041c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00420
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00428
			 0x0c780000,                                                  // Leading 1s:                                 /// 0042c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00430
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00434
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00438
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00440
			 0x0c700000,                                                  // Leading 1s:                                 /// 00444
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00448
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0044c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00454
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00458
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00460
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00464
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0046c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0xff800000,                                                  // -inf                                        /// 00474
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00478
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00480
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00484
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00488
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0048c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00490
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00494
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00498
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0049c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004a8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004b0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 004b4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004b8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 004bc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 004c4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004c8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004cc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x0e000003,                                                  // Trailing 1s:                                /// 004d4
			 0x00000000,                                                  // zero                                        /// 004d8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004dc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004e0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 004ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004f4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004f8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004fc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00504
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00508
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0xff800000,                                                  // -inf                                        /// 00510
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00514
			 0x80000000,                                                  // -zero                                       /// 00518
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0051c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00520
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00524
			 0x33333333,                                                  // 4 random values                             /// 00528
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0052c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00530
			 0x0c700000,                                                  // Leading 1s:                                 /// 00534
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0053c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0054c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00550
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00558
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0055c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00560
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00564
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00568
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0056c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00570
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00574
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00578
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0057c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00580
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00584
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00588
			 0x4b000000,                                                  // 8388608.0                                   /// 0058c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00590
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00594
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00598
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0059c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005a0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005a4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005a8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005b0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005b4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005b8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005bc
			 0xcb000000,                                                  // -8388608.0                                  /// 005c0
			 0x3f028f5c,                                                  // 0.51                                        /// 005c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005c8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005cc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 005d4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005d8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 005dc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005e0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x4b000000,                                                  // 8388608.0                                   /// 005f0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005f8
			 0xcb000000,                                                  // -8388608.0                                  /// 005fc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00600
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0060c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00610
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00618
			 0x00011111,                                                  // 9.7958E-41                                  /// 0061c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00620
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00624
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0062c
			 0xffc00001,                                                  // -signaling NaN                              /// 00630
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00638
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0063c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00640
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00644
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00650
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00654
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0065c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00660
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00664
			 0x80011111,                                                  // -9.7958E-41                                 /// 00668
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0066c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00670
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00674
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00678
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0067c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00680
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00684
			 0xcb000000,                                                  // -8388608.0                                  /// 00688
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00694
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00698
			 0x0c780000,                                                  // Leading 1s:                                 /// 0069c
			 0x7fc00001,                                                  // signaling NaN                               /// 006a0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006a4
			 0xcb000000,                                                  // -8388608.0                                  /// 006a8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006ac
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006b4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006b8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006c0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006c4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006c8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006cc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 006d0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006d4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 006dc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006e0
			 0x00000000,                                                  // zero                                        /// 006e4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006e8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006ec
			 0xcb000000,                                                  // -8388608.0                                  /// 006f0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006f4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 006f8
			 0x0c600000,                                                  // Leading 1s:                                 /// 006fc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00704
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0070c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00710
			 0x0c400000,                                                  // Leading 1s:                                 /// 00714
			 0x7fc00001,                                                  // signaling NaN                               /// 00718
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0071c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00720
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00724
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00728
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0072c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0xffc00001,                                                  // -signaling NaN                              /// 00734
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00738
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0073c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00740
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0074c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00750
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00754
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00758
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0075c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00760
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00764
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x0c700000,                                                  // Leading 1s:                                 /// 0076c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00770
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00774
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0077c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00784
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00788
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0078c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x0e000003,                                                  // Trailing 1s:                                /// 00794
			 0x0c600000,                                                  // Leading 1s:                                 /// 00798
			 0x0c780000,                                                  // Leading 1s:                                 /// 0079c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 007a0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007a4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007a8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007ac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007bc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007c0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007c4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0xbf028f5c,                                                  // -0.51                                       /// 007cc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007d0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007d4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007d8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007dc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 007e0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007ec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007f0
			 0x0e000003,                                                  // Trailing 1s:                                /// 007f4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007f8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007fc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00800
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00804
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00808
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0080c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00810
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00814
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00818
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0081c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00820
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00824
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00828
			 0x3f028f5c,                                                  // 0.51                                        /// 0082c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00830
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00834
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00838
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0083c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00840
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0084c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00850
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00854
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00858
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0085c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00860
			 0x7f800000,                                                  // inf                                         /// 00864
			 0x33333333,                                                  // 4 random values                             /// 00868
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0086c
			 0xffc00001,                                                  // -signaling NaN                              /// 00870
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x80011111,                                                  // -9.7958E-41                                 /// 00878
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0087c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00880
			 0xff800000,                                                  // -inf                                        /// 00884
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00890
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00894
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00898
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0089c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008a4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008a8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008ac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008b0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008b4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008b8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008c0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008c4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008c8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008cc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008d0
			 0x3f028f5c,                                                  // 0.51                                        /// 008d4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 008d8
			 0x0c400000,                                                  // Leading 1s:                                 /// 008dc
			 0x33333333,                                                  // 4 random values                             /// 008e0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008e8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008ec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008f0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008f8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008fc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00904
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00908
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0090c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00910
			 0x0e000001,                                                  // Trailing 1s:                                /// 00914
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0091c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00920
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00924
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00928
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00930
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00938
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0093c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00944
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00948
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0094c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00950
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00958
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0095c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00960
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00968
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0096c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00970
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0097c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00980
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00988
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0098c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00990
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00998
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0099c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009a0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 009a4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 009ac
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 009b0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009b4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009b8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009bc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 009c4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009c8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x80000000,                                                  // -zero                                       /// 009d8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009dc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 009e0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 009e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009e8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 009ec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009f0
			 0x33333333,                                                  // 4 random values                             /// 009f4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009fc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a04
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a08
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a0c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a10
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a18
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x3f028f5c,                                                  // 0.51                                        /// 00a24
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a28
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a2c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a30
			 0xffc00001,                                                  // -signaling NaN                              /// 00a34
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a38
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a3c
			 0x55555555,                                                  // 4 random values                             /// 00a40
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a44
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a4c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a50
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a58
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a5c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a60
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00a64
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a68
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a6c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a70
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a74
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a7c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a80
			 0x00000000,                                                  // zero                                        /// 00a84
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a88
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a8c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a90
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a94
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a98
			 0x0c700000,                                                  // Leading 1s:                                 /// 00a9c
			 0x80000000,                                                  // -zero                                       /// 00aa0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00aa4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00aa8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00aac
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ab8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x55555555,                                                  // 4 random values                             /// 00ac0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ac4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ac8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00acc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ad4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ad8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ae0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ae4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00ae8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00af0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00af4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0xffc00001,                                                  // -signaling NaN                              /// 00afc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b00
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b04
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b08
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00b0c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b10
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b14
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b18
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b20
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b28
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b2c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b30
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b34
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b38
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b3c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b40
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b48
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b4c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b50
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b54
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b58
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00b5c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b60
			 0x33333333,                                                  // 4 random values                             /// 00b64
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b68
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b6c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b70
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b78
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00b7c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b80
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b84
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00b8c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b90
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b94
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b9c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ba0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ba4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ba8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bac
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bb0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00bb4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bb8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bbc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bc0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bc4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bc8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bd0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bd4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bdc
			 0xcb000000,                                                  // -8388608.0                                  /// 00be0
			 0x55555555,                                                  // 4 random values                             /// 00be4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00be8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00bf0
			 0xcb000000,                                                  // -8388608.0                                  /// 00bf4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bf8
			 0x80000000,                                                  // -zero                                       /// 00bfc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c04
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c08
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c10
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c14
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c1c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c24
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c28
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c2c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c30
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c38
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c3c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c40
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c44
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c48
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c4c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c50
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c54
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c58
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c5c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c60
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c68
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c6c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c70
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c74
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c78
			 0x7fc00001,                                                  // signaling NaN                               /// 00c7c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c80
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c84
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c88
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c8c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c90
			 0xbf028f5c,                                                  // -0.51                                       /// 00c94
			 0xcb000000,                                                  // -8388608.0                                  /// 00c98
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c9c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ca0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ca4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00cac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00cb0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00cb4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00cb8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cbc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cc0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cc4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cc8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ccc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cd0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cdc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00ce4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ce8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cec
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cf0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cf4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cf8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cfc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d00
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d04
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d08
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d0c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d14
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d18
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d1c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d20
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d24
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d2c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d30
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00d34
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d38
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d3c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d40
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d4c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d54
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d68
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d70
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d78
			 0x7f800000,                                                  // inf                                         /// 00d7c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d80
			 0xcb000000,                                                  // -8388608.0                                  /// 00d84
			 0x0c780000,                                                  // Leading 1s:                                 /// 00d88
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d8c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00d94
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00d98
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d9c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00da0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00da4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00da8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00db0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00db4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00dbc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dc0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00dcc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00dd0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00dd4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00dd8
			 0xbf028f5c,                                                  // -0.51                                       /// 00ddc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00de4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00de8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00dec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00df0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00df4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00df8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dfc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e00
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e04
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e0c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e10
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e14
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e18
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e1c
			 0x33333333,                                                  // 4 random values                             /// 00e20
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e24
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e28
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e2c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e30
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e38
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e40
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e44
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e48
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e4c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e50
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e54
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e60
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0xffc00001,                                                  // -signaling NaN                              /// 00e68
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e6c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e70
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e78
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e7c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e80
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e84
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e88
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e90
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e94
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e98
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00e9c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ea0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ea4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00eac
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00eb4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00eb8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ebc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ec0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ec4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ec8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ecc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ed8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00edc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ee4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ee8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00eec
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ef4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ef8
			 0xffc00001,                                                  // -signaling NaN                              /// 00efc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f00
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f04
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f08
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f0c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f10
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f14
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f18
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f1c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f20
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f24
			 0xbf028f5c,                                                  // -0.51                                       /// 00f28
			 0xcb000000,                                                  // -8388608.0                                  /// 00f2c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f34
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f38
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f3c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f40
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f44
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f48
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f4c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f50
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f54
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f58
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f5c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f64
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f6c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f70
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f78
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f84
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f90
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f94
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f98
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f9c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00fa0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fa4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fa8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00fac
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fb0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fb4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fb8
			 0x4b000000,                                                  // 8388608.0                                   /// 00fbc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fc0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fc4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fc8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fcc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00fd0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fd4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fdc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00fe4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00fe8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00fec
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ff0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ff4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ff8
			 0x00020000                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x6cec8451, /// 0x0
			 0x052365aa, /// 0x4
			 0x298f565b, /// 0x8
			 0x15e7ce8e, /// 0xc
			 0x36504138, /// 0x10
			 0x3da4def7, /// 0x14
			 0xa20b2a45, /// 0x18
			 0xb656808a, /// 0x1c
			 0x2a4ea547, /// 0x20
			 0xc3498840, /// 0x24
			 0xf860b17e, /// 0x28
			 0xc15cb67b, /// 0x2c
			 0x99281b7f, /// 0x30
			 0x4324df81, /// 0x34
			 0xc30fb696, /// 0x38
			 0xf926dddb, /// 0x3c
			 0x83a7d79b, /// 0x40
			 0x9d4d2d60, /// 0x44
			 0xf909ec5a, /// 0x48
			 0x7f86d639, /// 0x4c
			 0x5d3a22df, /// 0x50
			 0xb7ab406f, /// 0x54
			 0x7bde4318, /// 0x58
			 0x998086d4, /// 0x5c
			 0x4c5064b0, /// 0x60
			 0x9c7cecad, /// 0x64
			 0x737560f4, /// 0x68
			 0xa4c8d67a, /// 0x6c
			 0xc2c5829c, /// 0x70
			 0xe8194f7e, /// 0x74
			 0xc67f1891, /// 0x78
			 0x70c1158b, /// 0x7c
			 0xc97ea068, /// 0x80
			 0x50758cc1, /// 0x84
			 0x62c26b61, /// 0x88
			 0xb09e64f3, /// 0x8c
			 0xbcad57d1, /// 0x90
			 0x7d0708e6, /// 0x94
			 0x5664ed09, /// 0x98
			 0x695cce6c, /// 0x9c
			 0x553b19fd, /// 0xa0
			 0xc2ab4d46, /// 0xa4
			 0xf949cf16, /// 0xa8
			 0x5721b7ae, /// 0xac
			 0xcc95c7e4, /// 0xb0
			 0x859f2136, /// 0xb4
			 0x2fdacf6d, /// 0xb8
			 0x25a32919, /// 0xbc
			 0x2f78eaa4, /// 0xc0
			 0x9489b6c6, /// 0xc4
			 0x5b674798, /// 0xc8
			 0xfae52bf6, /// 0xcc
			 0xdf56f9e6, /// 0xd0
			 0x8d0f89a7, /// 0xd4
			 0x30f1a36a, /// 0xd8
			 0xdc270255, /// 0xdc
			 0x3c23bf6e, /// 0xe0
			 0x9ed65112, /// 0xe4
			 0x1b579675, /// 0xe8
			 0x0140c1b6, /// 0xec
			 0x2f1f859e, /// 0xf0
			 0xf0c423ca, /// 0xf4
			 0xef01e2f3, /// 0xf8
			 0xc56d42e2, /// 0xfc
			 0x92055474, /// 0x100
			 0xa97647ce, /// 0x104
			 0x3348409a, /// 0x108
			 0x3b628b3c, /// 0x10c
			 0x5861e679, /// 0x110
			 0xa11692bd, /// 0x114
			 0x5d6c690d, /// 0x118
			 0x378961e0, /// 0x11c
			 0x690e7260, /// 0x120
			 0x573ab031, /// 0x124
			 0xa0353dc9, /// 0x128
			 0x7537af9e, /// 0x12c
			 0xe0745bdc, /// 0x130
			 0xc0997ce7, /// 0x134
			 0x06ea23f8, /// 0x138
			 0xcb906115, /// 0x13c
			 0x63c0d19c, /// 0x140
			 0xaa9e6d31, /// 0x144
			 0xb9686c4f, /// 0x148
			 0x98043f5e, /// 0x14c
			 0x45556831, /// 0x150
			 0xb900285d, /// 0x154
			 0x01da64af, /// 0x158
			 0xe30e10d6, /// 0x15c
			 0x973555fd, /// 0x160
			 0x8b56e0ad, /// 0x164
			 0xb1fea763, /// 0x168
			 0x51a5a383, /// 0x16c
			 0xa1210bdd, /// 0x170
			 0x384dbadc, /// 0x174
			 0x4c7f590b, /// 0x178
			 0x5e08fa35, /// 0x17c
			 0x04bdc0fe, /// 0x180
			 0x768eec5f, /// 0x184
			 0x5b340bbd, /// 0x188
			 0x21abcf45, /// 0x18c
			 0xe28e47eb, /// 0x190
			 0x294255b1, /// 0x194
			 0xc8990a39, /// 0x198
			 0xd10fc323, /// 0x19c
			 0xdc421529, /// 0x1a0
			 0x8841da65, /// 0x1a4
			 0xa3d1d07a, /// 0x1a8
			 0x3ccc9802, /// 0x1ac
			 0x8e88c765, /// 0x1b0
			 0x7b371768, /// 0x1b4
			 0x0d9b18cc, /// 0x1b8
			 0x68fb91a2, /// 0x1bc
			 0xcc6266a6, /// 0x1c0
			 0x9586607e, /// 0x1c4
			 0xdb198c38, /// 0x1c8
			 0xc12fdd9a, /// 0x1cc
			 0x498b6442, /// 0x1d0
			 0xb829e8ca, /// 0x1d4
			 0x1ac959a5, /// 0x1d8
			 0x25e38eba, /// 0x1dc
			 0x9fdcfba0, /// 0x1e0
			 0x45dce610, /// 0x1e4
			 0xe298cd9f, /// 0x1e8
			 0xb9cb9e95, /// 0x1ec
			 0xea2b8247, /// 0x1f0
			 0x573d30a7, /// 0x1f4
			 0x66a1d32f, /// 0x1f8
			 0x04028707, /// 0x1fc
			 0x47b8db04, /// 0x200
			 0x192dff16, /// 0x204
			 0xf1df91a3, /// 0x208
			 0x1c1951be, /// 0x20c
			 0x4a5986be, /// 0x210
			 0x48517f23, /// 0x214
			 0x36083cd8, /// 0x218
			 0x2edd2614, /// 0x21c
			 0xfe5646fc, /// 0x220
			 0x72db4aa2, /// 0x224
			 0x48a78d00, /// 0x228
			 0xc5d4e9de, /// 0x22c
			 0xd99a9437, /// 0x230
			 0xb6cb89b4, /// 0x234
			 0xaa1d10ed, /// 0x238
			 0xf9d7af51, /// 0x23c
			 0x24bd7498, /// 0x240
			 0x45c52c1a, /// 0x244
			 0xbd7d6abf, /// 0x248
			 0xb03dd78d, /// 0x24c
			 0x715872db, /// 0x250
			 0xd04eb4ca, /// 0x254
			 0x2d460533, /// 0x258
			 0xde393a51, /// 0x25c
			 0xde1995b1, /// 0x260
			 0x5df2edec, /// 0x264
			 0x16097da3, /// 0x268
			 0x8d39577b, /// 0x26c
			 0x22c15a4e, /// 0x270
			 0x57cc1f6a, /// 0x274
			 0x093d4a45, /// 0x278
			 0xecd47391, /// 0x27c
			 0x6e8a19b0, /// 0x280
			 0x804aba15, /// 0x284
			 0x292a4a98, /// 0x288
			 0x00e457e5, /// 0x28c
			 0xef7777f9, /// 0x290
			 0x68aeea36, /// 0x294
			 0x881e0f57, /// 0x298
			 0xc181ca01, /// 0x29c
			 0x0c774d8f, /// 0x2a0
			 0x92ced87c, /// 0x2a4
			 0x1ba73a21, /// 0x2a8
			 0x3198710f, /// 0x2ac
			 0xd4d0ae1b, /// 0x2b0
			 0xabe51d95, /// 0x2b4
			 0x434e1dba, /// 0x2b8
			 0x87f9343a, /// 0x2bc
			 0x2e7b3aca, /// 0x2c0
			 0xf18e793f, /// 0x2c4
			 0x66025962, /// 0x2c8
			 0xcbe3e7b3, /// 0x2cc
			 0xd7801cdb, /// 0x2d0
			 0x2afe2138, /// 0x2d4
			 0xd4ae8887, /// 0x2d8
			 0x7f63a585, /// 0x2dc
			 0x568dfb4f, /// 0x2e0
			 0xecf8d117, /// 0x2e4
			 0x3bf26819, /// 0x2e8
			 0x372ceb21, /// 0x2ec
			 0x6c6d848b, /// 0x2f0
			 0x6db1f50c, /// 0x2f4
			 0x079d4f2f, /// 0x2f8
			 0x11d9e285, /// 0x2fc
			 0x1caf5340, /// 0x300
			 0xc35bb3ad, /// 0x304
			 0xd74dde13, /// 0x308
			 0x4996da8b, /// 0x30c
			 0x553d08c4, /// 0x310
			 0xa168c3a0, /// 0x314
			 0x590f536d, /// 0x318
			 0x90d0fdb1, /// 0x31c
			 0xac3c1eb4, /// 0x320
			 0xb36e6cfc, /// 0x324
			 0x09881a51, /// 0x328
			 0xf32467d4, /// 0x32c
			 0x221d4c84, /// 0x330
			 0x297620b0, /// 0x334
			 0x23840185, /// 0x338
			 0x183aba2c, /// 0x33c
			 0x94ac06f3, /// 0x340
			 0xfe43f864, /// 0x344
			 0xd7a5344c, /// 0x348
			 0x4d9cbd09, /// 0x34c
			 0x16aff73b, /// 0x350
			 0xa87d5799, /// 0x354
			 0x96486ec3, /// 0x358
			 0xd92e835b, /// 0x35c
			 0xdc90410f, /// 0x360
			 0xf30634b6, /// 0x364
			 0x855a7d2f, /// 0x368
			 0x413b9701, /// 0x36c
			 0x0d092af1, /// 0x370
			 0x38830b63, /// 0x374
			 0xa99d444f, /// 0x378
			 0x33c8ca28, /// 0x37c
			 0xb38bf808, /// 0x380
			 0x11e1322e, /// 0x384
			 0x789944f2, /// 0x388
			 0xd61a409c, /// 0x38c
			 0xa79add1d, /// 0x390
			 0x1229973d, /// 0x394
			 0x03942033, /// 0x398
			 0x8ff7c90e, /// 0x39c
			 0x82358869, /// 0x3a0
			 0x8b74ddff, /// 0x3a4
			 0x198eb591, /// 0x3a8
			 0x579dfef4, /// 0x3ac
			 0x538dbae2, /// 0x3b0
			 0xea0b834a, /// 0x3b4
			 0xc52e62a1, /// 0x3b8
			 0x537e6928, /// 0x3bc
			 0x4e40a134, /// 0x3c0
			 0x860eff57, /// 0x3c4
			 0xefeacd44, /// 0x3c8
			 0x9a82bfe8, /// 0x3cc
			 0xeb9bc489, /// 0x3d0
			 0xfc0183ea, /// 0x3d4
			 0x147b6243, /// 0x3d8
			 0x7f228718, /// 0x3dc
			 0x405e8c40, /// 0x3e0
			 0xca3db6be, /// 0x3e4
			 0x4830fc9e, /// 0x3e8
			 0x838fb2d0, /// 0x3ec
			 0x76f6e3f1, /// 0x3f0
			 0x1a20e1d8, /// 0x3f4
			 0x1a81841e, /// 0x3f8
			 0x85394ad3, /// 0x3fc
			 0x13261741, /// 0x400
			 0x58900e1c, /// 0x404
			 0x66f77a06, /// 0x408
			 0x607a129f, /// 0x40c
			 0xbbcb1edb, /// 0x410
			 0x47e45fce, /// 0x414
			 0xa1856870, /// 0x418
			 0x4a398ce0, /// 0x41c
			 0x2281ce98, /// 0x420
			 0x4d2801d7, /// 0x424
			 0xba225a80, /// 0x428
			 0xbaa2e234, /// 0x42c
			 0x464e500b, /// 0x430
			 0x73b8d3e9, /// 0x434
			 0x2e145d43, /// 0x438
			 0x2da02ab0, /// 0x43c
			 0x1785ebce, /// 0x440
			 0x322c89e4, /// 0x444
			 0x4ac81981, /// 0x448
			 0x6fd130b8, /// 0x44c
			 0x690f2686, /// 0x450
			 0x6fe73221, /// 0x454
			 0x1dacce8b, /// 0x458
			 0x65bd02d7, /// 0x45c
			 0x93426981, /// 0x460
			 0xbe3abba3, /// 0x464
			 0x47c11565, /// 0x468
			 0x57b14d27, /// 0x46c
			 0xaecd0dac, /// 0x470
			 0x2dc88e91, /// 0x474
			 0x542f6d0a, /// 0x478
			 0xe3550d95, /// 0x47c
			 0xd6c046ac, /// 0x480
			 0xc5923d32, /// 0x484
			 0xcc0b71b1, /// 0x488
			 0x0b024139, /// 0x48c
			 0xf52b2ecc, /// 0x490
			 0x774874aa, /// 0x494
			 0x898aa9bc, /// 0x498
			 0xab9ddac2, /// 0x49c
			 0x291230a6, /// 0x4a0
			 0xc02bf7d0, /// 0x4a4
			 0xf5735e82, /// 0x4a8
			 0xa4b7b5d8, /// 0x4ac
			 0xc64afffb, /// 0x4b0
			 0xe482682a, /// 0x4b4
			 0x83e61fa9, /// 0x4b8
			 0x48f47861, /// 0x4bc
			 0xadd667b7, /// 0x4c0
			 0x8fea375b, /// 0x4c4
			 0x3e26aec3, /// 0x4c8
			 0x0f3237e6, /// 0x4cc
			 0x50f2bc2a, /// 0x4d0
			 0x5b2819ee, /// 0x4d4
			 0x8c263749, /// 0x4d8
			 0x7561056e, /// 0x4dc
			 0x812d110d, /// 0x4e0
			 0x7450a1a0, /// 0x4e4
			 0xf08fc562, /// 0x4e8
			 0x81901cc5, /// 0x4ec
			 0x4629c989, /// 0x4f0
			 0xb46a6af0, /// 0x4f4
			 0x5940c404, /// 0x4f8
			 0xb7b31e44, /// 0x4fc
			 0xeddef507, /// 0x500
			 0xa2695d70, /// 0x504
			 0x8d493420, /// 0x508
			 0x0b9df972, /// 0x50c
			 0x637b25ca, /// 0x510
			 0x8f38e560, /// 0x514
			 0xd464ee3c, /// 0x518
			 0x8cb62d96, /// 0x51c
			 0x0989de58, /// 0x520
			 0xccded99b, /// 0x524
			 0x64955215, /// 0x528
			 0x57a2be72, /// 0x52c
			 0x9277b2d9, /// 0x530
			 0xb255a6bb, /// 0x534
			 0xd0e26a92, /// 0x538
			 0xcea2847b, /// 0x53c
			 0x314f6aeb, /// 0x540
			 0x6f24954b, /// 0x544
			 0x9497a65e, /// 0x548
			 0x2bf811a5, /// 0x54c
			 0xf267fceb, /// 0x550
			 0xe69b0bbe, /// 0x554
			 0x2541dc92, /// 0x558
			 0x74275b08, /// 0x55c
			 0x482ec6ce, /// 0x560
			 0xc1fef673, /// 0x564
			 0xb8e2d4b9, /// 0x568
			 0xda3153f3, /// 0x56c
			 0xefe786ac, /// 0x570
			 0x782a8236, /// 0x574
			 0x9061031c, /// 0x578
			 0x18d34984, /// 0x57c
			 0xf05e525c, /// 0x580
			 0xc0a2ed43, /// 0x584
			 0x6b218773, /// 0x588
			 0x515a283b, /// 0x58c
			 0x14d5a325, /// 0x590
			 0x058f1227, /// 0x594
			 0x7035b56b, /// 0x598
			 0xb2730eea, /// 0x59c
			 0xf2bf2b38, /// 0x5a0
			 0x0f590dc7, /// 0x5a4
			 0x3e50230a, /// 0x5a8
			 0xe4e31fe0, /// 0x5ac
			 0xbfc7e36b, /// 0x5b0
			 0x9fe72f56, /// 0x5b4
			 0x8840930c, /// 0x5b8
			 0xbb1eeee0, /// 0x5bc
			 0x40a75fb8, /// 0x5c0
			 0xae1b58f0, /// 0x5c4
			 0xea952b1e, /// 0x5c8
			 0xe123c433, /// 0x5cc
			 0xb0e3a77e, /// 0x5d0
			 0x3953d121, /// 0x5d4
			 0x79118e44, /// 0x5d8
			 0x875d944e, /// 0x5dc
			 0x9fa0cc6a, /// 0x5e0
			 0xe64a7424, /// 0x5e4
			 0xbcb27e3d, /// 0x5e8
			 0x18660b4f, /// 0x5ec
			 0x6c71c7c2, /// 0x5f0
			 0x0948940b, /// 0x5f4
			 0xcf91306f, /// 0x5f8
			 0x78783441, /// 0x5fc
			 0xf623d6c7, /// 0x600
			 0x454f532a, /// 0x604
			 0x74f7cba8, /// 0x608
			 0xf2abd73c, /// 0x60c
			 0xdedb71ce, /// 0x610
			 0x66b2124e, /// 0x614
			 0x5a069e43, /// 0x618
			 0x7c28c5f7, /// 0x61c
			 0x6b937e19, /// 0x620
			 0xd682a5ed, /// 0x624
			 0x862261c4, /// 0x628
			 0x0fc37471, /// 0x62c
			 0x768bfb1a, /// 0x630
			 0x8b0eea5f, /// 0x634
			 0x16e69a32, /// 0x638
			 0x83484ef9, /// 0x63c
			 0xffe77144, /// 0x640
			 0xb0d5445f, /// 0x644
			 0x368cade2, /// 0x648
			 0x96d21aaf, /// 0x64c
			 0x60a49c9a, /// 0x650
			 0x03584c4c, /// 0x654
			 0xc74d46fd, /// 0x658
			 0xe1222aca, /// 0x65c
			 0x57bfdb11, /// 0x660
			 0x782a4ab5, /// 0x664
			 0x7c4128d1, /// 0x668
			 0xc96c3d68, /// 0x66c
			 0xb13a573f, /// 0x670
			 0x5e9c3729, /// 0x674
			 0x85842b61, /// 0x678
			 0x078cf42d, /// 0x67c
			 0xa845702b, /// 0x680
			 0x1c566e80, /// 0x684
			 0x0b929174, /// 0x688
			 0x8c9c7802, /// 0x68c
			 0x469e30ca, /// 0x690
			 0x4a92dabe, /// 0x694
			 0x30a7bd49, /// 0x698
			 0xb59beb8e, /// 0x69c
			 0xffb3ea57, /// 0x6a0
			 0x30dfe264, /// 0x6a4
			 0xf5b4f221, /// 0x6a8
			 0xebeba99c, /// 0x6ac
			 0x57bf211c, /// 0x6b0
			 0x120364c3, /// 0x6b4
			 0x02bfc749, /// 0x6b8
			 0x087eec3d, /// 0x6bc
			 0xacf678af, /// 0x6c0
			 0x7351d7ce, /// 0x6c4
			 0xa3eb8087, /// 0x6c8
			 0xc8f0d717, /// 0x6cc
			 0xe83a4f92, /// 0x6d0
			 0xa3876cdd, /// 0x6d4
			 0xcde4b90a, /// 0x6d8
			 0x4f238e77, /// 0x6dc
			 0xa3a84928, /// 0x6e0
			 0x533ac5b3, /// 0x6e4
			 0x5027db49, /// 0x6e8
			 0xdfc1a6ae, /// 0x6ec
			 0xc972b684, /// 0x6f0
			 0xb697a4f4, /// 0x6f4
			 0xe03694e1, /// 0x6f8
			 0x994cb798, /// 0x6fc
			 0x59262bdd, /// 0x700
			 0x0ba1339f, /// 0x704
			 0x062635e8, /// 0x708
			 0x7d9d2a5f, /// 0x70c
			 0x4a6f266f, /// 0x710
			 0xc5dd03d6, /// 0x714
			 0x5fb93577, /// 0x718
			 0x7a0d2b9c, /// 0x71c
			 0x0d14e0f5, /// 0x720
			 0x8716e46a, /// 0x724
			 0x76703cad, /// 0x728
			 0x70bcc4e0, /// 0x72c
			 0x6b24a582, /// 0x730
			 0x08f17d7b, /// 0x734
			 0xbaf05b95, /// 0x738
			 0xad004795, /// 0x73c
			 0xd64d4902, /// 0x740
			 0x2ad0cc02, /// 0x744
			 0x426f2315, /// 0x748
			 0xd1645732, /// 0x74c
			 0x7220b8b9, /// 0x750
			 0x9fdc6449, /// 0x754
			 0x4648417f, /// 0x758
			 0x592bbae4, /// 0x75c
			 0x46abfc17, /// 0x760
			 0x6a7fe1d7, /// 0x764
			 0xe1ec7735, /// 0x768
			 0x696dcf57, /// 0x76c
			 0xfd0c0c2b, /// 0x770
			 0xf9441a3c, /// 0x774
			 0x15b7a9a7, /// 0x778
			 0xf45fb437, /// 0x77c
			 0xd300d453, /// 0x780
			 0x83f7c8e9, /// 0x784
			 0x68ebaa33, /// 0x788
			 0xbc32a461, /// 0x78c
			 0xf40effd1, /// 0x790
			 0x7e0cd86d, /// 0x794
			 0x201b19b9, /// 0x798
			 0x274e5167, /// 0x79c
			 0xb041c5bc, /// 0x7a0
			 0x3ff9f6bb, /// 0x7a4
			 0x476f2219, /// 0x7a8
			 0x191bac7c, /// 0x7ac
			 0x45f5db23, /// 0x7b0
			 0xb46e7f86, /// 0x7b4
			 0xe08058ff, /// 0x7b8
			 0x7f1381b4, /// 0x7bc
			 0x8ef1f2dc, /// 0x7c0
			 0x03d1009b, /// 0x7c4
			 0x89d70a69, /// 0x7c8
			 0x705a64c9, /// 0x7cc
			 0x6dfb7796, /// 0x7d0
			 0xaafa44b8, /// 0x7d4
			 0xb8e28c6c, /// 0x7d8
			 0x46a097fe, /// 0x7dc
			 0x18676672, /// 0x7e0
			 0x5d89a11d, /// 0x7e4
			 0x9d976fdc, /// 0x7e8
			 0xda5947bd, /// 0x7ec
			 0x10d0e9cf, /// 0x7f0
			 0x549d9d10, /// 0x7f4
			 0x351ec31f, /// 0x7f8
			 0x85f3a8b8, /// 0x7fc
			 0x869178e1, /// 0x800
			 0xbd0e5b3c, /// 0x804
			 0x63480c43, /// 0x808
			 0x16880c0c, /// 0x80c
			 0xc25d6681, /// 0x810
			 0x26428699, /// 0x814
			 0x17a6e751, /// 0x818
			 0x11fda182, /// 0x81c
			 0x2b5eda87, /// 0x820
			 0x0c46696a, /// 0x824
			 0x68d8d378, /// 0x828
			 0x4563f1f5, /// 0x82c
			 0x9423d23c, /// 0x830
			 0x4907bc08, /// 0x834
			 0xd49e45cb, /// 0x838
			 0x863cd6e8, /// 0x83c
			 0xc61a55f1, /// 0x840
			 0x9f4f0223, /// 0x844
			 0x60eda18d, /// 0x848
			 0x80c0a30f, /// 0x84c
			 0x4fdced8a, /// 0x850
			 0x57eb948a, /// 0x854
			 0xddf282b9, /// 0x858
			 0x273c0830, /// 0x85c
			 0x315c9ac8, /// 0x860
			 0xe22ab0fb, /// 0x864
			 0x3c5b8b6a, /// 0x868
			 0x0937da4d, /// 0x86c
			 0x2211e855, /// 0x870
			 0xeb815a08, /// 0x874
			 0x953befe6, /// 0x878
			 0xa85dd3fe, /// 0x87c
			 0x2e2b9170, /// 0x880
			 0x34d03156, /// 0x884
			 0x758eb1be, /// 0x888
			 0x0ab004ec, /// 0x88c
			 0xafe3fd01, /// 0x890
			 0x2c43d172, /// 0x894
			 0x92d296d1, /// 0x898
			 0xf5a2b293, /// 0x89c
			 0x14bc0b55, /// 0x8a0
			 0xa85d5ed2, /// 0x8a4
			 0xf1c75a35, /// 0x8a8
			 0x878dcd67, /// 0x8ac
			 0x780a3e8f, /// 0x8b0
			 0x81f9d65d, /// 0x8b4
			 0xe781b131, /// 0x8b8
			 0xc70f932e, /// 0x8bc
			 0x34b54d25, /// 0x8c0
			 0x7fc30e1c, /// 0x8c4
			 0xbd4d71f9, /// 0x8c8
			 0xb3e76171, /// 0x8cc
			 0xe946e813, /// 0x8d0
			 0x3103952f, /// 0x8d4
			 0xab91160b, /// 0x8d8
			 0xf74bbc00, /// 0x8dc
			 0x4dafa8e1, /// 0x8e0
			 0x9ea55abc, /// 0x8e4
			 0xc0ff845e, /// 0x8e8
			 0xa8cc593d, /// 0x8ec
			 0x2eaddf32, /// 0x8f0
			 0x42ec81d0, /// 0x8f4
			 0x58a458a7, /// 0x8f8
			 0xf6d9cbc9, /// 0x8fc
			 0x67bcf2e0, /// 0x900
			 0x61915eeb, /// 0x904
			 0x28ec75e5, /// 0x908
			 0x4b3d6883, /// 0x90c
			 0xa4ea8df6, /// 0x910
			 0xeff9bfd1, /// 0x914
			 0x9670ec9c, /// 0x918
			 0xe4d52994, /// 0x91c
			 0x7e9e19fc, /// 0x920
			 0xccc8590e, /// 0x924
			 0x68bb977a, /// 0x928
			 0x009a576a, /// 0x92c
			 0x722c77ca, /// 0x930
			 0x7e4d242d, /// 0x934
			 0xdb0cab70, /// 0x938
			 0x4908062b, /// 0x93c
			 0x36f01d63, /// 0x940
			 0xc45b90b4, /// 0x944
			 0x390451a4, /// 0x948
			 0xf90ffc70, /// 0x94c
			 0x8c1c411e, /// 0x950
			 0x4f8cd9e9, /// 0x954
			 0xcde8ccf4, /// 0x958
			 0x0542ee97, /// 0x95c
			 0x2f948aab, /// 0x960
			 0x012fab4a, /// 0x964
			 0x65270167, /// 0x968
			 0x0e24d5c7, /// 0x96c
			 0x47154f41, /// 0x970
			 0x63fac651, /// 0x974
			 0x9a70b358, /// 0x978
			 0xd7348968, /// 0x97c
			 0x2b82cd8d, /// 0x980
			 0x81659f1d, /// 0x984
			 0xe53d365d, /// 0x988
			 0xf66204a5, /// 0x98c
			 0x020b79a5, /// 0x990
			 0x05a5ca6e, /// 0x994
			 0x1499bb23, /// 0x998
			 0x66ebc63f, /// 0x99c
			 0x8a71dd15, /// 0x9a0
			 0x41ded070, /// 0x9a4
			 0x2e166e18, /// 0x9a8
			 0x99bb197d, /// 0x9ac
			 0xbb14f064, /// 0x9b0
			 0x40021ff5, /// 0x9b4
			 0xcc96589f, /// 0x9b8
			 0xc2ae3efa, /// 0x9bc
			 0x26ec425d, /// 0x9c0
			 0x267f0503, /// 0x9c4
			 0xce19b275, /// 0x9c8
			 0x65e8f9d6, /// 0x9cc
			 0x04178098, /// 0x9d0
			 0xa11f54d2, /// 0x9d4
			 0xe2403317, /// 0x9d8
			 0x8b257cce, /// 0x9dc
			 0x7cf1df0e, /// 0x9e0
			 0x9e8ad3cb, /// 0x9e4
			 0xf15c9bd8, /// 0x9e8
			 0x0adc3dc8, /// 0x9ec
			 0x4eeed807, /// 0x9f0
			 0x27cea77b, /// 0x9f4
			 0x10a8dea4, /// 0x9f8
			 0xbb677c05, /// 0x9fc
			 0x11da7f40, /// 0xa00
			 0x0033d678, /// 0xa04
			 0x0bbe4f84, /// 0xa08
			 0x93acd767, /// 0xa0c
			 0x9e831337, /// 0xa10
			 0xb1980ed0, /// 0xa14
			 0xbeb624c7, /// 0xa18
			 0x81f5396d, /// 0xa1c
			 0x7b2decb8, /// 0xa20
			 0xf1b8bc8d, /// 0xa24
			 0xcabc02d1, /// 0xa28
			 0xdc00d17b, /// 0xa2c
			 0xfc6fe79f, /// 0xa30
			 0x3b7c3bd3, /// 0xa34
			 0x8a52b410, /// 0xa38
			 0xc813de34, /// 0xa3c
			 0x9ccca093, /// 0xa40
			 0xc7400a66, /// 0xa44
			 0xd157ed9f, /// 0xa48
			 0xa1b6e00f, /// 0xa4c
			 0xa2d9ee8f, /// 0xa50
			 0x8daaa714, /// 0xa54
			 0xb334391e, /// 0xa58
			 0xe777d504, /// 0xa5c
			 0xf32fc3b1, /// 0xa60
			 0x9d3d64d6, /// 0xa64
			 0x96ab4826, /// 0xa68
			 0x78add62d, /// 0xa6c
			 0x1d9a535e, /// 0xa70
			 0xee02efc0, /// 0xa74
			 0xdf98e403, /// 0xa78
			 0xfe660e36, /// 0xa7c
			 0xaa3bfe98, /// 0xa80
			 0x77b281df, /// 0xa84
			 0x1a15b679, /// 0xa88
			 0x1d6d6a82, /// 0xa8c
			 0x58067399, /// 0xa90
			 0xedad42d1, /// 0xa94
			 0x12aeedd8, /// 0xa98
			 0xe0f8de40, /// 0xa9c
			 0x66972a50, /// 0xaa0
			 0xfe2cdd3f, /// 0xaa4
			 0xec03d28f, /// 0xaa8
			 0x90f4ec5a, /// 0xaac
			 0xe541350f, /// 0xab0
			 0x103149d6, /// 0xab4
			 0x677a9308, /// 0xab8
			 0xca71a01d, /// 0xabc
			 0x2dac18b6, /// 0xac0
			 0x85446d4a, /// 0xac4
			 0x4b92d9bc, /// 0xac8
			 0xd642dc20, /// 0xacc
			 0xaf5a36df, /// 0xad0
			 0x2d251497, /// 0xad4
			 0x4da08948, /// 0xad8
			 0x5cc184d8, /// 0xadc
			 0xd2554ae2, /// 0xae0
			 0x9d5bfc97, /// 0xae4
			 0xe241c632, /// 0xae8
			 0xf41afc45, /// 0xaec
			 0xd2de40a8, /// 0xaf0
			 0xcae16b15, /// 0xaf4
			 0x66cd6012, /// 0xaf8
			 0x4477f4a4, /// 0xafc
			 0xaf819b50, /// 0xb00
			 0x74b88f1b, /// 0xb04
			 0x3651e2ae, /// 0xb08
			 0xcd7c78f2, /// 0xb0c
			 0x29eaca8c, /// 0xb10
			 0xed666cad, /// 0xb14
			 0x8ffb5c37, /// 0xb18
			 0x62022088, /// 0xb1c
			 0x1c15ebcc, /// 0xb20
			 0x9d91fa29, /// 0xb24
			 0x5d2cc986, /// 0xb28
			 0xf30bb00d, /// 0xb2c
			 0xeebe183f, /// 0xb30
			 0xb2f1fa3a, /// 0xb34
			 0x662de13e, /// 0xb38
			 0xfe8701b2, /// 0xb3c
			 0x4fafc76f, /// 0xb40
			 0x9bfcb9ad, /// 0xb44
			 0x391afb2d, /// 0xb48
			 0x7a70a514, /// 0xb4c
			 0xf25405ee, /// 0xb50
			 0x8b62cb8e, /// 0xb54
			 0x68130392, /// 0xb58
			 0x377e5689, /// 0xb5c
			 0x3e650361, /// 0xb60
			 0xcdf39952, /// 0xb64
			 0x84780a23, /// 0xb68
			 0xa4214c7a, /// 0xb6c
			 0xa4614201, /// 0xb70
			 0x598bf4a6, /// 0xb74
			 0x4c72ca8d, /// 0xb78
			 0x474d6f6a, /// 0xb7c
			 0x16ec6750, /// 0xb80
			 0x727f3b17, /// 0xb84
			 0x981f3a80, /// 0xb88
			 0xe711af4e, /// 0xb8c
			 0x0976267f, /// 0xb90
			 0x743672a8, /// 0xb94
			 0x6162be66, /// 0xb98
			 0x2aaffdcb, /// 0xb9c
			 0x17561bc4, /// 0xba0
			 0xba9e372c, /// 0xba4
			 0xeba6aae5, /// 0xba8
			 0x9c4cb774, /// 0xbac
			 0xf9504d9c, /// 0xbb0
			 0x0efabd7d, /// 0xbb4
			 0x45e73aba, /// 0xbb8
			 0xcbd87e1f, /// 0xbbc
			 0x9e9e4770, /// 0xbc0
			 0x8b2e7e52, /// 0xbc4
			 0x4a3f870f, /// 0xbc8
			 0x42cf6ae2, /// 0xbcc
			 0x3bf95dfb, /// 0xbd0
			 0xec9d7237, /// 0xbd4
			 0xd96b348d, /// 0xbd8
			 0xd9d86155, /// 0xbdc
			 0x60165e31, /// 0xbe0
			 0x6e1e45a5, /// 0xbe4
			 0x671dc6a7, /// 0xbe8
			 0x1a8fd9ab, /// 0xbec
			 0x000bbbe7, /// 0xbf0
			 0xebc0a9dc, /// 0xbf4
			 0xc3f68b7f, /// 0xbf8
			 0x472c2e52, /// 0xbfc
			 0x0dcc427d, /// 0xc00
			 0xd9149f31, /// 0xc04
			 0x342df458, /// 0xc08
			 0xe1815e5d, /// 0xc0c
			 0x8854e2b7, /// 0xc10
			 0x1019f334, /// 0xc14
			 0x77c59e80, /// 0xc18
			 0xd242b295, /// 0xc1c
			 0x622d2509, /// 0xc20
			 0xd32ef03e, /// 0xc24
			 0x511a7c40, /// 0xc28
			 0x84aa26ac, /// 0xc2c
			 0x23748d67, /// 0xc30
			 0xbcecb10c, /// 0xc34
			 0x2f420ade, /// 0xc38
			 0x455cfd9b, /// 0xc3c
			 0xf482409d, /// 0xc40
			 0xd30d22bc, /// 0xc44
			 0xd42f9c66, /// 0xc48
			 0xa75ac2a7, /// 0xc4c
			 0x9efdfb52, /// 0xc50
			 0x631cc51d, /// 0xc54
			 0x14a8df92, /// 0xc58
			 0x157589dc, /// 0xc5c
			 0xd570d3bc, /// 0xc60
			 0xcfc97578, /// 0xc64
			 0x3e30fc7c, /// 0xc68
			 0x543a8677, /// 0xc6c
			 0x59554dd7, /// 0xc70
			 0xc296636d, /// 0xc74
			 0x33fb5701, /// 0xc78
			 0xcd10d6f4, /// 0xc7c
			 0x1f21cc3d, /// 0xc80
			 0x6bcb0547, /// 0xc84
			 0x8aefd4f1, /// 0xc88
			 0x58663ed0, /// 0xc8c
			 0x4f490ec4, /// 0xc90
			 0x2cbf1997, /// 0xc94
			 0x7b0c1402, /// 0xc98
			 0x6586171d, /// 0xc9c
			 0x647b2d76, /// 0xca0
			 0xb8bc4752, /// 0xca4
			 0x646f7ab1, /// 0xca8
			 0x5e02f3a0, /// 0xcac
			 0x406d83da, /// 0xcb0
			 0x8123a265, /// 0xcb4
			 0x23d5455c, /// 0xcb8
			 0xaabbc1de, /// 0xcbc
			 0x158e5be5, /// 0xcc0
			 0xaaf1374a, /// 0xcc4
			 0x7eb45098, /// 0xcc8
			 0xe65bde95, /// 0xccc
			 0xc67e8b8f, /// 0xcd0
			 0x0e3334f7, /// 0xcd4
			 0x9da77e91, /// 0xcd8
			 0x930657f5, /// 0xcdc
			 0xf21e349b, /// 0xce0
			 0x2d152d60, /// 0xce4
			 0x8d539bbe, /// 0xce8
			 0xd541fd92, /// 0xcec
			 0xd604322f, /// 0xcf0
			 0x4063f2fe, /// 0xcf4
			 0xa5e00dfd, /// 0xcf8
			 0x077892d5, /// 0xcfc
			 0xf14ae119, /// 0xd00
			 0x9b901e86, /// 0xd04
			 0x5781363a, /// 0xd08
			 0x7ff9f441, /// 0xd0c
			 0x1406f49a, /// 0xd10
			 0x1db9633e, /// 0xd14
			 0x9274dd77, /// 0xd18
			 0xbf61536a, /// 0xd1c
			 0x3f717c97, /// 0xd20
			 0x60d53367, /// 0xd24
			 0x4100ba81, /// 0xd28
			 0x5b33d462, /// 0xd2c
			 0x3c8996e9, /// 0xd30
			 0x50cd8313, /// 0xd34
			 0x8a1ef42c, /// 0xd38
			 0x88469977, /// 0xd3c
			 0xc8aaf066, /// 0xd40
			 0x94b7f63a, /// 0xd44
			 0xee2f1aad, /// 0xd48
			 0xb79290fd, /// 0xd4c
			 0x40cd47f3, /// 0xd50
			 0x0627bef9, /// 0xd54
			 0x6b1def6a, /// 0xd58
			 0x0676980b, /// 0xd5c
			 0x1851b2a7, /// 0xd60
			 0x7172759d, /// 0xd64
			 0xda4cde28, /// 0xd68
			 0x738c6e1f, /// 0xd6c
			 0x09c24e97, /// 0xd70
			 0x6cfd0e1a, /// 0xd74
			 0x80d9d196, /// 0xd78
			 0x6187bc7d, /// 0xd7c
			 0xc737adb1, /// 0xd80
			 0x81a57cde, /// 0xd84
			 0x85766654, /// 0xd88
			 0x4fb38519, /// 0xd8c
			 0x016840ef, /// 0xd90
			 0x1b697845, /// 0xd94
			 0x4b498d69, /// 0xd98
			 0xacbc9423, /// 0xd9c
			 0x22b77523, /// 0xda0
			 0x9ee1e885, /// 0xda4
			 0x79e4b944, /// 0xda8
			 0x5a3bf8e7, /// 0xdac
			 0x5b2ff3eb, /// 0xdb0
			 0xfdac51cb, /// 0xdb4
			 0xa4e80fe2, /// 0xdb8
			 0x6fc39a31, /// 0xdbc
			 0x20a1c620, /// 0xdc0
			 0x032f5e2a, /// 0xdc4
			 0x0d0840b6, /// 0xdc8
			 0x4fd1dab1, /// 0xdcc
			 0xdcbe23a6, /// 0xdd0
			 0xc577765b, /// 0xdd4
			 0xec64ca13, /// 0xdd8
			 0x0735a22d, /// 0xddc
			 0xb5d305f7, /// 0xde0
			 0x69f7af8c, /// 0xde4
			 0x5771873c, /// 0xde8
			 0x375e1e31, /// 0xdec
			 0x10e1e733, /// 0xdf0
			 0x5a1639b5, /// 0xdf4
			 0xee0a0586, /// 0xdf8
			 0x87b53cb2, /// 0xdfc
			 0x72e5d71d, /// 0xe00
			 0xe2986787, /// 0xe04
			 0xee1ffca3, /// 0xe08
			 0x8ad4babc, /// 0xe0c
			 0xc2f9cc8b, /// 0xe10
			 0x88c0b656, /// 0xe14
			 0xf74ae071, /// 0xe18
			 0x3f5ef592, /// 0xe1c
			 0xed4f9f90, /// 0xe20
			 0x43bfb30c, /// 0xe24
			 0xecf056a1, /// 0xe28
			 0xd825a884, /// 0xe2c
			 0x75128641, /// 0xe30
			 0x91312483, /// 0xe34
			 0x874365c7, /// 0xe38
			 0xbe8a6e0a, /// 0xe3c
			 0x5018dfcf, /// 0xe40
			 0x7b73a0de, /// 0xe44
			 0x05d8afc0, /// 0xe48
			 0xfe12a9b0, /// 0xe4c
			 0x7999b8c3, /// 0xe50
			 0x8172f0a9, /// 0xe54
			 0xf72345f8, /// 0xe58
			 0x6ca12041, /// 0xe5c
			 0xce73c8ca, /// 0xe60
			 0x91113ee3, /// 0xe64
			 0x9e605cae, /// 0xe68
			 0x5c47214c, /// 0xe6c
			 0xe13d37bc, /// 0xe70
			 0xcc5e5e3a, /// 0xe74
			 0x0da55386, /// 0xe78
			 0x9c10369f, /// 0xe7c
			 0x4cb1e71c, /// 0xe80
			 0x13e5f07d, /// 0xe84
			 0x004e6681, /// 0xe88
			 0xddf103be, /// 0xe8c
			 0xd7adcfd3, /// 0xe90
			 0xa7a6e9ad, /// 0xe94
			 0x08670152, /// 0xe98
			 0x278c8f3d, /// 0xe9c
			 0xfc6b1014, /// 0xea0
			 0x4a709774, /// 0xea4
			 0x88dc051f, /// 0xea8
			 0xf1d2ab59, /// 0xeac
			 0x7900f0d7, /// 0xeb0
			 0xc37f34ee, /// 0xeb4
			 0x497bc1a6, /// 0xeb8
			 0xf31de38f, /// 0xebc
			 0x538a8250, /// 0xec0
			 0x8078968c, /// 0xec4
			 0x922c6c0f, /// 0xec8
			 0x0a40b22b, /// 0xecc
			 0x887d7baa, /// 0xed0
			 0xa5403159, /// 0xed4
			 0x53669766, /// 0xed8
			 0x92e0a62f, /// 0xedc
			 0x329c44e1, /// 0xee0
			 0x27e6927b, /// 0xee4
			 0xc686a61f, /// 0xee8
			 0x9f525fd6, /// 0xeec
			 0x7ab96f70, /// 0xef0
			 0xbe4b1cdd, /// 0xef4
			 0x046444c9, /// 0xef8
			 0x8e22be0b, /// 0xefc
			 0xb36e7b1c, /// 0xf00
			 0x817139e6, /// 0xf04
			 0x18ba8abc, /// 0xf08
			 0xc6690504, /// 0xf0c
			 0x530339a3, /// 0xf10
			 0x35ffc731, /// 0xf14
			 0x5b98f84f, /// 0xf18
			 0x05465e49, /// 0xf1c
			 0x8ad5ecd9, /// 0xf20
			 0x2839b1a3, /// 0xf24
			 0x47e5792b, /// 0xf28
			 0x71f548f8, /// 0xf2c
			 0x705b0afe, /// 0xf30
			 0xfc4d2ece, /// 0xf34
			 0xd20b8ca0, /// 0xf38
			 0x2217f7b8, /// 0xf3c
			 0x467edcc2, /// 0xf40
			 0xb9db6574, /// 0xf44
			 0x31a540cb, /// 0xf48
			 0xe78b3f00, /// 0xf4c
			 0xecc3eb9e, /// 0xf50
			 0x0ab03c2c, /// 0xf54
			 0xc2fa6f71, /// 0xf58
			 0xfe5eedc9, /// 0xf5c
			 0x06348ceb, /// 0xf60
			 0x4f5c09d0, /// 0xf64
			 0x8da252c5, /// 0xf68
			 0xf5ad9fbc, /// 0xf6c
			 0xf08d4fb4, /// 0xf70
			 0x84860d0a, /// 0xf74
			 0xed708c39, /// 0xf78
			 0x31a2d5e0, /// 0xf7c
			 0x82806587, /// 0xf80
			 0x9abd058d, /// 0xf84
			 0x3d9028e3, /// 0xf88
			 0x2d24fb0d, /// 0xf8c
			 0x4bc5197f, /// 0xf90
			 0xa4299b4d, /// 0xf94
			 0x64413dc5, /// 0xf98
			 0xf1bf6456, /// 0xf9c
			 0x688379b4, /// 0xfa0
			 0x6ca1cc85, /// 0xfa4
			 0xad8d2746, /// 0xfa8
			 0x5ebf6827, /// 0xfac
			 0xf7f4ede5, /// 0xfb0
			 0xdc778f56, /// 0xfb4
			 0x7b03739c, /// 0xfb8
			 0x02320dbf, /// 0xfbc
			 0x350c40d4, /// 0xfc0
			 0xa629d1a4, /// 0xfc4
			 0xaf03b829, /// 0xfc8
			 0xa7daa524, /// 0xfcc
			 0x4fcafa50, /// 0xfd0
			 0xe2da017f, /// 0xfd4
			 0xcbad8fe4, /// 0xfd8
			 0x9c326e4a, /// 0xfdc
			 0x5dc8fcb0, /// 0xfe0
			 0xcb01656b, /// 0xfe4
			 0x5a878896, /// 0xfe8
			 0xb5337958, /// 0xfec
			 0xb14e8e4f, /// 0xff0
			 0x06b8d30a, /// 0xff4
			 0x9951324c, /// 0xff8
			 0xac1b74e2 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00000
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00004
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0000c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00014
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00018
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0001c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00020
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00028
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0002c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00030
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00034
			 0x80011111,                                                  // -9.7958E-41                                 /// 00038
			 0x80011111,                                                  // -9.7958E-41                                 /// 0003c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00040
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00044
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00048
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0004c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00050
			 0x80011111,                                                  // -9.7958E-41                                 /// 00054
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00058
			 0x33333333,                                                  // 4 random values                             /// 0005c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00060
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00064
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00068
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00070
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00074
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00078
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x00000000,                                                  // zero                                        /// 00080
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00084
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0008c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00090
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00098
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0009c
			 0xff800000,                                                  // -inf                                        /// 000a0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x0c700000,                                                  // Leading 1s:                                 /// 000a8
			 0x0e000007,                                                  // Trailing 1s:                                /// 000ac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 000b4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000b8
			 0xff800000,                                                  // -inf                                        /// 000bc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000c0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 000c4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 000c8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000cc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 000d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000d4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000d8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000dc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000e0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000e4
			 0x0e000003,                                                  // Trailing 1s:                                /// 000e8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000ec
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 000f4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000f8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 000fc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00100
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00104
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00108
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0010c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00110
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00114
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0011c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00120
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00124
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0012c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00130
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00134
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00138
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0013c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00140
			 0x33333333,                                                  // 4 random values                             /// 00144
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00148
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0014c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00150
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00154
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00158
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00160
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00164
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00168
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0016c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00174
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00178
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0017c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00180
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00184
			 0x55555555,                                                  // 4 random values                             /// 00188
			 0x80011111,                                                  // -9.7958E-41                                 /// 0018c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00190
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x0c700000,                                                  // Leading 1s:                                 /// 00198
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0019c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001a4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ac
			 0x33333333,                                                  // 4 random values                             /// 001b0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 001b8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001bc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 001c8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001d0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001dc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001e8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001f8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 001fc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00200
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00204
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00208
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00210
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00214
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00218
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0021c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00220
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00224
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00228
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0022c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00230
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00238
			 0xbf028f5c,                                                  // -0.51                                       /// 0023c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00240
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00244
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00248
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0024c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00250
			 0x4b000000,                                                  // 8388608.0                                   /// 00254
			 0x80011111,                                                  // -9.7958E-41                                 /// 00258
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00260
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00264
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0026c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00270
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00274
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00278
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0027c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00284
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00288
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0028c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00294
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00298
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x0e000003,                                                  // Trailing 1s:                                /// 002a0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002a8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x0c600000,                                                  // Leading 1s:                                 /// 002b0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002bc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002c0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 002c4
			 0x80000000,                                                  // -zero                                       /// 002c8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002cc
			 0x33333333,                                                  // 4 random values                             /// 002d0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 002d4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002d8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 002dc
			 0x4b000000,                                                  // 8388608.0                                   /// 002e0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 002e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002f0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 002f8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002fc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00300
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00304
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00308
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0xcb000000,                                                  // -8388608.0                                  /// 00310
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00314
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00318
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00320
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00324
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00328
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0032c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00330
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00334
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0033c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00340
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00348
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0034c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00350
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00358
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0035c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00364
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00368
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0036c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00374
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00378
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0037c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00380
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00384
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00388
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0038c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00390
			 0x0e000003,                                                  // Trailing 1s:                                /// 00394
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00398
			 0xff800000,                                                  // -inf                                        /// 0039c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x80000000,                                                  // -zero                                       /// 003a8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003ac
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003b4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003b8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003c4
			 0x0e000003,                                                  // Trailing 1s:                                /// 003c8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003d4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003d8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003dc
			 0x0c600000,                                                  // Leading 1s:                                 /// 003e0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 003e4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003e8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003ec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003f0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003f4
			 0xff800000,                                                  // -inf                                        /// 003f8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003fc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00400
			 0x0c400000,                                                  // Leading 1s:                                 /// 00404
			 0x4b000000,                                                  // 8388608.0                                   /// 00408
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0040c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00410
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x0c780000,                                                  // Leading 1s:                                 /// 00418
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0041c
			 0x3f028f5c,                                                  // 0.51                                        /// 00420
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00424
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00428
			 0x7f800000,                                                  // inf                                         /// 0042c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00430
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00434
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0043c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00440
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00444
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00448
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0044c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00450
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00458
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0045c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00460
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00464
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00468
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0046c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00470
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00474
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00478
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0047c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00480
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00484
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00488
			 0x0c400000,                                                  // Leading 1s:                                 /// 0048c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00490
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00494
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00498
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0xbf028f5c,                                                  // -0.51                                       /// 004a0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004a4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004a8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 004ac
			 0x00000000,                                                  // zero                                        /// 004b0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004b4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004b8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004bc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004c0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004c4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004cc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 004d4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004dc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004e4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004e8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004f0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004fc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00500
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00504
			 0x00011111,                                                  // 9.7958E-41                                  /// 00508
			 0x55555555,                                                  // 4 random values                             /// 0050c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00510
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00514
			 0x55555555,                                                  // 4 random values                             /// 00518
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0051c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00520
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00524
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00528
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0052c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00530
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00534
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0053c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00544
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00548
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0054c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00550
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00558
			 0x80000000,                                                  // -zero                                       /// 0055c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00560
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00568
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0056c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00570
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00574
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00578
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0057c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00580
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00584
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00588
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0058c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00590
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00594
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00598
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0059c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 005a4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005a8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x80000000,                                                  // -zero                                       /// 005b0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005b4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005b8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005c0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 005c4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005c8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005d0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005d4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 005d8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005dc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 005e0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005e4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 005e8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005f0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 005f8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00604
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00608
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00610
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00614
			 0x0c400000,                                                  // Leading 1s:                                 /// 00618
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0061c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00620
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0062c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00630
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00634
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00638
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0063c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00640
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00644
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00648
			 0x55555555,                                                  // 4 random values                             /// 0064c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00650
			 0x7f800000,                                                  // inf                                         /// 00654
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00658
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00660
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00664
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0066c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00670
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00678
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0067c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00680
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00684
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00688
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0068c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00690
			 0x0e000007,                                                  // Trailing 1s:                                /// 00694
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0069c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006b0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b8
			 0x3f028f5c,                                                  // 0.51                                        /// 006bc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006c0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 006c8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 006d0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 006d4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006d8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006dc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 006e4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 006e8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 006f0
			 0x00000000,                                                  // zero                                        /// 006f4
			 0x80011111,                                                  // -9.7958E-41                                 /// 006f8
			 0x0c400000,                                                  // Leading 1s:                                 /// 006fc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00700
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x0e000003,                                                  // Trailing 1s:                                /// 00708
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00714
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0071c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00720
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00724
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00728
			 0xff800000,                                                  // -inf                                        /// 0072c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00730
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00738
			 0x0c700000,                                                  // Leading 1s:                                 /// 0073c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00740
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00744
			 0x0e000003,                                                  // Trailing 1s:                                /// 00748
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00754
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00758
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0075c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00760
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00764
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00768
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0076c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00770
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00774
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00778
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0077c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0078c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00798
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 007a0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 007a4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007a8
			 0x0c700000,                                                  // Leading 1s:                                 /// 007ac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 007b0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 007b8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007bc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007c4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007c8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007cc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 007d0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007dc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 007e0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007e4
			 0x0c400000,                                                  // Leading 1s:                                 /// 007e8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 007ec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007f0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007f4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007f8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 007fc
			 0x00000000,                                                  // zero                                        /// 00800
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00804
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00808
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0080c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00810
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00814
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00828
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0082c
			 0x55555555,                                                  // 4 random values                             /// 00830
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00834
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00838
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0083c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00840
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00848
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0084c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00850
			 0xbf028f5c,                                                  // -0.51                                       /// 00854
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00858
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0085c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00860
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00864
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00868
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0086c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00870
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00874
			 0x0c700000,                                                  // Leading 1s:                                 /// 00878
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0087c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00880
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00884
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0088c
			 0x00000000,                                                  // zero                                        /// 00890
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00898
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0089c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008a0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008a8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008ac
			 0x00011111,                                                  // 9.7958E-41                                  /// 008b0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 008b4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008b8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008bc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x0c700000,                                                  // Leading 1s:                                 /// 008c4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008c8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008cc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 008d0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008d8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 008dc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008e8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 008ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008f4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008fc
			 0xff800000,                                                  // -inf                                        /// 00900
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00904
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00908
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00910
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00914
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00918
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00920
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00924
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00928
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0092c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00930
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00934
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00938
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00940
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00944
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00948
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0094c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00954
			 0x3f028f5c,                                                  // 0.51                                        /// 00958
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0095c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00960
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00964
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00968
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0096c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00970
			 0xcb000000,                                                  // -8388608.0                                  /// 00974
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00978
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0097c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00980
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00984
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00988
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0098c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00990
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0099c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009a0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009a8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009ac
			 0x0c780000,                                                  // Leading 1s:                                 /// 009b0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 009b8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009bc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009c4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c8
			 0x0e000001,                                                  // Trailing 1s:                                /// 009cc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 009d0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009d4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009dc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009e0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009e8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009ec
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009f0
			 0x80011111,                                                  // -9.7958E-41                                 /// 009f4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0xffc00001,                                                  // -signaling NaN                              /// 009fc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a00
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a04
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a08
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a0c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a10
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a14
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a1c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a20
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a30
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a34
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a38
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a3c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a44
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a48
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a4c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a54
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a5c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a60
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a64
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a68
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a6c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00a70
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a74
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a78
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a80
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a84
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a88
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a8c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a90
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a94
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a98
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a9c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00aa0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00aa8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00aac
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ab0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00ab4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ab8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ac0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ac8
			 0x3f028f5c,                                                  // 0.51                                        /// 00acc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ad0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ad4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ad8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00adc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ae4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x7fc00001,                                                  // signaling NaN                               /// 00af0
			 0x7fc00001,                                                  // signaling NaN                               /// 00af4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00af8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00afc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b00
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b04
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b08
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00b0c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00b10
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b14
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00b18
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x80000000,                                                  // -zero                                       /// 00b20
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b24
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b28
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b2c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b30
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b34
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b38
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b3c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b40
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b48
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b4c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b50
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b54
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b58
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b5c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b60
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b64
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b68
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b6c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b70
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b74
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b78
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b80
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b88
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b8c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b90
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b94
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b98
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b9c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ba0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ba4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ba8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bac
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bb4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bbc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bc0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bc8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bcc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bd0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bd4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bd8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00be0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00be4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00be8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00bfc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c00
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c04
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c08
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00c0c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c10
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c14
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c18
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c1c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c20
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c24
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c28
			 0x4b000000,                                                  // 8388608.0                                   /// 00c2c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c30
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c34
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c38
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c3c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x00000000,                                                  // zero                                        /// 00c44
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c48
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c4c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c50
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c54
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c58
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c5c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c68
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c6c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c70
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c78
			 0x7fc00001,                                                  // signaling NaN                               /// 00c7c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c80
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00c8c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00c90
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00c94
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c98
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c9c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ca0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ca4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00ca8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cac
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cb0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cb8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cbc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cc0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cc4
			 0x00000000,                                                  // zero                                        /// 00cc8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ccc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00cd4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cd8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ce0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ce4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cf0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x00000000,                                                  // zero                                        /// 00d00
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d04
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d08
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d14
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d1c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d20
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d24
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d28
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d2c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d34
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d38
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d3c
			 0x55555555,                                                  // 4 random values                             /// 00d40
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d48
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00d54
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d58
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d64
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00d68
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d6c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00d70
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d74
			 0x33333333,                                                  // 4 random values                             /// 00d78
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d7c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d80
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d84
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d94
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00d98
			 0xcb000000,                                                  // -8388608.0                                  /// 00d9c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00da0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00da4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00dac
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00db4
			 0x80000000,                                                  // -zero                                       /// 00db8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x7f800000,                                                  // inf                                         /// 00dc0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00dc4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00dc8
			 0xbf028f5c,                                                  // -0.51                                       /// 00dcc
			 0x80000000,                                                  // -zero                                       /// 00dd0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00dd4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00dd8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ddc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00de0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00de8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dec
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00df0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00df8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00dfc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00e00
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e04
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e0c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e10
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e20
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e24
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e28
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e2c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e30
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e34
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0xcb000000,                                                  // -8388608.0                                  /// 00e3c
			 0xff800000,                                                  // -inf                                        /// 00e40
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e48
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e4c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e50
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e5c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e60
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e64
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e68
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e6c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e70
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x33333333,                                                  // 4 random values                             /// 00e78
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e7c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e80
			 0x33333333,                                                  // 4 random values                             /// 00e84
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e88
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e8c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e90
			 0x4b000000,                                                  // 8388608.0                                   /// 00e94
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e98
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00e9c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ea0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ea4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ea8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00eac
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00eb0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00eb8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ec0
			 0xff800000,                                                  // -inf                                        /// 00ec4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ec8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ecc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ed0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ed4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ed8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ee8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00eec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ef0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ef4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ef8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00efc
			 0x4b000000,                                                  // 8388608.0                                   /// 00f00
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f08
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f0c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0x4b000000,                                                  // 8388608.0                                   /// 00f14
			 0xffc00001,                                                  // -signaling NaN                              /// 00f18
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00f1c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f20
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f24
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f28
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f2c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00f30
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00f34
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f38
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f3c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f40
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f44
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f48
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f4c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f50
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f54
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f58
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f5c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f60
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f64
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f6c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f70
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00f74
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f80
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f84
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f8c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f90
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f94
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f98
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f9c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fa0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00fa4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fa8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fb8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fbc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00fc4
			 0xcb000000,                                                  // -8388608.0                                  /// 00fc8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fcc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fd0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fd8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00fdc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fe0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fe4
			 0xcb000000,                                                  // -8388608.0                                  /// 00fe8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ff0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ff4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0x0e00003f                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x547135f4, /// 0x0
			 0x39e097be, /// 0x4
			 0x10e8090c, /// 0x8
			 0xf63120cd, /// 0xc
			 0x316c80ca, /// 0x10
			 0x8f7005ae, /// 0x14
			 0x1da6083d, /// 0x18
			 0x9e87216c, /// 0x1c
			 0x4b86aaa4, /// 0x20
			 0x84baaff6, /// 0x24
			 0xffec5e92, /// 0x28
			 0x499bf2d9, /// 0x2c
			 0x5828fc51, /// 0x30
			 0x3ad343db, /// 0x34
			 0x4097646f, /// 0x38
			 0xb5fbec1c, /// 0x3c
			 0xcc4b75c6, /// 0x40
			 0xaa6e941e, /// 0x44
			 0x52fc3db0, /// 0x48
			 0xa73bba41, /// 0x4c
			 0x84f3d5f4, /// 0x50
			 0xbe5d0835, /// 0x54
			 0xfad04ab6, /// 0x58
			 0x772f0948, /// 0x5c
			 0x7bf1b4d4, /// 0x60
			 0x1761cdef, /// 0x64
			 0x3251ef69, /// 0x68
			 0xbeef9431, /// 0x6c
			 0x30753b1f, /// 0x70
			 0x8dd87f04, /// 0x74
			 0x076b6d8f, /// 0x78
			 0x9d75cc8a, /// 0x7c
			 0x119e926c, /// 0x80
			 0x65d57876, /// 0x84
			 0x7b849b2b, /// 0x88
			 0xb01c9ca7, /// 0x8c
			 0x699c52ae, /// 0x90
			 0x25a9a020, /// 0x94
			 0x9dc27617, /// 0x98
			 0x29bb086c, /// 0x9c
			 0xd9690217, /// 0xa0
			 0x6ae04176, /// 0xa4
			 0x74b88913, /// 0xa8
			 0x08b58116, /// 0xac
			 0xd8c7e6ed, /// 0xb0
			 0x571b4531, /// 0xb4
			 0xb87acfdb, /// 0xb8
			 0x695c1714, /// 0xbc
			 0x9573b16d, /// 0xc0
			 0x99b64c8a, /// 0xc4
			 0xb47725c3, /// 0xc8
			 0x22d2e209, /// 0xcc
			 0x651a8567, /// 0xd0
			 0x944c2621, /// 0xd4
			 0xa41104cc, /// 0xd8
			 0xb8812d2d, /// 0xdc
			 0x425fd9d8, /// 0xe0
			 0x723991fe, /// 0xe4
			 0x6cd7ac2d, /// 0xe8
			 0x5e2fc966, /// 0xec
			 0x3e6acb78, /// 0xf0
			 0x69afdbb9, /// 0xf4
			 0x2e5ad4f4, /// 0xf8
			 0x46404022, /// 0xfc
			 0x345e7dd6, /// 0x100
			 0xe7a659a2, /// 0x104
			 0x32d46a6d, /// 0x108
			 0xaf375494, /// 0x10c
			 0x6c811be0, /// 0x110
			 0x84df1d66, /// 0x114
			 0xe0db199e, /// 0x118
			 0x7e4e26b8, /// 0x11c
			 0xdba9d0d8, /// 0x120
			 0x8dfcd26e, /// 0x124
			 0x6863510e, /// 0x128
			 0x167ce842, /// 0x12c
			 0x77e8da3c, /// 0x130
			 0xe57cdd10, /// 0x134
			 0x18acfefa, /// 0x138
			 0x58f3a49e, /// 0x13c
			 0xd36affcd, /// 0x140
			 0x1f76c56a, /// 0x144
			 0x84f77a55, /// 0x148
			 0xcb4748f3, /// 0x14c
			 0x88f5c4c6, /// 0x150
			 0x9534e5fb, /// 0x154
			 0x2aa614ca, /// 0x158
			 0xf9fd84e4, /// 0x15c
			 0xe48552d5, /// 0x160
			 0xb28345f5, /// 0x164
			 0x0f00fcc5, /// 0x168
			 0x0d1d5f43, /// 0x16c
			 0x2b147b76, /// 0x170
			 0xdb0615a2, /// 0x174
			 0x6e84c378, /// 0x178
			 0xb0587bd2, /// 0x17c
			 0xcd01aa7e, /// 0x180
			 0x3dd781d2, /// 0x184
			 0xb0705de0, /// 0x188
			 0x01439104, /// 0x18c
			 0x35f2a35e, /// 0x190
			 0xa817daea, /// 0x194
			 0x401687ba, /// 0x198
			 0x4e228d20, /// 0x19c
			 0x618d5413, /// 0x1a0
			 0x453435d9, /// 0x1a4
			 0xb268b652, /// 0x1a8
			 0xce9dca65, /// 0x1ac
			 0x7e15d963, /// 0x1b0
			 0x13f586e3, /// 0x1b4
			 0xb9dc2eeb, /// 0x1b8
			 0x580c7ad2, /// 0x1bc
			 0xb714ea4f, /// 0x1c0
			 0x16f27aae, /// 0x1c4
			 0x0d3f467b, /// 0x1c8
			 0x0cd90065, /// 0x1cc
			 0x6430ddc2, /// 0x1d0
			 0x8e1ae48b, /// 0x1d4
			 0x015a51ac, /// 0x1d8
			 0x8d609ca2, /// 0x1dc
			 0x64810954, /// 0x1e0
			 0x91270077, /// 0x1e4
			 0x9c2e878b, /// 0x1e8
			 0x80957b24, /// 0x1ec
			 0x67a6cce0, /// 0x1f0
			 0xae8cfa2d, /// 0x1f4
			 0x787a99f8, /// 0x1f8
			 0x2ed4bd91, /// 0x1fc
			 0x3c377a58, /// 0x200
			 0x35897e5d, /// 0x204
			 0xe5fe7f89, /// 0x208
			 0x8d01058b, /// 0x20c
			 0xce3bd92f, /// 0x210
			 0x91477bfa, /// 0x214
			 0xad562bab, /// 0x218
			 0xa72dcff0, /// 0x21c
			 0x4e23c078, /// 0x220
			 0x8003bc2a, /// 0x224
			 0x4bab199c, /// 0x228
			 0x2e3b4762, /// 0x22c
			 0xa69411d3, /// 0x230
			 0x2d910edf, /// 0x234
			 0x6d298d6f, /// 0x238
			 0x92fbfee7, /// 0x23c
			 0x280852ab, /// 0x240
			 0x0c93ba99, /// 0x244
			 0x8eed0581, /// 0x248
			 0x2ff2887d, /// 0x24c
			 0x15b9f7ed, /// 0x250
			 0xc2d5f551, /// 0x254
			 0xd26534df, /// 0x258
			 0xe4dff6f7, /// 0x25c
			 0x4761bc3a, /// 0x260
			 0xeed8b849, /// 0x264
			 0x1c60ed38, /// 0x268
			 0xfaf68d40, /// 0x26c
			 0x54a945a8, /// 0x270
			 0x148e1705, /// 0x274
			 0x6342086b, /// 0x278
			 0x24041d35, /// 0x27c
			 0xe9ab2242, /// 0x280
			 0x3978e25e, /// 0x284
			 0x41e39a94, /// 0x288
			 0xf0a87d0e, /// 0x28c
			 0xfe046660, /// 0x290
			 0x8d715260, /// 0x294
			 0x02defb31, /// 0x298
			 0xb4b5f902, /// 0x29c
			 0x63e70800, /// 0x2a0
			 0xd0e35074, /// 0x2a4
			 0xc411ae7f, /// 0x2a8
			 0x6646ba86, /// 0x2ac
			 0x42ef0363, /// 0x2b0
			 0x54529d9b, /// 0x2b4
			 0x64a726ed, /// 0x2b8
			 0x6fdbf269, /// 0x2bc
			 0x521773d0, /// 0x2c0
			 0xf30ff805, /// 0x2c4
			 0x2ea3ae6a, /// 0x2c8
			 0x299a87e0, /// 0x2cc
			 0x48258b2e, /// 0x2d0
			 0xb0ae9809, /// 0x2d4
			 0xc4533632, /// 0x2d8
			 0xe807ac43, /// 0x2dc
			 0x0c820d33, /// 0x2e0
			 0x66f999fa, /// 0x2e4
			 0x4508af31, /// 0x2e8
			 0x85742921, /// 0x2ec
			 0x0968be37, /// 0x2f0
			 0x1161eadc, /// 0x2f4
			 0x5295afa4, /// 0x2f8
			 0xe604204d, /// 0x2fc
			 0x307a839c, /// 0x300
			 0x6c2ec3b4, /// 0x304
			 0xac24a4ee, /// 0x308
			 0x5aecc5e1, /// 0x30c
			 0xc059dd76, /// 0x310
			 0xf3201e79, /// 0x314
			 0x93431764, /// 0x318
			 0x7de349d3, /// 0x31c
			 0x78012919, /// 0x320
			 0xd01d31bc, /// 0x324
			 0x8e93e121, /// 0x328
			 0xb4faecad, /// 0x32c
			 0x05b60be3, /// 0x330
			 0x27295a25, /// 0x334
			 0xe3b42608, /// 0x338
			 0x825a25ed, /// 0x33c
			 0xa4a3d5d3, /// 0x340
			 0xc3684a90, /// 0x344
			 0xaa3268ce, /// 0x348
			 0xd163d917, /// 0x34c
			 0x48e3f5ba, /// 0x350
			 0x2b5882ad, /// 0x354
			 0x18e597d7, /// 0x358
			 0x0fdda36f, /// 0x35c
			 0x8369917c, /// 0x360
			 0x6b058f65, /// 0x364
			 0x1755c070, /// 0x368
			 0x24e6de9d, /// 0x36c
			 0x0341f302, /// 0x370
			 0x8237521c, /// 0x374
			 0x221d8671, /// 0x378
			 0x24ade5ff, /// 0x37c
			 0x0283c040, /// 0x380
			 0x9883ec3c, /// 0x384
			 0x0932d855, /// 0x388
			 0x4a1d9034, /// 0x38c
			 0x3356dfc7, /// 0x390
			 0x4ad319cc, /// 0x394
			 0xdc843819, /// 0x398
			 0x831d2513, /// 0x39c
			 0xfc8c3fdc, /// 0x3a0
			 0x7f5ddaad, /// 0x3a4
			 0xdd0e052e, /// 0x3a8
			 0x2a7bb2d4, /// 0x3ac
			 0x50338167, /// 0x3b0
			 0xd323d56a, /// 0x3b4
			 0x9dbcc773, /// 0x3b8
			 0x3475ba4e, /// 0x3bc
			 0x8edd2a2a, /// 0x3c0
			 0x7a39ef45, /// 0x3c4
			 0x33f7fa0e, /// 0x3c8
			 0x741ea8c8, /// 0x3cc
			 0x797cf2f0, /// 0x3d0
			 0x5073e1bf, /// 0x3d4
			 0x782a3fc2, /// 0x3d8
			 0x5fbe95f0, /// 0x3dc
			 0xb82a650a, /// 0x3e0
			 0xb3e36db1, /// 0x3e4
			 0xbc86c733, /// 0x3e8
			 0x3528c8e5, /// 0x3ec
			 0x1bc8720a, /// 0x3f0
			 0xb402ee0a, /// 0x3f4
			 0x7118fb1d, /// 0x3f8
			 0x1eae3979, /// 0x3fc
			 0x4f9640bd, /// 0x400
			 0x6af339f9, /// 0x404
			 0xb928f876, /// 0x408
			 0x77600f6d, /// 0x40c
			 0xbd5fae63, /// 0x410
			 0x9f24523a, /// 0x414
			 0x3de00078, /// 0x418
			 0xcd6f495d, /// 0x41c
			 0x9944bad8, /// 0x420
			 0x4afe5df0, /// 0x424
			 0x71c53280, /// 0x428
			 0x3e5fd05f, /// 0x42c
			 0x913162e6, /// 0x430
			 0x681754dc, /// 0x434
			 0x06e714f3, /// 0x438
			 0xb133d3ab, /// 0x43c
			 0x274f2553, /// 0x440
			 0x8412a489, /// 0x444
			 0xe47642c0, /// 0x448
			 0xb032d12f, /// 0x44c
			 0xfa341881, /// 0x450
			 0xe7e0c68c, /// 0x454
			 0x649d34c5, /// 0x458
			 0xf2991bf5, /// 0x45c
			 0x7d19c99c, /// 0x460
			 0x2cc305d6, /// 0x464
			 0x82aa9b16, /// 0x468
			 0x2686270d, /// 0x46c
			 0x496573e0, /// 0x470
			 0xfa7989c8, /// 0x474
			 0x065c6f60, /// 0x478
			 0x0f672705, /// 0x47c
			 0x06ec5f18, /// 0x480
			 0xb8db1c8c, /// 0x484
			 0x9a3ce0a3, /// 0x488
			 0x1b126d59, /// 0x48c
			 0x765f9277, /// 0x490
			 0x7975d2d7, /// 0x494
			 0x53c51340, /// 0x498
			 0xcb4f5ae1, /// 0x49c
			 0x2eabdc7f, /// 0x4a0
			 0xf0349f83, /// 0x4a4
			 0x1d992d82, /// 0x4a8
			 0x7f97efc3, /// 0x4ac
			 0x3b13704e, /// 0x4b0
			 0xbdaa2130, /// 0x4b4
			 0x972af16c, /// 0x4b8
			 0xe24d10f4, /// 0x4bc
			 0x12ec031b, /// 0x4c0
			 0x6ebd17cd, /// 0x4c4
			 0xb8bf4456, /// 0x4c8
			 0xb08a0396, /// 0x4cc
			 0x1ea050e3, /// 0x4d0
			 0xf3e0c320, /// 0x4d4
			 0xa26e88ba, /// 0x4d8
			 0xa9ee0936, /// 0x4dc
			 0x8ae8abfb, /// 0x4e0
			 0x4b4203a5, /// 0x4e4
			 0x1059b211, /// 0x4e8
			 0x2eff6f1d, /// 0x4ec
			 0x470f6b39, /// 0x4f0
			 0xd7b28214, /// 0x4f4
			 0xfd137d7e, /// 0x4f8
			 0x51560081, /// 0x4fc
			 0xba7b467f, /// 0x500
			 0xfc9b6fee, /// 0x504
			 0x210acaad, /// 0x508
			 0x8390412d, /// 0x50c
			 0x3352c026, /// 0x510
			 0xf848b425, /// 0x514
			 0xcce03088, /// 0x518
			 0xfd4a91ae, /// 0x51c
			 0x7cb64622, /// 0x520
			 0x5242a19a, /// 0x524
			 0xb8fe04bd, /// 0x528
			 0x8357befb, /// 0x52c
			 0x60e98718, /// 0x530
			 0xbf8a906f, /// 0x534
			 0x4675e18f, /// 0x538
			 0x718fe7e6, /// 0x53c
			 0x648ed6b6, /// 0x540
			 0x114792e0, /// 0x544
			 0x48ba1097, /// 0x548
			 0xafafda19, /// 0x54c
			 0xd3a04875, /// 0x550
			 0x3a05e2e8, /// 0x554
			 0x9f1727a0, /// 0x558
			 0x912aeb3c, /// 0x55c
			 0x381177f0, /// 0x560
			 0x00c80d95, /// 0x564
			 0xe1300ab7, /// 0x568
			 0x3e907ad0, /// 0x56c
			 0x93ac30a0, /// 0x570
			 0x16eadeeb, /// 0x574
			 0x6441be79, /// 0x578
			 0xc506c057, /// 0x57c
			 0x2108f29c, /// 0x580
			 0x37aa796d, /// 0x584
			 0x81688bdb, /// 0x588
			 0x91c90959, /// 0x58c
			 0xc07155e1, /// 0x590
			 0x792a2646, /// 0x594
			 0xce642a13, /// 0x598
			 0x3030a4df, /// 0x59c
			 0xba8255c0, /// 0x5a0
			 0xd88f9e16, /// 0x5a4
			 0xa6ea9a7f, /// 0x5a8
			 0x8eb83c3e, /// 0x5ac
			 0x752817e6, /// 0x5b0
			 0x18758b66, /// 0x5b4
			 0xa09b6c20, /// 0x5b8
			 0x6b98e166, /// 0x5bc
			 0xd9efe751, /// 0x5c0
			 0x241df298, /// 0x5c4
			 0x35619f6c, /// 0x5c8
			 0xfebdb623, /// 0x5cc
			 0x1e2e4cda, /// 0x5d0
			 0xb8045fef, /// 0x5d4
			 0x9605492d, /// 0x5d8
			 0x65bd7191, /// 0x5dc
			 0x198c0706, /// 0x5e0
			 0xb2d70727, /// 0x5e4
			 0x7288a202, /// 0x5e8
			 0x59ba1443, /// 0x5ec
			 0xb4e1efaf, /// 0x5f0
			 0xe9d3d88f, /// 0x5f4
			 0x07ac85b3, /// 0x5f8
			 0xea6b65ab, /// 0x5fc
			 0x51a11072, /// 0x600
			 0xec6d47db, /// 0x604
			 0xfc82578e, /// 0x608
			 0xc94c014a, /// 0x60c
			 0x111fa43f, /// 0x610
			 0x833df561, /// 0x614
			 0x202838d2, /// 0x618
			 0x1859dbdb, /// 0x61c
			 0x62cb350e, /// 0x620
			 0x115e1edd, /// 0x624
			 0x5c3a82c8, /// 0x628
			 0x43151496, /// 0x62c
			 0x9c86466f, /// 0x630
			 0x1893b35e, /// 0x634
			 0x5ffeb94e, /// 0x638
			 0xfeb2051b, /// 0x63c
			 0x35097fc7, /// 0x640
			 0x034443ee, /// 0x644
			 0xb6a857d2, /// 0x648
			 0x227b2144, /// 0x64c
			 0x018dfe9a, /// 0x650
			 0x124e86ca, /// 0x654
			 0x22b882dc, /// 0x658
			 0x1df8de6d, /// 0x65c
			 0xd9f8dfde, /// 0x660
			 0x64f539d6, /// 0x664
			 0x512f0256, /// 0x668
			 0x7d5ed0df, /// 0x66c
			 0x0b626bc2, /// 0x670
			 0x95d4f74b, /// 0x674
			 0x0e7d84e2, /// 0x678
			 0xe38468a8, /// 0x67c
			 0x2124aceb, /// 0x680
			 0x08d38169, /// 0x684
			 0x9ca37c5d, /// 0x688
			 0xb1984fed, /// 0x68c
			 0x71c31727, /// 0x690
			 0xfc728f11, /// 0x694
			 0xb4be4041, /// 0x698
			 0x4ce4fbdf, /// 0x69c
			 0x3b5a1209, /// 0x6a0
			 0x984fe655, /// 0x6a4
			 0x4d73d9e5, /// 0x6a8
			 0x1ff1d1dc, /// 0x6ac
			 0x795cfa31, /// 0x6b0
			 0x8c1ee966, /// 0x6b4
			 0x83d5db48, /// 0x6b8
			 0x18eebc14, /// 0x6bc
			 0x890e917f, /// 0x6c0
			 0x9d26471d, /// 0x6c4
			 0x49eb8616, /// 0x6c8
			 0x394cc3a8, /// 0x6cc
			 0x972728cf, /// 0x6d0
			 0x278282c1, /// 0x6d4
			 0xc2c2fba2, /// 0x6d8
			 0x91d59b4b, /// 0x6dc
			 0x7d4e6bbe, /// 0x6e0
			 0x4c664a24, /// 0x6e4
			 0x4bd2b606, /// 0x6e8
			 0x95d4ef48, /// 0x6ec
			 0xc87a26bf, /// 0x6f0
			 0xc1d5d1ea, /// 0x6f4
			 0xa3159272, /// 0x6f8
			 0xc16e78ce, /// 0x6fc
			 0x7e4c3c8a, /// 0x700
			 0x6dbc8a45, /// 0x704
			 0x1abf20ac, /// 0x708
			 0xeaf07d0c, /// 0x70c
			 0x9a10f632, /// 0x710
			 0x42f220a8, /// 0x714
			 0x251cd606, /// 0x718
			 0x86d9fd63, /// 0x71c
			 0xb957ff55, /// 0x720
			 0x4252bb25, /// 0x724
			 0x7351b7c1, /// 0x728
			 0x2a0172cd, /// 0x72c
			 0x370771a5, /// 0x730
			 0xc3d7faef, /// 0x734
			 0x1833346e, /// 0x738
			 0xf4cbb1e7, /// 0x73c
			 0x92f2e312, /// 0x740
			 0xc4a7e717, /// 0x744
			 0xd6abd833, /// 0x748
			 0xa9e5e3c6, /// 0x74c
			 0x7fd1fb6e, /// 0x750
			 0x5a8bf1df, /// 0x754
			 0x9d9bf4fc, /// 0x758
			 0xb6bb2a5a, /// 0x75c
			 0x778d3b7f, /// 0x760
			 0xa8724a82, /// 0x764
			 0xdf63449b, /// 0x768
			 0xb4fa0c3a, /// 0x76c
			 0xae00ccb7, /// 0x770
			 0x8691a5b4, /// 0x774
			 0xf5af0194, /// 0x778
			 0x46f070f4, /// 0x77c
			 0xf331103e, /// 0x780
			 0x2ae10695, /// 0x784
			 0x4c727764, /// 0x788
			 0x9350bc53, /// 0x78c
			 0x90d90cd9, /// 0x790
			 0xe444976a, /// 0x794
			 0x905a0b8c, /// 0x798
			 0x37ef3b2c, /// 0x79c
			 0x1fb87c05, /// 0x7a0
			 0x7ae9426b, /// 0x7a4
			 0x5075c6e6, /// 0x7a8
			 0x76919043, /// 0x7ac
			 0x93dec849, /// 0x7b0
			 0xd58036e2, /// 0x7b4
			 0x76990bd3, /// 0x7b8
			 0x14fcc66f, /// 0x7bc
			 0x2be84023, /// 0x7c0
			 0x14538f08, /// 0x7c4
			 0xe55ff08b, /// 0x7c8
			 0x2ec559cf, /// 0x7cc
			 0x65a5a466, /// 0x7d0
			 0xc199d4fe, /// 0x7d4
			 0xd8c94e43, /// 0x7d8
			 0x6bc92f0b, /// 0x7dc
			 0xbee79da4, /// 0x7e0
			 0xb0222def, /// 0x7e4
			 0x0d0bde94, /// 0x7e8
			 0x702aaa8f, /// 0x7ec
			 0x9dcce03b, /// 0x7f0
			 0x00b6a240, /// 0x7f4
			 0x7afdb030, /// 0x7f8
			 0x1380f08c, /// 0x7fc
			 0x45835b0f, /// 0x800
			 0xa63ae42b, /// 0x804
			 0xad9422b5, /// 0x808
			 0x10c45ad3, /// 0x80c
			 0x763b1bd7, /// 0x810
			 0x6c53064f, /// 0x814
			 0x8ae0b4dd, /// 0x818
			 0x13c663e6, /// 0x81c
			 0xda3d7226, /// 0x820
			 0xaa748ba5, /// 0x824
			 0x66b248fc, /// 0x828
			 0x9f5f06e1, /// 0x82c
			 0x3d7b5e06, /// 0x830
			 0x1686d50b, /// 0x834
			 0xb0d3dad4, /// 0x838
			 0x38ca1f3a, /// 0x83c
			 0xcc10fe92, /// 0x840
			 0x592a642e, /// 0x844
			 0x47a22281, /// 0x848
			 0x0392d463, /// 0x84c
			 0x3d085368, /// 0x850
			 0x5aaecaa6, /// 0x854
			 0x5faab809, /// 0x858
			 0x442685de, /// 0x85c
			 0x88381210, /// 0x860
			 0x9780cd99, /// 0x864
			 0x2934fef6, /// 0x868
			 0xf637a4fc, /// 0x86c
			 0x0f4b42f8, /// 0x870
			 0xa9af8f7b, /// 0x874
			 0xc1a831d5, /// 0x878
			 0x45aabb8e, /// 0x87c
			 0x8aa074c3, /// 0x880
			 0xb448f3e8, /// 0x884
			 0xe2b80624, /// 0x888
			 0x5418545f, /// 0x88c
			 0x19ad998c, /// 0x890
			 0x4ce2275b, /// 0x894
			 0x6e6e060c, /// 0x898
			 0x6483f684, /// 0x89c
			 0xd44bebfb, /// 0x8a0
			 0x3e903508, /// 0x8a4
			 0xd15d0ed3, /// 0x8a8
			 0xbe37a67f, /// 0x8ac
			 0x532805a0, /// 0x8b0
			 0xc396128d, /// 0x8b4
			 0x0161a535, /// 0x8b8
			 0xb05730d7, /// 0x8bc
			 0xc3344241, /// 0x8c0
			 0xba42ea47, /// 0x8c4
			 0x20fa871a, /// 0x8c8
			 0xf25ad5f1, /// 0x8cc
			 0x89b074ac, /// 0x8d0
			 0x1c458d98, /// 0x8d4
			 0x8d1878cc, /// 0x8d8
			 0x0c802e2d, /// 0x8dc
			 0xf7bc60c3, /// 0x8e0
			 0x3cc55871, /// 0x8e4
			 0x31d72486, /// 0x8e8
			 0x88d1e714, /// 0x8ec
			 0xb95d8067, /// 0x8f0
			 0xcb6fff13, /// 0x8f4
			 0x99f52719, /// 0x8f8
			 0x51b5db6c, /// 0x8fc
			 0x905c56f2, /// 0x900
			 0x897e19cb, /// 0x904
			 0x26ab13b5, /// 0x908
			 0x160a6da6, /// 0x90c
			 0xbd0727ab, /// 0x910
			 0x4a97e594, /// 0x914
			 0x1b285f3e, /// 0x918
			 0xda937ab1, /// 0x91c
			 0xe9674eec, /// 0x920
			 0xc9a2fe0d, /// 0x924
			 0x3a9adf56, /// 0x928
			 0xe5f828b7, /// 0x92c
			 0x0c85d3f8, /// 0x930
			 0x35de8a4a, /// 0x934
			 0x016efa92, /// 0x938
			 0x682f2e53, /// 0x93c
			 0xdae42903, /// 0x940
			 0x5fc66dc1, /// 0x944
			 0xaad5b7a4, /// 0x948
			 0x3264294f, /// 0x94c
			 0x0cd04e97, /// 0x950
			 0x9342febb, /// 0x954
			 0xc310f7c5, /// 0x958
			 0xae617df2, /// 0x95c
			 0x8bc47687, /// 0x960
			 0xd8979dc1, /// 0x964
			 0xf308e29d, /// 0x968
			 0x0737eb52, /// 0x96c
			 0x0e89c1bd, /// 0x970
			 0xf3bbdd6f, /// 0x974
			 0x6c97d32e, /// 0x978
			 0xb4b0c955, /// 0x97c
			 0xf7643fdd, /// 0x980
			 0x20cec281, /// 0x984
			 0xbef80574, /// 0x988
			 0x5951e7aa, /// 0x98c
			 0x746d88dd, /// 0x990
			 0x4ee5e813, /// 0x994
			 0x839e2f7c, /// 0x998
			 0xb388fc77, /// 0x99c
			 0x6f691a30, /// 0x9a0
			 0xb1da6fd5, /// 0x9a4
			 0x0ae6ec75, /// 0x9a8
			 0x2119f8fd, /// 0x9ac
			 0x3229cd5c, /// 0x9b0
			 0xc1d164a2, /// 0x9b4
			 0x5ece35b4, /// 0x9b8
			 0xd74cc860, /// 0x9bc
			 0x8eb97746, /// 0x9c0
			 0xbfa9795b, /// 0x9c4
			 0xb660d1b6, /// 0x9c8
			 0xe0a485ca, /// 0x9cc
			 0xe5653532, /// 0x9d0
			 0x1fcd31ee, /// 0x9d4
			 0x84509d36, /// 0x9d8
			 0x21b27469, /// 0x9dc
			 0x21ceac72, /// 0x9e0
			 0x32323be9, /// 0x9e4
			 0xf02f073b, /// 0x9e8
			 0xd8db3906, /// 0x9ec
			 0xb2674b40, /// 0x9f0
			 0xb62abb28, /// 0x9f4
			 0x10bda340, /// 0x9f8
			 0x334b21fa, /// 0x9fc
			 0xddfb59c3, /// 0xa00
			 0x80d89667, /// 0xa04
			 0x165d52e1, /// 0xa08
			 0x65c90b2b, /// 0xa0c
			 0xd6ac45a8, /// 0xa10
			 0x13685552, /// 0xa14
			 0x6c372492, /// 0xa18
			 0x0587a0c7, /// 0xa1c
			 0x47b51a52, /// 0xa20
			 0x71b91c2a, /// 0xa24
			 0xd9ea61c3, /// 0xa28
			 0xc911cd6f, /// 0xa2c
			 0x97c41adf, /// 0xa30
			 0x84656352, /// 0xa34
			 0x1273074d, /// 0xa38
			 0x231aef50, /// 0xa3c
			 0xab83052f, /// 0xa40
			 0xfcf14562, /// 0xa44
			 0x294129dd, /// 0xa48
			 0x7631287d, /// 0xa4c
			 0x0179123d, /// 0xa50
			 0xba2ccdfb, /// 0xa54
			 0x1f5ac8dc, /// 0xa58
			 0x60478f49, /// 0xa5c
			 0x0cc29339, /// 0xa60
			 0x53632d86, /// 0xa64
			 0x8b0944f2, /// 0xa68
			 0x3e5aab3f, /// 0xa6c
			 0x78ffe8d7, /// 0xa70
			 0x7323be07, /// 0xa74
			 0xd0e09326, /// 0xa78
			 0xca4e8d0d, /// 0xa7c
			 0x0fceb8c9, /// 0xa80
			 0xa10b31b5, /// 0xa84
			 0xc7dec7e2, /// 0xa88
			 0xca329bc4, /// 0xa8c
			 0x8e852145, /// 0xa90
			 0x691c0c08, /// 0xa94
			 0x389f59d6, /// 0xa98
			 0xf1509f55, /// 0xa9c
			 0x9da403a7, /// 0xaa0
			 0x525139ff, /// 0xaa4
			 0x56a98ba3, /// 0xaa8
			 0xbc5e3a3e, /// 0xaac
			 0x9b44b3a5, /// 0xab0
			 0xa2cd23d6, /// 0xab4
			 0x7f8a9789, /// 0xab8
			 0xbad42ed8, /// 0xabc
			 0x100419f0, /// 0xac0
			 0xf4e144b4, /// 0xac4
			 0xe2c7565a, /// 0xac8
			 0xff3b1e77, /// 0xacc
			 0x2692b135, /// 0xad0
			 0xdc818a78, /// 0xad4
			 0xa0813819, /// 0xad8
			 0xff6f5e92, /// 0xadc
			 0xe3489faa, /// 0xae0
			 0x04d57845, /// 0xae4
			 0x9f3dada3, /// 0xae8
			 0x253e6d69, /// 0xaec
			 0x021dbeb4, /// 0xaf0
			 0x2194269f, /// 0xaf4
			 0xe841c380, /// 0xaf8
			 0x7900fe01, /// 0xafc
			 0x5e6748d9, /// 0xb00
			 0x5f0b5064, /// 0xb04
			 0xcc584bef, /// 0xb08
			 0x443e684c, /// 0xb0c
			 0x261d6f63, /// 0xb10
			 0xbb236434, /// 0xb14
			 0xd31092cf, /// 0xb18
			 0x1f79bb61, /// 0xb1c
			 0xc4f5464c, /// 0xb20
			 0x666c5537, /// 0xb24
			 0x3b553c25, /// 0xb28
			 0xb6f84dfa, /// 0xb2c
			 0xaf49a676, /// 0xb30
			 0xb96c3cde, /// 0xb34
			 0xb5105c7a, /// 0xb38
			 0xc68bdc4b, /// 0xb3c
			 0xce8a605e, /// 0xb40
			 0x45b0e3a4, /// 0xb44
			 0x78b4b992, /// 0xb48
			 0x4002e8c0, /// 0xb4c
			 0x881bcbd9, /// 0xb50
			 0x2a097c0e, /// 0xb54
			 0x4b554b87, /// 0xb58
			 0x75393d18, /// 0xb5c
			 0xb3ce278b, /// 0xb60
			 0x3264f72c, /// 0xb64
			 0x09a2db68, /// 0xb68
			 0xfe11398a, /// 0xb6c
			 0x7afb6d6a, /// 0xb70
			 0x7114b596, /// 0xb74
			 0x8853b1c9, /// 0xb78
			 0xd8ef8ca0, /// 0xb7c
			 0x33e045bd, /// 0xb80
			 0x371ee977, /// 0xb84
			 0x498ff4a7, /// 0xb88
			 0x10555bb8, /// 0xb8c
			 0xeb2714fc, /// 0xb90
			 0x3a97dc2b, /// 0xb94
			 0xab7c9509, /// 0xb98
			 0x9c0cdfa4, /// 0xb9c
			 0x07d67b9f, /// 0xba0
			 0x7861ce4a, /// 0xba4
			 0x5efc8a37, /// 0xba8
			 0x78ca0e6d, /// 0xbac
			 0x4b1382eb, /// 0xbb0
			 0x060ae87c, /// 0xbb4
			 0x2fc653a8, /// 0xbb8
			 0x100152f4, /// 0xbbc
			 0x5c543484, /// 0xbc0
			 0x4c1c0667, /// 0xbc4
			 0x6a3c93db, /// 0xbc8
			 0x1a53a05d, /// 0xbcc
			 0x05d1dd97, /// 0xbd0
			 0x90359495, /// 0xbd4
			 0xef1ab757, /// 0xbd8
			 0xd85fb985, /// 0xbdc
			 0xc3146645, /// 0xbe0
			 0x37682c88, /// 0xbe4
			 0x97362f2b, /// 0xbe8
			 0x2e4cc0ea, /// 0xbec
			 0x80e4cab1, /// 0xbf0
			 0xd99e04bc, /// 0xbf4
			 0x06fc601e, /// 0xbf8
			 0xb32b5abf, /// 0xbfc
			 0x1441dd91, /// 0xc00
			 0xbe21a919, /// 0xc04
			 0x5411ce1b, /// 0xc08
			 0xb04092fd, /// 0xc0c
			 0x2858d346, /// 0xc10
			 0x30bfca5f, /// 0xc14
			 0xe870ef89, /// 0xc18
			 0xaf17fc0b, /// 0xc1c
			 0x20afc1d5, /// 0xc20
			 0x0d4fcdcd, /// 0xc24
			 0x75f69191, /// 0xc28
			 0x91488596, /// 0xc2c
			 0xc9bcab27, /// 0xc30
			 0x0265c80d, /// 0xc34
			 0xcfa78958, /// 0xc38
			 0xa8b7aa0d, /// 0xc3c
			 0x51823698, /// 0xc40
			 0x37996018, /// 0xc44
			 0xf9c3a727, /// 0xc48
			 0xa663b980, /// 0xc4c
			 0xc5c31339, /// 0xc50
			 0x2342d1a7, /// 0xc54
			 0x35ff7bde, /// 0xc58
			 0xfcac411b, /// 0xc5c
			 0x54d64590, /// 0xc60
			 0xc4fff6ce, /// 0xc64
			 0xf037b212, /// 0xc68
			 0x274e91d3, /// 0xc6c
			 0xbe932ea2, /// 0xc70
			 0x8ed7e3e3, /// 0xc74
			 0x2d8ee0e3, /// 0xc78
			 0x15526372, /// 0xc7c
			 0xbb70b2cc, /// 0xc80
			 0xf83c2f1f, /// 0xc84
			 0xfe02e5ab, /// 0xc88
			 0xa30a2979, /// 0xc8c
			 0x301ea940, /// 0xc90
			 0x3fd2b79e, /// 0xc94
			 0xd723e804, /// 0xc98
			 0x6a5b808d, /// 0xc9c
			 0xf052cf39, /// 0xca0
			 0xf8497601, /// 0xca4
			 0xa64b9c05, /// 0xca8
			 0x258e95c5, /// 0xcac
			 0xc354015c, /// 0xcb0
			 0xd89562b4, /// 0xcb4
			 0x7cc00a61, /// 0xcb8
			 0x1a6fdccb, /// 0xcbc
			 0xb087ef70, /// 0xcc0
			 0xa3b2346b, /// 0xcc4
			 0xbabd98a9, /// 0xcc8
			 0xba6cbd05, /// 0xccc
			 0x3901f9ba, /// 0xcd0
			 0xe87f028a, /// 0xcd4
			 0xf74ed42a, /// 0xcd8
			 0x2bd93b1f, /// 0xcdc
			 0xf27f047b, /// 0xce0
			 0x27092dda, /// 0xce4
			 0x77e13388, /// 0xce8
			 0x548c7cb4, /// 0xcec
			 0x9d9eb965, /// 0xcf0
			 0x8dacd8f4, /// 0xcf4
			 0x46cfb0a4, /// 0xcf8
			 0xdf8e504d, /// 0xcfc
			 0x285e738d, /// 0xd00
			 0x75bece51, /// 0xd04
			 0x0dbddd98, /// 0xd08
			 0x398a9eb4, /// 0xd0c
			 0x21f2ac5b, /// 0xd10
			 0x4f5b1fbf, /// 0xd14
			 0x4373d7c8, /// 0xd18
			 0xc531ebe0, /// 0xd1c
			 0xe4bf5974, /// 0xd20
			 0x069b4f40, /// 0xd24
			 0xc35e7a69, /// 0xd28
			 0xd98dc666, /// 0xd2c
			 0x743bd367, /// 0xd30
			 0x49a81eb0, /// 0xd34
			 0xcb98178a, /// 0xd38
			 0x8a50f270, /// 0xd3c
			 0x485f841a, /// 0xd40
			 0x7a5f05b5, /// 0xd44
			 0xd00dcfa3, /// 0xd48
			 0xb0f8a7de, /// 0xd4c
			 0x995644cb, /// 0xd50
			 0x5815c46b, /// 0xd54
			 0x7dffde15, /// 0xd58
			 0x20125209, /// 0xd5c
			 0x345a6580, /// 0xd60
			 0x6961fd25, /// 0xd64
			 0x0803c2ae, /// 0xd68
			 0x381b82fc, /// 0xd6c
			 0xaf18a437, /// 0xd70
			 0x856a94f7, /// 0xd74
			 0x0a36049b, /// 0xd78
			 0xe235fed5, /// 0xd7c
			 0x72f1807b, /// 0xd80
			 0x677af0b7, /// 0xd84
			 0xb3df3e6e, /// 0xd88
			 0xebdca84a, /// 0xd8c
			 0xf8a509b0, /// 0xd90
			 0x1cf2d1c9, /// 0xd94
			 0x96e8c520, /// 0xd98
			 0xc5f1ee11, /// 0xd9c
			 0xf93c3475, /// 0xda0
			 0x3cbcb6f4, /// 0xda4
			 0x291db554, /// 0xda8
			 0x08da08c5, /// 0xdac
			 0x0d4f0b9b, /// 0xdb0
			 0xca256704, /// 0xdb4
			 0x5de17d00, /// 0xdb8
			 0x88ba8f8d, /// 0xdbc
			 0x18d2c7fd, /// 0xdc0
			 0xfc478210, /// 0xdc4
			 0x19540b6d, /// 0xdc8
			 0x9135f40b, /// 0xdcc
			 0x9400b148, /// 0xdd0
			 0x1209879a, /// 0xdd4
			 0x19534b09, /// 0xdd8
			 0x2b11126e, /// 0xddc
			 0x9288252c, /// 0xde0
			 0x4ad689ed, /// 0xde4
			 0x8975fcb9, /// 0xde8
			 0x38a7724b, /// 0xdec
			 0xdd6bd35d, /// 0xdf0
			 0xf6a4c74b, /// 0xdf4
			 0x5f2f2564, /// 0xdf8
			 0x3969c5f2, /// 0xdfc
			 0xc39629ed, /// 0xe00
			 0xf076dfe7, /// 0xe04
			 0x17cd2657, /// 0xe08
			 0x8cda842f, /// 0xe0c
			 0xc573ec6c, /// 0xe10
			 0xcea6cad5, /// 0xe14
			 0x21eb83fa, /// 0xe18
			 0x6aefe8b9, /// 0xe1c
			 0x4c26eb3f, /// 0xe20
			 0x196429db, /// 0xe24
			 0x7645f43e, /// 0xe28
			 0x7ae902f0, /// 0xe2c
			 0x1ce54982, /// 0xe30
			 0x0a08064f, /// 0xe34
			 0xef1c0a40, /// 0xe38
			 0x4aa0411c, /// 0xe3c
			 0x2a728ac3, /// 0xe40
			 0xa0ad5010, /// 0xe44
			 0x727be4e2, /// 0xe48
			 0x7c767ffe, /// 0xe4c
			 0xaaf48f3e, /// 0xe50
			 0x5cb37cc0, /// 0xe54
			 0x0e7553e7, /// 0xe58
			 0xe235a08c, /// 0xe5c
			 0x143055be, /// 0xe60
			 0xcc3a14f2, /// 0xe64
			 0xfeca7789, /// 0xe68
			 0xd33655b5, /// 0xe6c
			 0x253408f3, /// 0xe70
			 0xf0a54cdc, /// 0xe74
			 0x8899ba51, /// 0xe78
			 0xe5dde91f, /// 0xe7c
			 0x3ee26778, /// 0xe80
			 0xa25dc95a, /// 0xe84
			 0x211a8a96, /// 0xe88
			 0x974fc650, /// 0xe8c
			 0x798e52ef, /// 0xe90
			 0xa0ae2e66, /// 0xe94
			 0x14584b9e, /// 0xe98
			 0xad13af3b, /// 0xe9c
			 0x00f839e5, /// 0xea0
			 0xe187efcd, /// 0xea4
			 0x17471ea3, /// 0xea8
			 0x9e4be649, /// 0xeac
			 0x73b8108a, /// 0xeb0
			 0x5d75ae1b, /// 0xeb4
			 0xd43e6f83, /// 0xeb8
			 0x6684bd4c, /// 0xebc
			 0xa191c4e6, /// 0xec0
			 0xb19cfb05, /// 0xec4
			 0xfd9709d1, /// 0xec8
			 0x0488a07a, /// 0xecc
			 0x611562d4, /// 0xed0
			 0x8d630df1, /// 0xed4
			 0x782a55e1, /// 0xed8
			 0x450cdc78, /// 0xedc
			 0x8c4a956e, /// 0xee0
			 0xb960de44, /// 0xee4
			 0x363438ff, /// 0xee8
			 0xedb5c7ad, /// 0xeec
			 0xe4801cee, /// 0xef0
			 0x5e47a6ea, /// 0xef4
			 0xb7ec6de9, /// 0xef8
			 0xe1e675da, /// 0xefc
			 0x7e0d093b, /// 0xf00
			 0x6795502e, /// 0xf04
			 0xe386a2ec, /// 0xf08
			 0xba173ef1, /// 0xf0c
			 0x0b529444, /// 0xf10
			 0xfe0007d3, /// 0xf14
			 0xfacd6d97, /// 0xf18
			 0xd36e2291, /// 0xf1c
			 0xf412ca4e, /// 0xf20
			 0x67b33e3c, /// 0xf24
			 0x91897eb1, /// 0xf28
			 0x31e384cb, /// 0xf2c
			 0xfbf1df87, /// 0xf30
			 0xd7e536bf, /// 0xf34
			 0xce8230f9, /// 0xf38
			 0xf28255d9, /// 0xf3c
			 0xe38bd53a, /// 0xf40
			 0xe654c989, /// 0xf44
			 0x7de03427, /// 0xf48
			 0x31d8857e, /// 0xf4c
			 0x5fe37e4b, /// 0xf50
			 0x04de6112, /// 0xf54
			 0x016d0d03, /// 0xf58
			 0x1ba5751c, /// 0xf5c
			 0x67227de3, /// 0xf60
			 0x38208b20, /// 0xf64
			 0xf3b1747d, /// 0xf68
			 0xff17e39c, /// 0xf6c
			 0x0f550517, /// 0xf70
			 0xd675b9a1, /// 0xf74
			 0x7a12492c, /// 0xf78
			 0xfdf0c8de, /// 0xf7c
			 0x4f07b89f, /// 0xf80
			 0xe14799cb, /// 0xf84
			 0x719c0b18, /// 0xf88
			 0x0376dbde, /// 0xf8c
			 0xe1f84352, /// 0xf90
			 0xcd3ad06a, /// 0xf94
			 0x89a1c812, /// 0xf98
			 0xa46dad40, /// 0xf9c
			 0x03685b72, /// 0xfa0
			 0x5de872d7, /// 0xfa4
			 0x3c36bd75, /// 0xfa8
			 0x5f5ef67c, /// 0xfac
			 0x9e6f6a6a, /// 0xfb0
			 0x9b390815, /// 0xfb4
			 0x0b9b71a1, /// 0xfb8
			 0x616c38bf, /// 0xfbc
			 0xb023fea8, /// 0xfc0
			 0x829b0e9e, /// 0xfc4
			 0x9373ffb4, /// 0xfc8
			 0xb5abb99f, /// 0xfcc
			 0x396534e4, /// 0xfd0
			 0x95b3e77c, /// 0xfd4
			 0x2dba4a68, /// 0xfd8
			 0x6feeead3, /// 0xfdc
			 0x11cbfc4e, /// 0xfe0
			 0xfcb52e78, /// 0xfe4
			 0x1e62743f, /// 0xfe8
			 0xab662e57, /// 0xfec
			 0x792e6846, /// 0xff0
			 0xfb57a703, /// 0xff4
			 0x6091190b, /// 0xff8
			 0x3f41bbda /// last
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
			 0x00000398,
			 0x000001dc,
			 0x00000350,
			 0x000002a0,
			 0x000007a4,
			 0x0000039c,
			 0x00000744,
			 0x0000035c,

			 /// vpu register f2
			 0x41880000,
			 0x41b00000,
			 0x41b00000,
			 0x41900000,
			 0x41b00000,
			 0x41e80000,
			 0x41980000,
			 0x41f80000,

			 /// vpu register f3
			 0x41e00000,
			 0x41e00000,
			 0x42000000,
			 0x41900000,
			 0x41880000,
			 0x41600000,
			 0x41000000,
			 0x41980000,

			 /// vpu register f4
			 0x41d00000,
			 0x41700000,
			 0x41200000,
			 0x41500000,
			 0x41980000,
			 0x40a00000,
			 0x41e00000,
			 0x41a00000,

			 /// vpu register f5
			 0x41d00000,
			 0x41f00000,
			 0x41980000,
			 0x41e00000,
			 0x41a00000,
			 0x41e00000,
			 0x41d00000,
			 0x40e00000,

			 /// vpu register f6
			 0x41c00000,
			 0x41600000,
			 0x41000000,
			 0x41b80000,
			 0x41980000,
			 0x41800000,
			 0x41900000,
			 0x41700000,

			 /// vpu register f7
			 0x40800000,
			 0x41f80000,
			 0x40800000,
			 0x3f800000,
			 0x41d80000,
			 0x40a00000,
			 0x41980000,
			 0x40000000,

			 /// vpu register f8
			 0x41a80000,
			 0x41800000,
			 0x41f80000,
			 0x41a00000,
			 0x41c80000,
			 0x41800000,
			 0x41500000,
			 0x41880000,

			 /// vpu register f9
			 0x41f00000,
			 0x41f80000,
			 0x41f80000,
			 0x41d00000,
			 0x41700000,
			 0x41e00000,
			 0x41400000,
			 0x41d00000,

			 /// vpu register f10
			 0x41700000,
			 0x41d00000,
			 0x41a80000,
			 0x41300000,
			 0x40800000,
			 0x40e00000,
			 0x41b00000,
			 0x41e80000,

			 /// vpu register f11
			 0x3f800000,
			 0x41100000,
			 0x41400000,
			 0x41980000,
			 0x3f800000,
			 0x41900000,
			 0x42000000,
			 0x41c80000,

			 /// vpu register f12
			 0x40e00000,
			 0x41500000,
			 0x40800000,
			 0x40c00000,
			 0x41600000,
			 0x41a00000,
			 0x3f800000,
			 0x40e00000,

			 /// vpu register f13
			 0x40800000,
			 0x41e00000,
			 0x40e00000,
			 0x41800000,
			 0x41800000,
			 0x3f800000,
			 0x41700000,
			 0x42000000,

			 /// vpu register f14
			 0x41800000,
			 0x41980000,
			 0x41600000,
			 0x40400000,
			 0x41b80000,
			 0x40000000,
			 0x41c00000,
			 0x41f80000,

			 /// vpu register f15
			 0x41f00000,
			 0x40400000,
			 0x41b80000,
			 0x41a80000,
			 0x41300000,
			 0x41c80000,
			 0x41880000,
			 0x41f00000,

			 /// vpu register f16
			 0x41500000,
			 0x41100000,
			 0x41600000,
			 0x41b00000,
			 0x40a00000,
			 0x41e80000,
			 0x41a80000,
			 0x41e80000,

			 /// vpu register f17
			 0x41600000,
			 0x40e00000,
			 0x41b80000,
			 0x40800000,
			 0x42000000,
			 0x41e00000,
			 0x41700000,
			 0x41500000,

			 /// vpu register f18
			 0x41000000,
			 0x41a00000,
			 0x40000000,
			 0x41c80000,
			 0x41a00000,
			 0x41800000,
			 0x41e00000,
			 0x41c00000,

			 /// vpu register f19
			 0x41d00000,
			 0x3f800000,
			 0x41900000,
			 0x41800000,
			 0x40a00000,
			 0x40400000,
			 0x41100000,
			 0x41a00000,

			 /// vpu register f20
			 0x41b80000,
			 0x41d00000,
			 0x41880000,
			 0x41e80000,
			 0x41500000,
			 0x3f800000,
			 0x41900000,
			 0x40a00000,

			 /// vpu register f21
			 0x41c80000,
			 0x41500000,
			 0x40000000,
			 0x41500000,
			 0x41600000,
			 0x40e00000,
			 0x41a80000,
			 0x40400000,

			 /// vpu register f22
			 0x41980000,
			 0x41a00000,
			 0x41300000,
			 0x41600000,
			 0x41700000,
			 0x41a00000,
			 0x41f80000,
			 0x41a80000,

			 /// vpu register f23
			 0x42000000,
			 0x41f80000,
			 0x40000000,
			 0x42000000,
			 0x40400000,
			 0x41a00000,
			 0x41600000,
			 0x41a00000,

			 /// vpu register f24
			 0x41300000,
			 0x41880000,
			 0x40e00000,
			 0x41600000,
			 0x41500000,
			 0x41400000,
			 0x41b80000,
			 0x40800000,

			 /// vpu register f25
			 0x41e00000,
			 0x41e00000,
			 0x41200000,
			 0x41600000,
			 0x41500000,
			 0x41d00000,
			 0x41b00000,
			 0x40800000,

			 /// vpu register f26
			 0x40e00000,
			 0x41100000,
			 0x41c80000,
			 0x41800000,
			 0x41600000,
			 0x41c00000,
			 0x41880000,
			 0x41200000,

			 /// vpu register f27
			 0x41d80000,
			 0x41e80000,
			 0x41f80000,
			 0x40000000,
			 0x3f800000,
			 0x41800000,
			 0x41a00000,
			 0x3f800000,

			 /// vpu register f28
			 0x40800000,
			 0x41f80000,
			 0x40a00000,
			 0x41700000,
			 0x40e00000,
			 0x41f80000,
			 0x41000000,
			 0x41000000,

			 /// vpu register f29
			 0x41e00000,
			 0x41880000,
			 0x40c00000,
			 0x41900000,
			 0x40800000,
			 0x41600000,
			 0x41d00000,
			 0x41d80000,

			 /// vpu register f30
			 0x41800000,
			 0x41300000,
			 0x41d00000,
			 0x41f00000,
			 0x41e00000,
			 0x41500000,
			 0x41e80000,
			 0x42000000,

			 /// vpu register f31
			 0x41800000,
			 0x41c80000,
			 0x41100000,
			 0x41f00000,
			 0x3f800000,
			 0x41e00000,
			 0x41c80000,
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
		"fnmsub.ps f9, f13, f13, f30, rup\n"                  ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f26, f7, f2, f20, dyn\n"                   ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f19, f25, f16, f9, rup\n"                  ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f24, f13, f16, f6, rdn\n"                  ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f2, f16, f17, f21, rne\n"                  ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f17, f27, f27, f25, rmm\n"                 ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f6, f18, f10, f26, rdn\n"                  ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f13, f16, f18, f1, dyn\n"                  ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f8, f0, f17, f3, rmm\n"                    ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f13, f10, f27, f7, rdn\n"                  ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f21, f15, f26, f2, rtz\n"                  ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f4, f30, f8, f4, rdn\n"                    ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f20, f6, f8, f11, rtz\n"                   ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f26, f17, f27, f19, rup\n"                 ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f8, f22, f12, f15, rne\n"                  ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f3, f17, f7, f8, rdn\n"                    ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f20, f28, f7, f10, dyn\n"                  ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f24, f18, f24, f29, rdn\n"                 ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f4, f28, f12, f14, rne\n"                  ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f19, f7, f5, f25, rne\n"                   ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f30, f0, f26, f23, rmm\n"                  ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f2, f1, f6, f5, rup\n"                     ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f11, f30, f25, f26, rtz\n"                 ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f8, f12, f13, f26, rne\n"                  ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f10, f7, f2, f13, dyn\n"                   ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f20, f25, f7, f28, dyn\n"                  ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f22, f18, f10, f26, rup\n"                 ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f7, f16, f4, f2, rup\n"                    ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f2, f13, f9, f22, rtz\n"                   ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f23, f6, f23, f8, rdn\n"                   ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f29, f11, f18, f18, rdn\n"                 ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f14, f2, f10, f22, rmm\n"                  ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f4, f26, f22, f9, rtz\n"                   ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f25, f10, f15, f27, rtz\n"                 ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f5, f26, f15, f3, rdn\n"                   ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f29, f23, f30, f8, rdn\n"                  ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f15, f15, f21, f3, rne\n"                  ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f18, f22, f5, f4, rup\n"                   ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f13, f27, f21, f19, rup\n"                 ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f27, f14, f20, f27, dyn\n"                 ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f20, f14, f7, f25, rup\n"                  ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f8, f23, f30, f3, rtz\n"                   ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f9, f9, f9, f30, rmm\n"                    ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f18, f28, f28, f13, rtz\n"                 ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f3, f0, f18, f14, rup\n"                   ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f26, f24, f11, f21, rup\n"                 ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f26, f6, f4, f30, rup\n"                   ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f17, f15, f25, f16, dyn\n"                 ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f27, f10, f28, f4, rmm\n"                  ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f3, f28, f27, f11, rdn\n"                  ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f30, f20, f17, f30, rtz\n"                 ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f26, f24, f24, f10, dyn\n"                 ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f18, f6, f22, f21, rmm\n"                  ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f10, f25, f18, f19, rmm\n"                 ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f30, f19, f23, f10, rdn\n"                 ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f16, f22, f29, f30, rne\n"                 ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f18, f10, f17, f12, rne\n"                 ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f24, f9, f21, f8, rup\n"                   ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f24, f29, f3, f12, rmm\n"                  ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f8, f16, f4, f6, rtz\n"                    ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f26, f28, f18, f14, dyn\n"                 ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f14, f30, f29, f1, rne\n"                  ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f19, f9, f25, f14, rne\n"                  ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f22, f17, f13, f15, rtz\n"                 ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f12, f0, f5, f10, rmm\n"                   ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f16, f20, f9, f27, rtz\n"                  ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f14, f26, f15, f26, rmm\n"                 ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f3, f15, f22, f5, rdn\n"                   ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f3, f6, f2, f14, rne\n"                    ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f20, f0, f29, f29, rup\n"                  ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f16, f6, f4, f1, rdn\n"                    ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f14, f21, f15, f4, rne\n"                  ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f18, f12, f24, f4, rdn\n"                  ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f9, f20, f6, f3, rne\n"                    ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f26, f19, f5, f17, rdn\n"                  ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f22, f18, f21, f24, rdn\n"                 ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f27, f10, f12, f22, dyn\n"                 ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f13, f16, f20, f21, rtz\n"                 ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f19, f28, f18, f9, rmm\n"                  ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f10, f28, f14, f0, rtz\n"                  ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f14, f25, f26, f16, rdn\n"                 ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f30, f2, f3, f22, rne\n"                   ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f10, f0, f19, f5, dyn\n"                   ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f16, f1, f5, f12, rup\n"                   ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f21, f10, f15, f11, dyn\n"                 ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f13, f22, f26, f28, rup\n"                 ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f26, f29, f11, f20, dyn\n"                 ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f30, f24, f13, f15, rdn\n"                 ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f21, f28, f15, f21, rne\n"                 ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f5, f16, f1, f25, rmm\n"                   ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f28, f26, f19, f23, dyn\n"                 ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f23, f27, f5, f0, rmm\n"                   ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f10, f2, f15, f3, rmm\n"                   ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f22, f22, f16, f4, rdn\n"                  ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f2, f3, f7, f20, rne\n"                    ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f25, f11, f28, f30, rmm\n"                 ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f21, f8, f25, f14, rtz\n"                  ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f24, f1, f23, f23, rne\n"                  ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnmsub.ps f18, f7, f27, f6, rtz\n"                   ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
