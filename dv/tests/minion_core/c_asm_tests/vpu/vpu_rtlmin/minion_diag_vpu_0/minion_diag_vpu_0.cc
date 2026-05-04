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
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00004
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00008
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0000c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00010
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00014
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00018
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0001c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00024
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00028
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0002c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00030
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00034
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00040
			 0x55555555,                                                  // 4 random values                             /// 00044
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00048
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0004c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00050
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00054
			 0x0e000007,                                                  // Trailing 1s:                                /// 00058
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0005c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00060
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00064
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00068
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00070
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00074
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00078
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0007c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00084
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00088
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0008c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00094
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00098
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0009c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x33333333,                                                  // 4 random values                             /// 000a4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000ac
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000b4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 000bc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000c0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000c4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 000c8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000cc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000d0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000d4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 000d8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000dc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000e8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x7fc00001,                                                  // signaling NaN                               /// 000f4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 000f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000fc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00100
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00104
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00108
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0010c
			 0x33333333,                                                  // 4 random values                             /// 00110
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00114
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00118
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0011c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00120
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00124
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00128
			 0xff800000,                                                  // -inf                                        /// 0012c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00130
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x0e000001,                                                  // Trailing 1s:                                /// 0013c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00140
			 0x00000000,                                                  // zero                                        /// 00144
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00148
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0014c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00150
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00154
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00158
			 0xff800000,                                                  // -inf                                        /// 0015c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00160
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00164
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00168
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0016c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00170
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00174
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00178
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0017c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00180
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00184
			 0x0e000003,                                                  // Trailing 1s:                                /// 00188
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0018c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00190
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00194
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00198
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001a0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001a4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001a8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001ac
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001b8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001bc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001c0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001c4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001c8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001cc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001d8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001e0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001e4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001e8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001ec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001f0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 001f4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001fc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00200
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00204
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00210
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00218
			 0x33333333,                                                  // 4 random values                             /// 0021c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00220
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00224
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00228
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0022c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00230
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00234
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00238
			 0xffc00001,                                                  // -signaling NaN                              /// 0023c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00240
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00248
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0024c
			 0x7f800000,                                                  // inf                                         /// 00250
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00254
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00258
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00260
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00264
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00268
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0026c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x00000000,                                                  // zero                                        /// 00274
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0027c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00280
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00284
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x3f028f5c,                                                  // 0.51                                        /// 0028c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00290
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00294
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00298
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0029c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002a4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002a8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 002ac
			 0xcb000000,                                                  // -8388608.0                                  /// 002b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002b4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002b8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002c0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002c4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 002c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002cc
			 0xcb000000,                                                  // -8388608.0                                  /// 002d0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 002dc
			 0xff800000,                                                  // -inf                                        /// 002e0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 002e8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 002ec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 002f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002f4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002f8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 002fc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00300
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00304
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00308
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0030c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00314
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x80000000,                                                  // -zero                                       /// 0031c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00320
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00324
			 0x7f800000,                                                  // inf                                         /// 00328
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0032c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00330
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00334
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0033c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00340
			 0x4b000000,                                                  // 8388608.0                                   /// 00344
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00348
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0034c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00350
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00354
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00358
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0035c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00360
			 0x4b000000,                                                  // 8388608.0                                   /// 00364
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00368
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0036c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00374
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00378
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0037c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00380
			 0x7f800000,                                                  // inf                                         /// 00384
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00388
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0038c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00390
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00394
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00398
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0039c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003a0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003a4
			 0x0c400000,                                                  // Leading 1s:                                 /// 003a8
			 0x00011111,                                                  // 9.7958E-41                                  /// 003ac
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x0e000007,                                                  // Trailing 1s:                                /// 003b4
			 0x0e000001,                                                  // Trailing 1s:                                /// 003b8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 003bc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003c0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003c8
			 0x0c700000,                                                  // Leading 1s:                                 /// 003cc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003d0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003d4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 003d8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003dc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003e0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003e4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003e8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003ec
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003f0
			 0xbf028f5c,                                                  // -0.51                                       /// 003f4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003f8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 003fc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00400
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x33333333,                                                  // 4 random values                             /// 0040c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00410
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00418
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00420
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00428
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00430
			 0xcb000000,                                                  // -8388608.0                                  /// 00434
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00438
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0043c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00440
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00444
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00448
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0044c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00458
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0045c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00460
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00478
			 0x80011111,                                                  // -9.7958E-41                                 /// 0047c
			 0x55555555,                                                  // 4 random values                             /// 00480
			 0xcb000000,                                                  // -8388608.0                                  /// 00484
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00488
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0048c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00490
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00494
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00498
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004a8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004b0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004b4
			 0x3f028f5c,                                                  // 0.51                                        /// 004b8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004bc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004c0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004c4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004c8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004cc
			 0xcb000000,                                                  // -8388608.0                                  /// 004d0
			 0x0e000007,                                                  // Trailing 1s:                                /// 004d4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004d8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 004dc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 004e0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004e4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004e8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004f0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004f4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 004f8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004fc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00504
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00508
			 0x0e000007,                                                  // Trailing 1s:                                /// 0050c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x33333333,                                                  // 4 random values                             /// 00514
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00518
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0051c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00520
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00528
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0052c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00530
			 0x80011111,                                                  // -9.7958E-41                                 /// 00534
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00538
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00548
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0054c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00550
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00554
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00558
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0055c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00560
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00564
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00568
			 0xff800000,                                                  // -inf                                        /// 0056c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00574
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00578
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0057c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00580
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00584
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00588
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0058c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x0e000001,                                                  // Trailing 1s:                                /// 00598
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0059c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005a0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005a4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005a8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x3f028f5c,                                                  // 0.51                                        /// 005b4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005b8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 005bc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 005c0
			 0xcb000000,                                                  // -8388608.0                                  /// 005c4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005c8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005cc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005d0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005d8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005dc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005e4
			 0x80011111,                                                  // -9.7958E-41                                 /// 005e8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ec
			 0x55555555,                                                  // 4 random values                             /// 005f0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 005f8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00600
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00604
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00608
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0060c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00610
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00614
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00618
			 0x7f800000,                                                  // inf                                         /// 0061c
			 0xff800000,                                                  // -inf                                        /// 00620
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00624
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00628
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0062c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00630
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00634
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00638
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0063c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00640
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00644
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00648
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0064c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00650
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00654
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00658
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0065c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00660
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00664
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00668
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0066c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00670
			 0x7fc00001,                                                  // signaling NaN                               /// 00674
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00678
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00688
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0068c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00694
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00698
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0069c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006a0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006a8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006ac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006b4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006bc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006c0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006c4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006c8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006d0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006d4
			 0x0c700000,                                                  // Leading 1s:                                 /// 006d8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 006dc
			 0x00011111,                                                  // 9.7958E-41                                  /// 006e0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006e8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006ec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006f0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006f4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006fc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00704
			 0x0c600000,                                                  // Leading 1s:                                 /// 00708
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0070c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00710
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00714
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00718
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0071c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00720
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00724
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00728
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0072c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00730
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00734
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00738
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0073c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00740
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x7fc00001,                                                  // signaling NaN                               /// 00750
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00754
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00758
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0075c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00760
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00768
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0076c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00770
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00778
			 0x0c780000,                                                  // Leading 1s:                                 /// 0077c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00780
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00784
			 0x33333333,                                                  // 4 random values                             /// 00788
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0078c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00790
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00798
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0079c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x00000000,                                                  // zero                                        /// 007a4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007ac
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007b0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007b4
			 0xcb000000,                                                  // -8388608.0                                  /// 007b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 007bc
			 0x00000000,                                                  // zero                                        /// 007c0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007c8
			 0x33333333,                                                  // 4 random values                             /// 007cc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x55555555,                                                  // 4 random values                             /// 007d4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007d8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007dc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007e0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 007e4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007e8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007ec
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 007f8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00804
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0080c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00814
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00818
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0081c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00820
			 0x0c600000,                                                  // Leading 1s:                                 /// 00824
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00828
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0082c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00834
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00838
			 0x7f800000,                                                  // inf                                         /// 0083c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00848
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0084c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00850
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00854
			 0xffc00001,                                                  // -signaling NaN                              /// 00858
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0085c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00860
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00864
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00868
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00870
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00874
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00878
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0087c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00880
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00884
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00888
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0088c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00890
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00898
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0089c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 008a0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008a4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008a8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008ac
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 008b0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008b4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008b8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008bc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 008c0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008c8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008cc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008d0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 008d4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008dc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008e0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008e4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 008e8
			 0xbf028f5c,                                                  // -0.51                                       /// 008ec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008f0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008f4
			 0x00011111,                                                  // 9.7958E-41                                  /// 008f8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 008fc
			 0xcb000000,                                                  // -8388608.0                                  /// 00900
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00904
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00908
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0090c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00910
			 0x00011111,                                                  // 9.7958E-41                                  /// 00914
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00918
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0091c
			 0xbf028f5c,                                                  // -0.51                                       /// 00920
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00924
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00930
			 0x7fc00001,                                                  // signaling NaN                               /// 00934
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00938
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0093c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00944
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00948
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0094c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00954
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00958
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0095c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00960
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00964
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00968
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0096c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00970
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00978
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0097c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00980
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00984
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00988
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0098c
			 0x33333333,                                                  // 4 random values                             /// 00990
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00994
			 0x0e000003,                                                  // Trailing 1s:                                /// 00998
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0099c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x55555555,                                                  // 4 random values                             /// 009a4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 009ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009b0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009b4
			 0x0c780000,                                                  // Leading 1s:                                 /// 009b8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 009bc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009c4
			 0xffc00001,                                                  // -signaling NaN                              /// 009c8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009cc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 009d0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 009d4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 009d8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009dc
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009ec
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009f0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009f4
			 0x0c700000,                                                  // Leading 1s:                                 /// 009f8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 009fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a00
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00a04
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a08
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a0c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a10
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a18
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a20
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a24
			 0x0c600000,                                                  // Leading 1s:                                 /// 00a28
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a30
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00a34
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a38
			 0x00000000,                                                  // zero                                        /// 00a3c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a44
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a48
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a54
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a58
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a60
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a64
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a68
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a6c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a70
			 0x3f028f5c,                                                  // 0.51                                        /// 00a74
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a78
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a7c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00a80
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a84
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a88
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a8c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a90
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a94
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a98
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a9c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x3f028f5c,                                                  // 0.51                                        /// 00aa4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00aa8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00aac
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ab0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x7f800000,                                                  // inf                                         /// 00ab8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00abc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ac0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ac8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00acc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ad0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ad4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00adc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ae0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ae8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00aec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00af0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00af4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00af8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00afc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b00
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b04
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b08
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b0c
			 0x33333333,                                                  // 4 random values                             /// 00b10
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00b18
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b20
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b24
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b28
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b30
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b38
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b3c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b40
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b44
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b48
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b4c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b50
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b54
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b5c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x33333333,                                                  // 4 random values                             /// 00b68
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b6c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b70
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b78
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b80
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b88
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b8c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b90
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00b98
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b9c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ba0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ba4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x80011111,                                                  // -9.7958E-41                                 /// 00bb0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00bb4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00bb8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x3f028f5c,                                                  // 0.51                                        /// 00bc0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00bc8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bcc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bd8
			 0x7fc00001,                                                  // signaling NaN                               /// 00bdc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00be0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00be4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00be8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00bf8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00bfc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c04
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c08
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c0c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c10
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c18
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00c20
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c24
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00c28
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c2c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c30
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c38
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c3c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c40
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c44
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c48
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c4c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c50
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00c54
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c58
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c5c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c60
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c64
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c68
			 0x80000000,                                                  // -zero                                       /// 00c6c
			 0x7fc00001,                                                  // signaling NaN                               /// 00c70
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x3f028f5c,                                                  // 0.51                                        /// 00c78
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c7c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c88
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c8c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00c90
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c98
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c9c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ca0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ca4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ca8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00cb0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cbc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00cc0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cc4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00cc8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ccc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cd0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cd4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cd8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cdc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ce4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00cec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00cf0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cf4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00cf8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cfc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d00
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d04
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d08
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d0c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d18
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d1c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d28
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x7f800000,                                                  // inf                                         /// 00d30
			 0x4b000000,                                                  // 8388608.0                                   /// 00d34
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d3c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d40
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d4c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d50
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d54
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d58
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d5c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d60
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00d64
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d68
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d6c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d70
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00d74
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d78
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d7c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d80
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d84
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d8c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d90
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d94
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d98
			 0xcb000000,                                                  // -8388608.0                                  /// 00d9c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00da4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00dac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00db0
			 0x7f800000,                                                  // inf                                         /// 00db4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dbc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dc4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dc8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00dcc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00dd0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00dd4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00dd8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ddc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00de0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00de4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00de8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00dec
			 0x0e000001,                                                  // Trailing 1s:                                /// 00df0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00df8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dfc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e00
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e08
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e14
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e18
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e1c
			 0x4b000000,                                                  // 8388608.0                                   /// 00e20
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e24
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e28
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00e2c
			 0x80000000,                                                  // -zero                                       /// 00e30
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e34
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e38
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e3c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00e40
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e48
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e4c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e50
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00e54
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e5c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e64
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e68
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e6c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e70
			 0xffc00001,                                                  // -signaling NaN                              /// 00e74
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e78
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e7c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e80
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e84
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e8c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e94
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e98
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e9c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ea0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ea4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ea8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00eac
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00eb0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00eb4
			 0xcb000000,                                                  // -8388608.0                                  /// 00eb8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ebc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ec4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ec8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ecc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ed0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00ed4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ed8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00edc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ee0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ee4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ee8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00eec
			 0x00000000,                                                  // zero                                        /// 00ef0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f00
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x80000000,                                                  // -zero                                       /// 00f08
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f0c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f10
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f14
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00f18
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f1c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f20
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f24
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f28
			 0xffc00001,                                                  // -signaling NaN                              /// 00f2c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f30
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f34
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f38
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f40
			 0x33333333,                                                  // 4 random values                             /// 00f44
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f48
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f4c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f50
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f54
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f58
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f5c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00f64
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f68
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f6c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f70
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f74
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f78
			 0xff800000,                                                  // -inf                                        /// 00f7c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00f80
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f84
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f88
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f8c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f90
			 0xcb000000,                                                  // -8388608.0                                  /// 00f94
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00f98
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fa0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fa8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fac
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fb0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fb4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fb8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fbc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00fc0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fc4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00fc8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00fcc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fd0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fd8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fdc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fe0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fe4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fe8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ff0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ff4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ff8
			 0xffc00000                                                  // -cquiet NaN                                 /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00000
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00004
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0xbf028f5c,                                                  // -0.51                                       /// 0000c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00010
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00014
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00020
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00024
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00028
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00034
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00038
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0003c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00040
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00044
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0004c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00050
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00054
			 0xff800000,                                                  // -inf                                        /// 00058
			 0x7f800000,                                                  // inf                                         /// 0005c
			 0x80000000,                                                  // -zero                                       /// 00060
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00064
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00068
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0006c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00070
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00074
			 0x7f800000,                                                  // inf                                         /// 00078
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0007c
			 0xffc00001,                                                  // -signaling NaN                              /// 00080
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00084
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00088
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00090
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00094
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00098
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0009c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000a0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000a4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 000a8
			 0xcb000000,                                                  // -8388608.0                                  /// 000ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000b0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000b4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000c0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000c4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000c8
			 0x0e000003,                                                  // Trailing 1s:                                /// 000cc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 000d4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 000d8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 000dc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 000e0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000e4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 000e8
			 0xff800000,                                                  // -inf                                        /// 000ec
			 0x3f028f5c,                                                  // 0.51                                        /// 000f0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000f4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 000f8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000fc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00100
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00108
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0010c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00114
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00118
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0011c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00120
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00124
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0012c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00138
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0013c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00140
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00148
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0014c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00150
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00154
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00158
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00160
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00164
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00168
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0016c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00170
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00174
			 0xbf028f5c,                                                  // -0.51                                       /// 00178
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0017c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00180
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00184
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00188
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0018c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00190
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00194
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001a0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001a4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 001ac
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001b0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001b4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001bc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001c0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001c8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001cc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 001d0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001d4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001d8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001dc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001e4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001ec
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x0c700000,                                                  // Leading 1s:                                 /// 001f4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001f8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001fc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0xff800000,                                                  // -inf                                        /// 00204
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0020c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00210
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00214
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00218
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0021c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00220
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00224
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00228
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0022c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00230
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00234
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00238
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0023c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00240
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00244
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00248
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0024c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00250
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00258
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0025c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00264
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0026c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00270
			 0xbf028f5c,                                                  // -0.51                                       /// 00274
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00278
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0027c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00280
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00284
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00288
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0028c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00290
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00294
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00298
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0029c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 002a0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 002a4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002ac
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002b0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 002b4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002b8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002bc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002c0
			 0xcb000000,                                                  // -8388608.0                                  /// 002c4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002c8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002d0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002d8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002dc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002e4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 002e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002ec
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002f0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002fc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00300
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0xff800000,                                                  // -inf                                        /// 00308
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0030c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00310
			 0xffc00001,                                                  // -signaling NaN                              /// 00314
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00318
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0031c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00324
			 0x80011111,                                                  // -9.7958E-41                                 /// 00328
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00330
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00334
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00338
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0033c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00340
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00344
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00348
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0034c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00350
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00354
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00358
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0035c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00360
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00364
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00368
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0036c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00370
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00374
			 0xbf028f5c,                                                  // -0.51                                       /// 00378
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00384
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0038c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00390
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00394
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00398
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0039c
			 0x0c400000,                                                  // Leading 1s:                                 /// 003a0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003a8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 003b0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 003b4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003b8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 003bc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 003c0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003c8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 003cc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 003d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x0e000003,                                                  // Trailing 1s:                                /// 003d8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003dc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003e0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 003e4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 003e8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003ec
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 003f4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003f8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003fc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00400
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00404
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00408
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0040c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00410
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00414
			 0x4b000000,                                                  // 8388608.0                                   /// 00418
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00420
			 0x7f800000,                                                  // inf                                         /// 00424
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0042c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00430
			 0x00011111,                                                  // 9.7958E-41                                  /// 00434
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00438
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0043c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00440
			 0x80011111,                                                  // -9.7958E-41                                 /// 00444
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00448
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00450
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x33333333,                                                  // 4 random values                             /// 00458
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0045c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00460
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00468
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0046c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00470
			 0x0e000003,                                                  // Trailing 1s:                                /// 00474
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0047c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00480
			 0x00000000,                                                  // zero                                        /// 00484
			 0x0c700000,                                                  // Leading 1s:                                 /// 00488
			 0x0e000001,                                                  // Trailing 1s:                                /// 0048c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00490
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00494
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x00011111,                                                  // 9.7958E-41                                  /// 0049c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 004a0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004a4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004a8
			 0x00000000,                                                  // zero                                        /// 004ac
			 0x0c400000,                                                  // Leading 1s:                                 /// 004b0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004b4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004b8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004bc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004c0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004c4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004c8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004cc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004d0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004d4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004d8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 004dc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004e0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 004e8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004ec
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004f0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004f4
			 0x00011111,                                                  // 9.7958E-41                                  /// 004f8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 004fc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00500
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00504
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00508
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0050c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00510
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00514
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0051c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00520
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00524
			 0x80000000,                                                  // -zero                                       /// 00528
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 0052c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00530
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0053c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00540
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00544
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00548
			 0x0e000003,                                                  // Trailing 1s:                                /// 0054c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00550
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00558
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0055c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00560
			 0x55555555,                                                  // 4 random values                             /// 00564
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00568
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0056c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00574
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00578
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0057c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00584
			 0x7f800000,                                                  // inf                                         /// 00588
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0058c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00590
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00594
			 0x0e000001,                                                  // Trailing 1s:                                /// 00598
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0059c
			 0x80011111,                                                  // -9.7958E-41                                 /// 005a0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 005a4
			 0xffc00001,                                                  // -signaling NaN                              /// 005a8
			 0x00000000,                                                  // zero                                        /// 005ac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005b0
			 0x33333333,                                                  // 4 random values                             /// 005b4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005c4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005c8
			 0x0e000001,                                                  // Trailing 1s:                                /// 005cc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005d0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005d8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005dc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005e0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005e4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005e8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 005ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005f0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 005f4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005fc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00600
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00604
			 0x4b000000,                                                  // 8388608.0                                   /// 00608
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0060c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00610
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00614
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00618
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0061c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00620
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00624
			 0x0c780000,                                                  // Leading 1s:                                 /// 00628
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0062c
			 0x55555555,                                                  // 4 random values                             /// 00630
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00634
			 0x7fc00001,                                                  // signaling NaN                               /// 00638
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0063c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00640
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00644
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00648
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0064c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00654
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00658
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0065c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0x80000000,                                                  // -zero                                       /// 00668
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0066c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00670
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00678
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00684
			 0x0e000007,                                                  // Trailing 1s:                                /// 00688
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0068c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00690
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00694
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00698
			 0x00011111,                                                  // 9.7958E-41                                  /// 0069c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006a0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ac
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006b0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 006b4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006b8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006bc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 006c0
			 0x3f028f5c,                                                  // 0.51                                        /// 006c4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 006c8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 006cc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006d4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 006e0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006e4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006ec
			 0x0e003fff,                                                  // Trailing 1s:                                /// 006f0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006f4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006f8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 006fc
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00700
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00704
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00708
			 0x0e000001,                                                  // Trailing 1s:                                /// 0070c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00710
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00714
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00718
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0071c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00720
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00724
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00728
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0072c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00734
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0073c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00740
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00744
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00748
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0074c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00750
			 0xff800000,                                                  // -inf                                        /// 00754
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00758
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0075c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00764
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0076c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00770
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00774
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00778
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0077c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00780
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00784
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00788
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x3f028f5c,                                                  // 0.51                                        /// 00790
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00794
			 0x3f028f5c,                                                  // 0.51                                        /// 00798
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0079c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007a0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 007ac
			 0x80011111,                                                  // -9.7958E-41                                 /// 007b0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 007b4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007b8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 007bc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007c0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007c8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007cc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007d8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007dc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e0
			 0x7fc00001,                                                  // signaling NaN                               /// 007e4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007e8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ec
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007f0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007f4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007fc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00800
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00804
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00808
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0080c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00810
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00814
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00818
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0081c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00820
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00824
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00828
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0082c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00830
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00834
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00838
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0083c
			 0x7fc00001,                                                  // signaling NaN                               /// 00840
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00844
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00848
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0084c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0085c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00860
			 0x0c780000,                                                  // Leading 1s:                                 /// 00864
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00868
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00870
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00878
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0087c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00880
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00884
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0088c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00890
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00898
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0089c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008a0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 008a4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0xcb000000,                                                  // -8388608.0                                  /// 008ac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008b0
			 0x0c600000,                                                  // Leading 1s:                                 /// 008b4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008bc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008c0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008c4
			 0x3f028f5c,                                                  // 0.51                                        /// 008c8
			 0x4b000000,                                                  // 8388608.0                                   /// 008cc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 008d4
			 0x0c400000,                                                  // Leading 1s:                                 /// 008d8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008dc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008e0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008e4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008e8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 008ec
			 0x00000000,                                                  // zero                                        /// 008f0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008f4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008f8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x3f028f5c,                                                  // 0.51                                        /// 00904
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00908
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00910
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00914
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00918
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0091c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00920
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00924
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00928
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0092c
			 0x4b000000,                                                  // 8388608.0                                   /// 00930
			 0x0c400000,                                                  // Leading 1s:                                 /// 00934
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00938
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0093c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00940
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00944
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00948
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0094c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00950
			 0x0c700000,                                                  // Leading 1s:                                 /// 00954
			 0xff800000,                                                  // -inf                                        /// 00958
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0095c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00960
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00964
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0096c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00970
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00978
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00980
			 0xff800000,                                                  // -inf                                        /// 00984
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0098c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00990
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00994
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00998
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0099c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 009a0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009a4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009ac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009b0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009b4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009b8
			 0xbf028f5c,                                                  // -0.51                                       /// 009bc
			 0x0c700000,                                                  // Leading 1s:                                 /// 009c0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009c4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009c8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009cc
			 0x33333333,                                                  // 4 random values                             /// 009d0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009d4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009dc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 009e0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 009ec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009f0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 009f8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009fc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00a00
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a04
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a08
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a0c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a10
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a14
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a18
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a1c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a20
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a24
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a30
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a34
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a38
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a3c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a40
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a44
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a48
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a4c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a50
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a54
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a58
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a5c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a60
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a64
			 0x55555555,                                                  // 4 random values                             /// 00a68
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00a6c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a70
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a74
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a78
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a7c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a80
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a84
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a88
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a8c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00a90
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a94
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a9c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00aa0
			 0x33333333,                                                  // 4 random values                             /// 00aa4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00aa8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00aac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ab0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ab4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00abc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ac4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ac8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ad0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ad4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ad8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00adc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ae0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ae8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00af0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00af8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00afc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x55555555,                                                  // 4 random values                             /// 00b04
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b08
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b10
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b14
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b1c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b20
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b24
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b28
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b2c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b30
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b34
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00b38
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b3c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b40
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b44
			 0x80000000,                                                  // -zero                                       /// 00b48
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b4c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b54
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b58
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b5c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b64
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b68
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b6c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b70
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b74
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b78
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00b80
			 0xffc00001,                                                  // -signaling NaN                              /// 00b84
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b88
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b8c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b90
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b98
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b9c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00ba8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bac
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bb4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bbc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bc0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00bc8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00bd0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00bd8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bdc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00be0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00be4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00be8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00bf0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00bf4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00bf8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bfc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00c04
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c08
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c0c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c14
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c18
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c1c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c20
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c24
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c28
			 0x0c700000,                                                  // Leading 1s:                                 /// 00c2c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c30
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c34
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c38
			 0x7f800000,                                                  // inf                                         /// 00c3c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c40
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c44
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c48
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c54
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c58
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c5c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c60
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c64
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x4b000000,                                                  // 8388608.0                                   /// 00c6c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c70
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c74
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c78
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c7c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c80
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c84
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c88
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c8c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c90
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c94
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c98
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cb0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00cb4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cbc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00cc0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00cc4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00cc8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ccc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cd0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00cd4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cd8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00cdc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ce0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00ce4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ce8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00cec
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cf4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cf8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d04
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d08
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d0c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d10
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d14
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d18
			 0xff800000,                                                  // -inf                                        /// 00d1c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00d20
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d24
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d28
			 0x55555555,                                                  // 4 random values                             /// 00d2c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d30
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00d38
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d3c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d40
			 0x4b000000,                                                  // 8388608.0                                   /// 00d44
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d48
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d4c
			 0xbf028f5c,                                                  // -0.51                                       /// 00d50
			 0xbf028f5c,                                                  // -0.51                                       /// 00d54
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d58
			 0x3f028f5c,                                                  // 0.51                                        /// 00d5c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d64
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d68
			 0x55555555,                                                  // 4 random values                             /// 00d6c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d70
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d74
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d78
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d7c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00d80
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d88
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d8c
			 0xbf028f5c,                                                  // -0.51                                       /// 00d90
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d94
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d9c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00da4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00da8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00dac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00db0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00db4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00db8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00dbc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00dc0
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00dc4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00dc8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00dcc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00dd0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00dd8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ddc
			 0x7fc00001,                                                  // signaling NaN                               /// 00de0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00de4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00de8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00dec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00df4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00df8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00dfc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e00
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e04
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e08
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e10
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e14
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00e18
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e1c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e20
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e24
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e28
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e2c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e30
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e34
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e40
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e44
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e48
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e58
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e60
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e64
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0xffc00001,                                                  // -signaling NaN                              /// 00e70
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e74
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e78
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e7c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e80
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e84
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e88
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00e8c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e90
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e94
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e98
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e9c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ea0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ea8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eac
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00eb0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00eb4
			 0x0c400000,                                                  // Leading 1s:                                 /// 00eb8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00ebc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ec0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ec4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ecc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ed0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00ed4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ed8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ee0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ee4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ee8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00eec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ef0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ef8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00efc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f00
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f04
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00f08
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f0c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f10
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f14
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f18
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f1c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f24
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f2c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f30
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f34
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f38
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f3c
			 0x7f800000,                                                  // inf                                         /// 00f40
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f48
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f4c
			 0x3f028f5c,                                                  // 0.51                                        /// 00f50
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f54
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f58
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f5c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f60
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f64
			 0x7fc00001,                                                  // signaling NaN                               /// 00f68
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f78
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f7c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f80
			 0x80000000,                                                  // -zero                                       /// 00f84
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f88
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f8c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f90
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00f94
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f98
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00fa0
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fa4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00fac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fb0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00fb4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fb8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fbc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fc4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00fc8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fcc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00fd0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fd4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fd8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00fdc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fe4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fec
			 0x7fc00001,                                                  // signaling NaN                               /// 00ff0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ff4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ff8
			 0xAAAAAAAA                                                  // 4 random values                             /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x9cbaabae, /// 0x0
			 0x5e95e671, /// 0x4
			 0xef740dac, /// 0x8
			 0x2b13cda8, /// 0xc
			 0x29a86b85, /// 0x10
			 0xbd0fe70c, /// 0x14
			 0xf0c375de, /// 0x18
			 0x645f908f, /// 0x1c
			 0x03989eb6, /// 0x20
			 0xc93c2b05, /// 0x24
			 0x9be7dfb4, /// 0x28
			 0xa7f2bc0a, /// 0x2c
			 0x23e2ecad, /// 0x30
			 0x7d7e1e69, /// 0x34
			 0x8be87ec5, /// 0x38
			 0x77fb9a4d, /// 0x3c
			 0x46b8a996, /// 0x40
			 0xfa60d49f, /// 0x44
			 0xdb19b215, /// 0x48
			 0xccdb8969, /// 0x4c
			 0xa619d4ed, /// 0x50
			 0x545da2af, /// 0x54
			 0x68599839, /// 0x58
			 0x70e33659, /// 0x5c
			 0x2f88927d, /// 0x60
			 0x901a0977, /// 0x64
			 0xde66dd05, /// 0x68
			 0x41e7441a, /// 0x6c
			 0x88c18090, /// 0x70
			 0x059a2d29, /// 0x74
			 0x59ba66a0, /// 0x78
			 0xb6d9ada7, /// 0x7c
			 0xe9c54487, /// 0x80
			 0x7ea1b6d2, /// 0x84
			 0x936376e9, /// 0x88
			 0xefb10733, /// 0x8c
			 0xce8dff10, /// 0x90
			 0xfd36dbcc, /// 0x94
			 0xc48ee94f, /// 0x98
			 0x868f186b, /// 0x9c
			 0xdf5428ff, /// 0xa0
			 0x39472906, /// 0xa4
			 0x803c35e2, /// 0xa8
			 0x041ccc61, /// 0xac
			 0xe0b9f5ed, /// 0xb0
			 0xeb2ccc0f, /// 0xb4
			 0x8f37e75f, /// 0xb8
			 0xaa1b9187, /// 0xbc
			 0x0c144d58, /// 0xc0
			 0xb3d822d3, /// 0xc4
			 0xa1cfbc95, /// 0xc8
			 0xd3bb3978, /// 0xcc
			 0x54664dcf, /// 0xd0
			 0xd8068a4c, /// 0xd4
			 0xce11ed3d, /// 0xd8
			 0xb9f568d2, /// 0xdc
			 0x561fe046, /// 0xe0
			 0x09bd1e00, /// 0xe4
			 0x4bd6d607, /// 0xe8
			 0xa6f31f93, /// 0xec
			 0xfe848660, /// 0xf0
			 0xd938c102, /// 0xf4
			 0x370c18df, /// 0xf8
			 0x3f7175fa, /// 0xfc
			 0x661cc50b, /// 0x100
			 0xe40725ec, /// 0x104
			 0x47dfe63c, /// 0x108
			 0x12965b7e, /// 0x10c
			 0x3a327085, /// 0x110
			 0xc3b66396, /// 0x114
			 0x4e028e07, /// 0x118
			 0x393b0e76, /// 0x11c
			 0xe42335a0, /// 0x120
			 0x09b08464, /// 0x124
			 0x4bd907d9, /// 0x128
			 0x5f6799df, /// 0x12c
			 0x640049a0, /// 0x130
			 0xde616ac2, /// 0x134
			 0x9c2c4a09, /// 0x138
			 0x5e6f949b, /// 0x13c
			 0xd302879d, /// 0x140
			 0x27619471, /// 0x144
			 0x88e5d6ca, /// 0x148
			 0x66f2e6ac, /// 0x14c
			 0xa3781ed5, /// 0x150
			 0x59faa7df, /// 0x154
			 0xe7d59393, /// 0x158
			 0x4be2baf2, /// 0x15c
			 0xe6062bf6, /// 0x160
			 0x527c8f8c, /// 0x164
			 0x0c4160c1, /// 0x168
			 0xc03d5979, /// 0x16c
			 0x2e7aefcc, /// 0x170
			 0xe61d54d3, /// 0x174
			 0xd600406d, /// 0x178
			 0xee210d3b, /// 0x17c
			 0x90e77b97, /// 0x180
			 0x18c104d3, /// 0x184
			 0x4c829b6e, /// 0x188
			 0x54725662, /// 0x18c
			 0x32f21e75, /// 0x190
			 0xdb1c75d9, /// 0x194
			 0xdfe20a81, /// 0x198
			 0x596a7e29, /// 0x19c
			 0xe9ed0af3, /// 0x1a0
			 0x472b5674, /// 0x1a4
			 0x65198084, /// 0x1a8
			 0x5d8cbec9, /// 0x1ac
			 0x83d4154b, /// 0x1b0
			 0x347f063b, /// 0x1b4
			 0x637c3244, /// 0x1b8
			 0x4890f56e, /// 0x1bc
			 0x9603b161, /// 0x1c0
			 0xd01b21b6, /// 0x1c4
			 0xd270e806, /// 0x1c8
			 0xbcaa7bf7, /// 0x1cc
			 0xa913c668, /// 0x1d0
			 0xc3aca33a, /// 0x1d4
			 0xa771d76a, /// 0x1d8
			 0xf1a49acb, /// 0x1dc
			 0xbd467d98, /// 0x1e0
			 0x9cfba33b, /// 0x1e4
			 0xb1f8bda4, /// 0x1e8
			 0x22af7e93, /// 0x1ec
			 0x8b82d4cb, /// 0x1f0
			 0x6dd07741, /// 0x1f4
			 0xc6e18630, /// 0x1f8
			 0x2938d602, /// 0x1fc
			 0x809d2360, /// 0x200
			 0xfd088553, /// 0x204
			 0x661f7d92, /// 0x208
			 0xf546854a, /// 0x20c
			 0x3afc611f, /// 0x210
			 0xc98e0c34, /// 0x214
			 0xb461b713, /// 0x218
			 0xa6725e4a, /// 0x21c
			 0xd79442de, /// 0x220
			 0xd897d96d, /// 0x224
			 0xf8a38f07, /// 0x228
			 0xe42d754b, /// 0x22c
			 0x9d2b1e99, /// 0x230
			 0x51352fda, /// 0x234
			 0x12508799, /// 0x238
			 0xaa43d994, /// 0x23c
			 0x5db89b15, /// 0x240
			 0xd64d44dd, /// 0x244
			 0x63fdb841, /// 0x248
			 0x0b3e3518, /// 0x24c
			 0x013c38b0, /// 0x250
			 0x80d83a38, /// 0x254
			 0xb009bb70, /// 0x258
			 0x9b0b43c3, /// 0x25c
			 0x72368f8e, /// 0x260
			 0x525827c7, /// 0x264
			 0xcf0e0b5f, /// 0x268
			 0xac6b39ce, /// 0x26c
			 0x3523c9e2, /// 0x270
			 0x054fae66, /// 0x274
			 0x2360944a, /// 0x278
			 0x5ae40ec4, /// 0x27c
			 0x3db0a724, /// 0x280
			 0xb0cab934, /// 0x284
			 0x583200de, /// 0x288
			 0x0d4ef200, /// 0x28c
			 0x5964bd4e, /// 0x290
			 0x70375d72, /// 0x294
			 0x4e27c050, /// 0x298
			 0x62d586d7, /// 0x29c
			 0x76de98ba, /// 0x2a0
			 0x1729fd58, /// 0x2a4
			 0xa4d64540, /// 0x2a8
			 0xe96c4a86, /// 0x2ac
			 0xadbc08a7, /// 0x2b0
			 0x509847bb, /// 0x2b4
			 0xb190e7c1, /// 0x2b8
			 0x25f1c297, /// 0x2bc
			 0x6e39b0e3, /// 0x2c0
			 0xae8fd93a, /// 0x2c4
			 0x3cc2e55f, /// 0x2c8
			 0x7c3e5a0f, /// 0x2cc
			 0xd295488e, /// 0x2d0
			 0x618849e8, /// 0x2d4
			 0x25c99eb6, /// 0x2d8
			 0x12fde320, /// 0x2dc
			 0x3af0956a, /// 0x2e0
			 0xe5b24205, /// 0x2e4
			 0x98b6e89e, /// 0x2e8
			 0x54cfc6fa, /// 0x2ec
			 0xc1fd373b, /// 0x2f0
			 0x3feb6fe2, /// 0x2f4
			 0x6c61fae4, /// 0x2f8
			 0x34f399a3, /// 0x2fc
			 0xc4c498d8, /// 0x300
			 0x2f246127, /// 0x304
			 0xc948fee3, /// 0x308
			 0xbcee45c3, /// 0x30c
			 0x9ac533c0, /// 0x310
			 0x7dd4bca9, /// 0x314
			 0x593c90e0, /// 0x318
			 0x2501c1f3, /// 0x31c
			 0xfc17c498, /// 0x320
			 0x7d088226, /// 0x324
			 0x6fef0561, /// 0x328
			 0xf12fedae, /// 0x32c
			 0xaf0e93e6, /// 0x330
			 0x61981057, /// 0x334
			 0xea2accf1, /// 0x338
			 0xcef33019, /// 0x33c
			 0x6898670d, /// 0x340
			 0x00906f6c, /// 0x344
			 0x8e727889, /// 0x348
			 0xfd89b87c, /// 0x34c
			 0x9ea2693a, /// 0x350
			 0x49ebb519, /// 0x354
			 0xc25b89b6, /// 0x358
			 0xfd059a33, /// 0x35c
			 0x1d40f3c5, /// 0x360
			 0x7e28f9c1, /// 0x364
			 0x437cc19e, /// 0x368
			 0xcd38a176, /// 0x36c
			 0x34d5842b, /// 0x370
			 0x60bf78c0, /// 0x374
			 0xef73f7f2, /// 0x378
			 0x05b0335d, /// 0x37c
			 0x0f9b7108, /// 0x380
			 0x757a38c4, /// 0x384
			 0x80411544, /// 0x388
			 0xb0e1aa4f, /// 0x38c
			 0xa85b215a, /// 0x390
			 0x1f6ddfc8, /// 0x394
			 0xed8cb20c, /// 0x398
			 0x2adb8268, /// 0x39c
			 0xf77e6425, /// 0x3a0
			 0xfcabe7b3, /// 0x3a4
			 0x092f46bc, /// 0x3a8
			 0x08857d58, /// 0x3ac
			 0xf48f763b, /// 0x3b0
			 0x746ca9af, /// 0x3b4
			 0x2a1a4dbd, /// 0x3b8
			 0xe3587167, /// 0x3bc
			 0x99349856, /// 0x3c0
			 0x10d76948, /// 0x3c4
			 0x9c3d3d95, /// 0x3c8
			 0x9faae96f, /// 0x3cc
			 0x1ad9d967, /// 0x3d0
			 0x853edffc, /// 0x3d4
			 0xc1b44e45, /// 0x3d8
			 0x1245606b, /// 0x3dc
			 0xae11d3ff, /// 0x3e0
			 0xa267aa9d, /// 0x3e4
			 0xf419eaf7, /// 0x3e8
			 0xb4d1efed, /// 0x3ec
			 0xf5a1babb, /// 0x3f0
			 0x5d72ca23, /// 0x3f4
			 0x6605b89b, /// 0x3f8
			 0x02ad7234, /// 0x3fc
			 0x30499d58, /// 0x400
			 0x2ff5266d, /// 0x404
			 0x056e50a6, /// 0x408
			 0x78d6ce03, /// 0x40c
			 0x2ad3729b, /// 0x410
			 0x394de3b9, /// 0x414
			 0x26d77235, /// 0x418
			 0x4f1991bb, /// 0x41c
			 0x43cbdd69, /// 0x420
			 0x5b3d19c1, /// 0x424
			 0xfa01cbf3, /// 0x428
			 0x65a06622, /// 0x42c
			 0xa849a3f2, /// 0x430
			 0xdc263590, /// 0x434
			 0xddc94a15, /// 0x438
			 0xe7f5c677, /// 0x43c
			 0x7abb11aa, /// 0x440
			 0x631122ce, /// 0x444
			 0x8ebee3b3, /// 0x448
			 0x2ad1c939, /// 0x44c
			 0x5bf87f8e, /// 0x450
			 0x75047a6e, /// 0x454
			 0xc1295e93, /// 0x458
			 0x119c1ba8, /// 0x45c
			 0x3c463d80, /// 0x460
			 0x09b3d778, /// 0x464
			 0x29a6a548, /// 0x468
			 0x6b19f921, /// 0x46c
			 0x30c7c0ce, /// 0x470
			 0x7352eda8, /// 0x474
			 0x533af12d, /// 0x478
			 0x11fc9067, /// 0x47c
			 0xcb59f63c, /// 0x480
			 0xaf73300f, /// 0x484
			 0x0c7822b9, /// 0x488
			 0xdb33a730, /// 0x48c
			 0x396d70df, /// 0x490
			 0xf390ade2, /// 0x494
			 0xbe0e2367, /// 0x498
			 0x20cce024, /// 0x49c
			 0xd7ce3212, /// 0x4a0
			 0x0d4706ac, /// 0x4a4
			 0x1d3f6f2d, /// 0x4a8
			 0xb7c832e2, /// 0x4ac
			 0x8761e9b8, /// 0x4b0
			 0xa7a7db0f, /// 0x4b4
			 0x6d7dd974, /// 0x4b8
			 0x5d0bf529, /// 0x4bc
			 0xba9efafe, /// 0x4c0
			 0x65b2e8a7, /// 0x4c4
			 0xb83d11ab, /// 0x4c8
			 0xe63a9e3a, /// 0x4cc
			 0xce099d2a, /// 0x4d0
			 0xb7b7e453, /// 0x4d4
			 0x81917883, /// 0x4d8
			 0x8b31370b, /// 0x4dc
			 0x5fe98cf8, /// 0x4e0
			 0x56551a25, /// 0x4e4
			 0xc392f2b6, /// 0x4e8
			 0xb16671de, /// 0x4ec
			 0x5205cbb9, /// 0x4f0
			 0x4631b60c, /// 0x4f4
			 0xeb936862, /// 0x4f8
			 0xbc7ff9de, /// 0x4fc
			 0xab31bff9, /// 0x500
			 0x860371af, /// 0x504
			 0x1c51f573, /// 0x508
			 0x44757718, /// 0x50c
			 0x6039ecc2, /// 0x510
			 0x9c3506a4, /// 0x514
			 0xaa81f395, /// 0x518
			 0x33258854, /// 0x51c
			 0xf879a662, /// 0x520
			 0x4850f450, /// 0x524
			 0x6e7484a0, /// 0x528
			 0x7afa3337, /// 0x52c
			 0x792ca39e, /// 0x530
			 0x4c8fa191, /// 0x534
			 0x29488057, /// 0x538
			 0xac9f2ca4, /// 0x53c
			 0xd8bc4cde, /// 0x540
			 0x4e650c81, /// 0x544
			 0x2ed89412, /// 0x548
			 0x59626033, /// 0x54c
			 0x6df15f82, /// 0x550
			 0x4d5fdd35, /// 0x554
			 0xae3cdbb7, /// 0x558
			 0xfb1436ad, /// 0x55c
			 0xa9f9a90c, /// 0x560
			 0xb7927167, /// 0x564
			 0x82520ed5, /// 0x568
			 0x35a2b2e9, /// 0x56c
			 0xc5014e04, /// 0x570
			 0x556b503a, /// 0x574
			 0xa114b9d4, /// 0x578
			 0x5c97d5e2, /// 0x57c
			 0x12d7f94b, /// 0x580
			 0x5a96947a, /// 0x584
			 0xf4d15f19, /// 0x588
			 0xbb5735a2, /// 0x58c
			 0x72b76f00, /// 0x590
			 0xb3d8ce30, /// 0x594
			 0xe3cebc19, /// 0x598
			 0xd4c5cbae, /// 0x59c
			 0x78094160, /// 0x5a0
			 0x4d7def56, /// 0x5a4
			 0xf8bb8cf3, /// 0x5a8
			 0x7de24a6a, /// 0x5ac
			 0xc430cb9c, /// 0x5b0
			 0x4e6e2e33, /// 0x5b4
			 0xdedc30d2, /// 0x5b8
			 0x00ebc715, /// 0x5bc
			 0x73ac2594, /// 0x5c0
			 0x4e88883b, /// 0x5c4
			 0x997453bf, /// 0x5c8
			 0x59bb0e8d, /// 0x5cc
			 0x47cf4edc, /// 0x5d0
			 0x78bbc5cd, /// 0x5d4
			 0x80372cff, /// 0x5d8
			 0x26348bf7, /// 0x5dc
			 0xdf9577d6, /// 0x5e0
			 0x0d7cd3ce, /// 0x5e4
			 0xe71c6c95, /// 0x5e8
			 0x73a05c0b, /// 0x5ec
			 0x1693df7a, /// 0x5f0
			 0x9e035b14, /// 0x5f4
			 0x97b5c83d, /// 0x5f8
			 0xba7cc5bf, /// 0x5fc
			 0x2eb4b314, /// 0x600
			 0xe1cf994f, /// 0x604
			 0xd4ed0082, /// 0x608
			 0x5d0fa4c2, /// 0x60c
			 0x48da8ffe, /// 0x610
			 0x67a30891, /// 0x614
			 0x0461c97b, /// 0x618
			 0x3f0ee143, /// 0x61c
			 0xc74f3038, /// 0x620
			 0x22de58c6, /// 0x624
			 0x7233fad4, /// 0x628
			 0x14504819, /// 0x62c
			 0x4a1673ba, /// 0x630
			 0x56fe7ce0, /// 0x634
			 0xa472b51b, /// 0x638
			 0x54a76c2f, /// 0x63c
			 0x619eae0c, /// 0x640
			 0x680c74f2, /// 0x644
			 0x64d81e2c, /// 0x648
			 0x67a3db1d, /// 0x64c
			 0x256e5529, /// 0x650
			 0x8eb31b81, /// 0x654
			 0xcad684c0, /// 0x658
			 0x0acd7c2b, /// 0x65c
			 0x51c22419, /// 0x660
			 0x4d8262bc, /// 0x664
			 0x4febd37b, /// 0x668
			 0xe37baccc, /// 0x66c
			 0x2ef9aa6d, /// 0x670
			 0x2c900bf5, /// 0x674
			 0x04aad36e, /// 0x678
			 0x84789a96, /// 0x67c
			 0x3a69b527, /// 0x680
			 0x9178a4fd, /// 0x684
			 0xccd58e8a, /// 0x688
			 0xc0c09291, /// 0x68c
			 0x716a2015, /// 0x690
			 0x0b44805f, /// 0x694
			 0x20bb7a69, /// 0x698
			 0xd57ca702, /// 0x69c
			 0x108e3827, /// 0x6a0
			 0x8ccf6ffc, /// 0x6a4
			 0xc731d0ea, /// 0x6a8
			 0xae77bc9e, /// 0x6ac
			 0x88b77f98, /// 0x6b0
			 0xe2f96cf0, /// 0x6b4
			 0xe5fa46ba, /// 0x6b8
			 0x7c095d2e, /// 0x6bc
			 0x4cea9e43, /// 0x6c0
			 0xc08acde8, /// 0x6c4
			 0xaaa48d07, /// 0x6c8
			 0xe1121a84, /// 0x6cc
			 0xa2ff35ed, /// 0x6d0
			 0x8a3664ba, /// 0x6d4
			 0xe6e81aa6, /// 0x6d8
			 0x7eafe769, /// 0x6dc
			 0xd08dcdc7, /// 0x6e0
			 0xd28f5896, /// 0x6e4
			 0x6e89148c, /// 0x6e8
			 0x72cce0cd, /// 0x6ec
			 0x72e8da9b, /// 0x6f0
			 0x6401c170, /// 0x6f4
			 0xc8a8cbb4, /// 0x6f8
			 0x56f8ee3a, /// 0x6fc
			 0x76124941, /// 0x700
			 0x9db362dd, /// 0x704
			 0x6137b9cd, /// 0x708
			 0x8a603c28, /// 0x70c
			 0xe0fab200, /// 0x710
			 0xe5aa0f94, /// 0x714
			 0xbcbc9e11, /// 0x718
			 0x57d7e4ea, /// 0x71c
			 0x66b4bb81, /// 0x720
			 0x5c366a05, /// 0x724
			 0x37308558, /// 0x728
			 0x02e9f6d6, /// 0x72c
			 0x1625a857, /// 0x730
			 0xf4b2c867, /// 0x734
			 0x9e7587ee, /// 0x738
			 0xcf5917b5, /// 0x73c
			 0xc134817d, /// 0x740
			 0x3e65836b, /// 0x744
			 0xb15d1714, /// 0x748
			 0xc12cb3f2, /// 0x74c
			 0xe824c493, /// 0x750
			 0x242f40df, /// 0x754
			 0x28a1ff12, /// 0x758
			 0xde4f4529, /// 0x75c
			 0x913a6686, /// 0x760
			 0x7f0905de, /// 0x764
			 0x9baf74a2, /// 0x768
			 0x704a7bba, /// 0x76c
			 0xc2f94214, /// 0x770
			 0x5d794009, /// 0x774
			 0x3b943114, /// 0x778
			 0xd27c40a0, /// 0x77c
			 0x39ad3250, /// 0x780
			 0xfe43a97c, /// 0x784
			 0x7227fa77, /// 0x788
			 0xb0146578, /// 0x78c
			 0x61551e68, /// 0x790
			 0xa5e8a737, /// 0x794
			 0x5747ed6a, /// 0x798
			 0x00e658a9, /// 0x79c
			 0x857f3812, /// 0x7a0
			 0x2b3b6f14, /// 0x7a4
			 0xcdd1202c, /// 0x7a8
			 0xfa1d5395, /// 0x7ac
			 0xe0b29937, /// 0x7b0
			 0x91d96b3d, /// 0x7b4
			 0xa71db7bf, /// 0x7b8
			 0xbf02e6b1, /// 0x7bc
			 0xc0aaf373, /// 0x7c0
			 0xf7fb98ea, /// 0x7c4
			 0xc0dc413d, /// 0x7c8
			 0x7fdc5791, /// 0x7cc
			 0x8af096eb, /// 0x7d0
			 0xbd284de7, /// 0x7d4
			 0x77e8a0f7, /// 0x7d8
			 0x18992cb3, /// 0x7dc
			 0xc0834c2b, /// 0x7e0
			 0x954a3c41, /// 0x7e4
			 0x5918ec58, /// 0x7e8
			 0xce09e714, /// 0x7ec
			 0xceb3dcf4, /// 0x7f0
			 0x2250917f, /// 0x7f4
			 0x4ebcd135, /// 0x7f8
			 0x25a13fdc, /// 0x7fc
			 0x547ca1c8, /// 0x800
			 0xb177244a, /// 0x804
			 0x00b9f8f0, /// 0x808
			 0xb249b5a6, /// 0x80c
			 0xece52bf6, /// 0x810
			 0xa7d39622, /// 0x814
			 0x2ed7f79f, /// 0x818
			 0x012b6f3c, /// 0x81c
			 0x37edc6d6, /// 0x820
			 0x40b625ad, /// 0x824
			 0x1fc86d19, /// 0x828
			 0x7c492e78, /// 0x82c
			 0xe4f2074b, /// 0x830
			 0x6ea01675, /// 0x834
			 0xf4cd0f6a, /// 0x838
			 0x47a89bc7, /// 0x83c
			 0x2f390a8a, /// 0x840
			 0x09af02e1, /// 0x844
			 0x33db1b48, /// 0x848
			 0x1f3fa7fe, /// 0x84c
			 0x7085d854, /// 0x850
			 0x1250c66e, /// 0x854
			 0x51029725, /// 0x858
			 0xcd65de7c, /// 0x85c
			 0x3bf74141, /// 0x860
			 0x78414382, /// 0x864
			 0x3d498135, /// 0x868
			 0x7b6116cb, /// 0x86c
			 0x3cce8540, /// 0x870
			 0x9eb297d0, /// 0x874
			 0x39426e02, /// 0x878
			 0x68a985ac, /// 0x87c
			 0x2cc0f050, /// 0x880
			 0x8721bbe6, /// 0x884
			 0xf9161e0d, /// 0x888
			 0x26c9a097, /// 0x88c
			 0x8b320435, /// 0x890
			 0x4806338b, /// 0x894
			 0x8e29eb83, /// 0x898
			 0xc2cdc7d2, /// 0x89c
			 0xdedf257a, /// 0x8a0
			 0xf41bd3a4, /// 0x8a4
			 0x2c1a149e, /// 0x8a8
			 0x867e2ba0, /// 0x8ac
			 0x53ca1334, /// 0x8b0
			 0xb8598c04, /// 0x8b4
			 0xdefaf02f, /// 0x8b8
			 0x483fc049, /// 0x8bc
			 0x07f7c3d5, /// 0x8c0
			 0x404577c1, /// 0x8c4
			 0x0a4e7f17, /// 0x8c8
			 0xeb6c0e9a, /// 0x8cc
			 0x283bac76, /// 0x8d0
			 0x19a59a93, /// 0x8d4
			 0xf4c1a660, /// 0x8d8
			 0xfd4fdbda, /// 0x8dc
			 0x0bd2eae7, /// 0x8e0
			 0xcefa8c0a, /// 0x8e4
			 0x24b2f888, /// 0x8e8
			 0x65224eb6, /// 0x8ec
			 0x17ee86d5, /// 0x8f0
			 0x673658e6, /// 0x8f4
			 0xcf101cbe, /// 0x8f8
			 0x115385b5, /// 0x8fc
			 0x8370fb99, /// 0x900
			 0x32e0a60b, /// 0x904
			 0xb68fae82, /// 0x908
			 0x3a667f2e, /// 0x90c
			 0x46d25255, /// 0x910
			 0x67638b92, /// 0x914
			 0xbb890312, /// 0x918
			 0x21e43b99, /// 0x91c
			 0x89a6f4b1, /// 0x920
			 0xa9e38276, /// 0x924
			 0x51aac2fc, /// 0x928
			 0xef58f157, /// 0x92c
			 0xda0e133b, /// 0x930
			 0xb894988c, /// 0x934
			 0xaf24e661, /// 0x938
			 0xe4223fe9, /// 0x93c
			 0x98c2fca6, /// 0x940
			 0x08155fe3, /// 0x944
			 0x21305ebf, /// 0x948
			 0x4b8310b0, /// 0x94c
			 0xc8d705cc, /// 0x950
			 0x91889fbd, /// 0x954
			 0x9289b3a7, /// 0x958
			 0x5d832f43, /// 0x95c
			 0x2e40ce10, /// 0x960
			 0x6a7ceb69, /// 0x964
			 0xf05ac369, /// 0x968
			 0x6130a151, /// 0x96c
			 0xc16daade, /// 0x970
			 0x106ec564, /// 0x974
			 0x34eaa15a, /// 0x978
			 0xede40dfe, /// 0x97c
			 0x7d33eb3c, /// 0x980
			 0x71e71fe9, /// 0x984
			 0x82ac9756, /// 0x988
			 0xc55d49af, /// 0x98c
			 0xccb4cded, /// 0x990
			 0xd928d5ae, /// 0x994
			 0x5018d55c, /// 0x998
			 0xbd7783ce, /// 0x99c
			 0xd2f2bf22, /// 0x9a0
			 0xf82f5e5b, /// 0x9a4
			 0x2672b89a, /// 0x9a8
			 0x6ce1abc8, /// 0x9ac
			 0x463adc1b, /// 0x9b0
			 0xe41e844e, /// 0x9b4
			 0xf59d4d39, /// 0x9b8
			 0x8e367767, /// 0x9bc
			 0xf3f48165, /// 0x9c0
			 0x4e80be9d, /// 0x9c4
			 0x1f249983, /// 0x9c8
			 0xbca51252, /// 0x9cc
			 0x6c54b172, /// 0x9d0
			 0x8adf50f7, /// 0x9d4
			 0x2521c113, /// 0x9d8
			 0xa0b11ac8, /// 0x9dc
			 0xa8ef20ff, /// 0x9e0
			 0x5235a34a, /// 0x9e4
			 0x0bb3e980, /// 0x9e8
			 0x0a2b2a61, /// 0x9ec
			 0xb0abf2c8, /// 0x9f0
			 0x72e109c5, /// 0x9f4
			 0xfa99dceb, /// 0x9f8
			 0x42298b27, /// 0x9fc
			 0x755f18b9, /// 0xa00
			 0xd47ee2c0, /// 0xa04
			 0x3bd88332, /// 0xa08
			 0xbd9ac223, /// 0xa0c
			 0x42faa3fa, /// 0xa10
			 0xb2eb5692, /// 0xa14
			 0xae6a24ec, /// 0xa18
			 0x00b0738f, /// 0xa1c
			 0x029eb6c3, /// 0xa20
			 0xab61b431, /// 0xa24
			 0x1037c0a5, /// 0xa28
			 0x6b009b74, /// 0xa2c
			 0xd513ff62, /// 0xa30
			 0xf7367a4e, /// 0xa34
			 0x6ed7eb73, /// 0xa38
			 0xa529f8b2, /// 0xa3c
			 0x2cbcd8b5, /// 0xa40
			 0xfc28e21d, /// 0xa44
			 0xd8e4c859, /// 0xa48
			 0x4036e98e, /// 0xa4c
			 0xdb552e4e, /// 0xa50
			 0x8ae7dc20, /// 0xa54
			 0xb967b6e3, /// 0xa58
			 0x251ae2a8, /// 0xa5c
			 0x2f45a36f, /// 0xa60
			 0xbc18acf4, /// 0xa64
			 0xb6454a33, /// 0xa68
			 0x39108966, /// 0xa6c
			 0xc5d7e13f, /// 0xa70
			 0xc52ce265, /// 0xa74
			 0x8c113bd9, /// 0xa78
			 0xf3409503, /// 0xa7c
			 0x8d8003ff, /// 0xa80
			 0x6589c054, /// 0xa84
			 0x0aca8d34, /// 0xa88
			 0xedfd8313, /// 0xa8c
			 0x20d61724, /// 0xa90
			 0x11a1776f, /// 0xa94
			 0x876dcab6, /// 0xa98
			 0xc2d3ccc1, /// 0xa9c
			 0x8e63bb3d, /// 0xaa0
			 0x9153fb06, /// 0xaa4
			 0x1cec5990, /// 0xaa8
			 0xb3fd774c, /// 0xaac
			 0x5aa00d62, /// 0xab0
			 0xaa75c185, /// 0xab4
			 0x083efa47, /// 0xab8
			 0x3a9f322b, /// 0xabc
			 0x80f6e96c, /// 0xac0
			 0x5c973b2a, /// 0xac4
			 0xabb10009, /// 0xac8
			 0x953395b6, /// 0xacc
			 0x6f138404, /// 0xad0
			 0x3fd5c340, /// 0xad4
			 0x378bc4f3, /// 0xad8
			 0xb5a5a25c, /// 0xadc
			 0x1aaad148, /// 0xae0
			 0xfa792e46, /// 0xae4
			 0xa6cf5b03, /// 0xae8
			 0x6dced2b2, /// 0xaec
			 0x3cf75eb8, /// 0xaf0
			 0x0a5b1237, /// 0xaf4
			 0x81ff40d5, /// 0xaf8
			 0xce4c6967, /// 0xafc
			 0x311977dc, /// 0xb00
			 0x1e7520ca, /// 0xb04
			 0x88b6afa5, /// 0xb08
			 0x36fbeccf, /// 0xb0c
			 0x85cd403d, /// 0xb10
			 0xad5b475a, /// 0xb14
			 0xae6ca4e3, /// 0xb18
			 0x4f388c9c, /// 0xb1c
			 0x01e2153f, /// 0xb20
			 0xf61ecb42, /// 0xb24
			 0x1e8e1bfb, /// 0xb28
			 0xfc5d59ee, /// 0xb2c
			 0x7a72313f, /// 0xb30
			 0x1e166771, /// 0xb34
			 0x6a72b3ab, /// 0xb38
			 0x67bc0e85, /// 0xb3c
			 0x9e55bcc7, /// 0xb40
			 0x9bb274bb, /// 0xb44
			 0xcbdf9972, /// 0xb48
			 0xa18af880, /// 0xb4c
			 0x2697c682, /// 0xb50
			 0xad6ee534, /// 0xb54
			 0x7352aea0, /// 0xb58
			 0x03f081ea, /// 0xb5c
			 0x08549e25, /// 0xb60
			 0x4af4c175, /// 0xb64
			 0x313652b2, /// 0xb68
			 0x2e309425, /// 0xb6c
			 0x40f456b2, /// 0xb70
			 0x3af103f9, /// 0xb74
			 0xf7e5fc2f, /// 0xb78
			 0x3cc91b40, /// 0xb7c
			 0x5ce3cb17, /// 0xb80
			 0xb1162844, /// 0xb84
			 0x0877d5c8, /// 0xb88
			 0x8a31ea69, /// 0xb8c
			 0x8617eda4, /// 0xb90
			 0x3009a90f, /// 0xb94
			 0x8f5a50ba, /// 0xb98
			 0xc9f01559, /// 0xb9c
			 0x8a9efb59, /// 0xba0
			 0xb8119390, /// 0xba4
			 0x0ce7ef27, /// 0xba8
			 0x0dacc81b, /// 0xbac
			 0x9c0449b8, /// 0xbb0
			 0xbbc61093, /// 0xbb4
			 0xa2517864, /// 0xbb8
			 0xc1af9a49, /// 0xbbc
			 0x5f4544ea, /// 0xbc0
			 0xf315ddde, /// 0xbc4
			 0xb29e93a1, /// 0xbc8
			 0x54dbfbec, /// 0xbcc
			 0x0665734c, /// 0xbd0
			 0x4a3c8576, /// 0xbd4
			 0x5096fe38, /// 0xbd8
			 0x46289f7f, /// 0xbdc
			 0x4b4974f0, /// 0xbe0
			 0x9bbb6433, /// 0xbe4
			 0x0e66a2ad, /// 0xbe8
			 0x6578fcf6, /// 0xbec
			 0xdc90c052, /// 0xbf0
			 0x62f5a365, /// 0xbf4
			 0x105f192e, /// 0xbf8
			 0x77eada91, /// 0xbfc
			 0x884c8b79, /// 0xc00
			 0xf82a5969, /// 0xc04
			 0x600ade96, /// 0xc08
			 0x2479f993, /// 0xc0c
			 0xd2a72f71, /// 0xc10
			 0x07e33673, /// 0xc14
			 0x88b82fb9, /// 0xc18
			 0x712834d6, /// 0xc1c
			 0xcced79a7, /// 0xc20
			 0xa5630d4f, /// 0xc24
			 0xad941832, /// 0xc28
			 0x158d6bc2, /// 0xc2c
			 0xd0ed6147, /// 0xc30
			 0xe84df8f4, /// 0xc34
			 0x6343c678, /// 0xc38
			 0xe75343cb, /// 0xc3c
			 0x8241726e, /// 0xc40
			 0x1c03f714, /// 0xc44
			 0x4f8a9627, /// 0xc48
			 0x3a4c0d3b, /// 0xc4c
			 0x2319cbad, /// 0xc50
			 0x32c5f3d5, /// 0xc54
			 0x20c9e5d5, /// 0xc58
			 0xb3698c28, /// 0xc5c
			 0xeeb911a2, /// 0xc60
			 0xa2c06b21, /// 0xc64
			 0x337a038c, /// 0xc68
			 0x79c571dd, /// 0xc6c
			 0x2897bd4c, /// 0xc70
			 0x85112959, /// 0xc74
			 0x1bd543f4, /// 0xc78
			 0x54e300ee, /// 0xc7c
			 0x7b627143, /// 0xc80
			 0x95a00baf, /// 0xc84
			 0xb31d68f8, /// 0xc88
			 0xcc7458fd, /// 0xc8c
			 0xbc19085f, /// 0xc90
			 0xd2ea66b4, /// 0xc94
			 0x04899fc6, /// 0xc98
			 0xb8db77e5, /// 0xc9c
			 0x255cdc5b, /// 0xca0
			 0x09b587a4, /// 0xca4
			 0xce2a2fe3, /// 0xca8
			 0x0cdaca79, /// 0xcac
			 0xd1457ff0, /// 0xcb0
			 0x8959fd79, /// 0xcb4
			 0x10087cb4, /// 0xcb8
			 0x41a0627c, /// 0xcbc
			 0x6672ff5e, /// 0xcc0
			 0xa56a33ad, /// 0xcc4
			 0x2e251b38, /// 0xcc8
			 0x0bb1dc6d, /// 0xccc
			 0xbc5bf786, /// 0xcd0
			 0xd33fdc79, /// 0xcd4
			 0x603b2215, /// 0xcd8
			 0x58843be6, /// 0xcdc
			 0xc89e91f6, /// 0xce0
			 0x0b775ff8, /// 0xce4
			 0x06808421, /// 0xce8
			 0xc70fd567, /// 0xcec
			 0x32b4e89a, /// 0xcf0
			 0xba2e831b, /// 0xcf4
			 0x3dcd269d, /// 0xcf8
			 0xbfc19acd, /// 0xcfc
			 0xe66d50e1, /// 0xd00
			 0x7abd8d47, /// 0xd04
			 0x2a7eab96, /// 0xd08
			 0xe1447e58, /// 0xd0c
			 0x53ee94f2, /// 0xd10
			 0xb9692815, /// 0xd14
			 0x34b49d7a, /// 0xd18
			 0xe40c2d75, /// 0xd1c
			 0x55f407cd, /// 0xd20
			 0x884ccca6, /// 0xd24
			 0x20d7da09, /// 0xd28
			 0xe9bfabdd, /// 0xd2c
			 0x3041f27c, /// 0xd30
			 0x38498de0, /// 0xd34
			 0x6d7d5de2, /// 0xd38
			 0x5a0f92ff, /// 0xd3c
			 0x6ed16432, /// 0xd40
			 0xa3ae9ee6, /// 0xd44
			 0x6638021d, /// 0xd48
			 0xf5465b15, /// 0xd4c
			 0x5d733e76, /// 0xd50
			 0xbf9c8a1b, /// 0xd54
			 0x65b7eee6, /// 0xd58
			 0x168cf659, /// 0xd5c
			 0x19082973, /// 0xd60
			 0xa79a979d, /// 0xd64
			 0x5a25e731, /// 0xd68
			 0x39caafe3, /// 0xd6c
			 0x8702a4ed, /// 0xd70
			 0x39bae40b, /// 0xd74
			 0xc5e8f06b, /// 0xd78
			 0x2403f731, /// 0xd7c
			 0xea096add, /// 0xd80
			 0x5b6bbd08, /// 0xd84
			 0x9b9ff605, /// 0xd88
			 0x4ac5d81b, /// 0xd8c
			 0x30af487b, /// 0xd90
			 0xc7812747, /// 0xd94
			 0xaec4f1d7, /// 0xd98
			 0x34647e59, /// 0xd9c
			 0xf3107a8a, /// 0xda0
			 0x13ef94d4, /// 0xda4
			 0x08c2d3da, /// 0xda8
			 0xce858aed, /// 0xdac
			 0x20469ffa, /// 0xdb0
			 0xfeced62e, /// 0xdb4
			 0xb5d2a024, /// 0xdb8
			 0x726f78d4, /// 0xdbc
			 0xac78fd5a, /// 0xdc0
			 0xbada0634, /// 0xdc4
			 0x09e9d31a, /// 0xdc8
			 0x7fc3e883, /// 0xdcc
			 0x792bb0b1, /// 0xdd0
			 0x2dea6ef6, /// 0xdd4
			 0x798a8396, /// 0xdd8
			 0xd7dba012, /// 0xddc
			 0x8972f9e0, /// 0xde0
			 0xe57aca98, /// 0xde4
			 0x102a2ed4, /// 0xde8
			 0xb5ac1f02, /// 0xdec
			 0xdb475093, /// 0xdf0
			 0xf9e6e97c, /// 0xdf4
			 0xfb63c676, /// 0xdf8
			 0x8d06e7bd, /// 0xdfc
			 0x9edddad8, /// 0xe00
			 0x2d26206a, /// 0xe04
			 0x55f228cf, /// 0xe08
			 0xaad86a5a, /// 0xe0c
			 0xccb80b3f, /// 0xe10
			 0x46396b07, /// 0xe14
			 0x3eb93953, /// 0xe18
			 0x3c04bff9, /// 0xe1c
			 0xf86601e9, /// 0xe20
			 0x76020a37, /// 0xe24
			 0xd42b7e46, /// 0xe28
			 0xdf3757fb, /// 0xe2c
			 0xe1a6db05, /// 0xe30
			 0xf1eb7df4, /// 0xe34
			 0x1d7ec4e3, /// 0xe38
			 0xe8730d7e, /// 0xe3c
			 0x696435ce, /// 0xe40
			 0x61780655, /// 0xe44
			 0x45b5d5a0, /// 0xe48
			 0x09459c31, /// 0xe4c
			 0x9ffa72b7, /// 0xe50
			 0x82f63227, /// 0xe54
			 0x1736271e, /// 0xe58
			 0xacc02547, /// 0xe5c
			 0x1b4466ff, /// 0xe60
			 0x10a9d163, /// 0xe64
			 0xdcdf22fd, /// 0xe68
			 0xcdb0fd06, /// 0xe6c
			 0xa5602d9a, /// 0xe70
			 0x466c9a48, /// 0xe74
			 0x3beee516, /// 0xe78
			 0x08912e30, /// 0xe7c
			 0x5594bda0, /// 0xe80
			 0xda92b024, /// 0xe84
			 0xbdf74d1c, /// 0xe88
			 0x0306370f, /// 0xe8c
			 0x18002914, /// 0xe90
			 0x3937b474, /// 0xe94
			 0xd8a345b8, /// 0xe98
			 0xcfa612d4, /// 0xe9c
			 0xba2d2143, /// 0xea0
			 0x95eaab4c, /// 0xea4
			 0xc3f4e970, /// 0xea8
			 0xce80a46b, /// 0xeac
			 0xb2350650, /// 0xeb0
			 0x50c9c9a6, /// 0xeb4
			 0x1f8281e4, /// 0xeb8
			 0x3a557551, /// 0xebc
			 0x33a87476, /// 0xec0
			 0x7a4cb015, /// 0xec4
			 0xde4adf10, /// 0xec8
			 0x376eed13, /// 0xecc
			 0x0f19fd6d, /// 0xed0
			 0x500041a9, /// 0xed4
			 0x107839c0, /// 0xed8
			 0x3870d33c, /// 0xedc
			 0x72a68956, /// 0xee0
			 0xed2e4634, /// 0xee4
			 0x18c4c067, /// 0xee8
			 0xe90fd41f, /// 0xeec
			 0xe22680da, /// 0xef0
			 0xe42a57d0, /// 0xef4
			 0xb9f59a6e, /// 0xef8
			 0xa08f1fec, /// 0xefc
			 0x109ed332, /// 0xf00
			 0x8a935b53, /// 0xf04
			 0xed0e9eae, /// 0xf08
			 0x411f9c64, /// 0xf0c
			 0x63a03a73, /// 0xf10
			 0x4bfe6d46, /// 0xf14
			 0xe8f2dcf3, /// 0xf18
			 0x59015c83, /// 0xf1c
			 0xe9fcc1de, /// 0xf20
			 0x9f2d9c10, /// 0xf24
			 0xe7f4c9e5, /// 0xf28
			 0xc0ee729c, /// 0xf2c
			 0xb831dd94, /// 0xf30
			 0x06d61ca7, /// 0xf34
			 0xadcd8d71, /// 0xf38
			 0x158f7b30, /// 0xf3c
			 0x6284a432, /// 0xf40
			 0xf0d9e7f1, /// 0xf44
			 0xd8aa17a9, /// 0xf48
			 0x1e0978de, /// 0xf4c
			 0x6e4a2960, /// 0xf50
			 0x1eaad527, /// 0xf54
			 0xb8282ee6, /// 0xf58
			 0x7b798e2a, /// 0xf5c
			 0x36a27146, /// 0xf60
			 0x34b8b6b4, /// 0xf64
			 0x91cc31fd, /// 0xf68
			 0x3519d10c, /// 0xf6c
			 0x77c55472, /// 0xf70
			 0x8e356d2e, /// 0xf74
			 0xa3a8ef8a, /// 0xf78
			 0x19d2b030, /// 0xf7c
			 0xab6f30c0, /// 0xf80
			 0xc01f56c5, /// 0xf84
			 0x23c219d5, /// 0xf88
			 0xc3429148, /// 0xf8c
			 0xe107015d, /// 0xf90
			 0x62edd2e2, /// 0xf94
			 0x50bd3775, /// 0xf98
			 0xaf5b8d6c, /// 0xf9c
			 0xd5fb5f5a, /// 0xfa0
			 0xd499df84, /// 0xfa4
			 0xbd25b788, /// 0xfa8
			 0x4431184d, /// 0xfac
			 0x62adb48f, /// 0xfb0
			 0x407eb2a5, /// 0xfb4
			 0xfc047a01, /// 0xfb8
			 0xb7c28e3b, /// 0xfbc
			 0xc2112f6a, /// 0xfc0
			 0xcb10254c, /// 0xfc4
			 0x4ddfd044, /// 0xfc8
			 0x07834bee, /// 0xfcc
			 0x595be7ca, /// 0xfd0
			 0x8ebbea75, /// 0xfd4
			 0xd67a6f95, /// 0xfd8
			 0x26451434, /// 0xfdc
			 0xbe56b760, /// 0xfe0
			 0x16c53995, /// 0xfe4
			 0x10f09a56, /// 0xfe8
			 0x4825c61d, /// 0xfec
			 0x90d0b3a2, /// 0xff0
			 0x5585145d, /// 0xff4
			 0x6b9a9716, /// 0xff8
			 0xd556edc5 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x0e000003,                                                  // Trailing 1s:                                /// 00000
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00004
			 0x0c700000,                                                  // Leading 1s:                                 /// 00008
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0000c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00010
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00014
			 0x0c400000,                                                  // Leading 1s:                                 /// 00018
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0001c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00020
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00024
			 0x0e000001,                                                  // Trailing 1s:                                /// 00028
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00030
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00034
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00038
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0003c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00044
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00048
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0004c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0xffc00001,                                                  // -signaling NaN                              /// 00058
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0005c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00060
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00064
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00068
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0006c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00070
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00074
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00078
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0007c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00080
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00088
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0008c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00090
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00094
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00098
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0009c
			 0x4b000000,                                                  // 8388608.0                                   /// 000a0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000a4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000a8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 000b0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 000b4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000bc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 000c4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 000c8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000d4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000d8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000dc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 000e4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000e8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 000ec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 000f0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000f8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00104
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00108
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0010c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00110
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00114
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00118
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0011c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00120
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00124
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00128
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0012c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00134
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00138
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0013c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00140
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00144
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00148
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0014c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00150
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00154
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00158
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0015c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00160
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00164
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00168
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0016c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00170
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00178
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0017c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00180
			 0x0c600000,                                                  // Leading 1s:                                 /// 00184
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00188
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0018c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00190
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00194
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00198
			 0x0e000007,                                                  // Trailing 1s:                                /// 0019c
			 0x0e000007,                                                  // Trailing 1s:                                /// 001a0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001a4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001a8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 001ac
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001b0
			 0x0e000001,                                                  // Trailing 1s:                                /// 001b4
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 001b8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001bc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 001c0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 001c4
			 0x0e000007,                                                  // Trailing 1s:                                /// 001c8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001cc
			 0x80000000,                                                  // -zero                                       /// 001d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001d4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 001dc
			 0x80011111,                                                  // -9.7958E-41                                 /// 001e0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001e4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 001e8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001ec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 001f8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001fc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00200
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00204
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00208
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0020c
			 0xff800000,                                                  // -inf                                        /// 00210
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00218
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0021c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00220
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00224
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0022c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00230
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00234
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00238
			 0x0c700000,                                                  // Leading 1s:                                 /// 0023c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00240
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00244
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00248
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0024c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00250
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00254
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00258
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00260
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00268
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00270
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00274
			 0x80011111,                                                  // -9.7958E-41                                 /// 00278
			 0x0e000003,                                                  // Trailing 1s:                                /// 0027c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00280
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00284
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00288
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0028c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00290
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00294
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00298
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0029c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002a0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 002a8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 002ac
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002b0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002b4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 002b8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 002bc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002c0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 002c4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002cc
			 0x7f800000,                                                  // inf                                         /// 002d0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002d4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002d8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002dc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 002e0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002e8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002ec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002f0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 002f4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 002fc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00300
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00304
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00308
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0030c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00310
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00314
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00318
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00320
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00324
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00328
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0032c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00330
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00334
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00338
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00340
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00344
			 0x0c700000,                                                  // Leading 1s:                                 /// 00348
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0034c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00350
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00354
			 0x0c780000,                                                  // Leading 1s:                                 /// 00358
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0035c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00360
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00364
			 0xbf028f5c,                                                  // -0.51                                       /// 00368
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00370
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00374
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00378
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0037c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00380
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00384
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00388
			 0x80011111,                                                  // -9.7958E-41                                 /// 0038c
			 0x7fc00001,                                                  // signaling NaN                               /// 00390
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00394
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00398
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0039c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 003a0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003ac
			 0x7fc00001,                                                  // signaling NaN                               /// 003b0
			 0x0c700000,                                                  // Leading 1s:                                 /// 003b4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003b8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003bc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 003cc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 003d8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 003dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 003e0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003e4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 003ec
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 003f0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003f4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 003f8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003fc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00400
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00404
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00408
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0040c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00410
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00414
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00418
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0041c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00420
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00424
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00428
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0042c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00430
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00434
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00438
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0043c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00440
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00444
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00448
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00450
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00454
			 0x80011111,                                                  // -9.7958E-41                                 /// 00458
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0045c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00460
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00464
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0046c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00470
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00474
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00478
			 0xffc00001,                                                  // -signaling NaN                              /// 0047c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00480
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00484
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00488
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0048c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00490
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00498
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0049c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 004a0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004ac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004bc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 004c0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004c8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004cc
			 0x0e000007,                                                  // Trailing 1s:                                /// 004d0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004d8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 004dc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 004e4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004ec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 004f4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004fc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00500
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00504
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00508
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0050c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00510
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00514
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00518
			 0xcb000000,                                                  // -8388608.0                                  /// 0051c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00520
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00524
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00528
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x3f028f5c,                                                  // 0.51                                        /// 00530
			 0x0e000007,                                                  // Trailing 1s:                                /// 00534
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00538
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00544
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00548
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0054c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00550
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0055c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00560
			 0x0e000007,                                                  // Trailing 1s:                                /// 00564
			 0x4b000000,                                                  // 8388608.0                                   /// 00568
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0056c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00574
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00578
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0057c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00580
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00584
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0058c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00590
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00594
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00598
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005a0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005a4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005a8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 005ac
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005b0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005bc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005c4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005c8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005cc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005d4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005e0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005e4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 005e8
			 0x00011111,                                                  // 9.7958E-41                                  /// 005ec
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005f0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005f4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005f8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005fc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00604
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00608
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0060c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00614
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0061c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00624
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00628
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0062c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00634
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00638
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0063c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00648
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0064c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00650
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00658
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0065c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00660
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00664
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00668
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0066c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00670
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00674
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0067c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00680
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00684
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00688
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0068c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00690
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00694
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00698
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006a0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 006a4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 006a8
			 0x00000000,                                                  // zero                                        /// 006ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006b0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 006b4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 006bc
			 0x3f028f5c,                                                  // 0.51                                        /// 006c0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006cc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006dc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006e8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006ec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006f0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 006f4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 006f8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 006fc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00704
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00708
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0070c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00710
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00714
			 0xff800000,                                                  // -inf                                        /// 00718
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0071c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00720
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0072c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00734
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00738
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0073c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00740
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00744
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0074c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00750
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00754
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00758
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0075c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00760
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00768
			 0x7f800000,                                                  // inf                                         /// 0076c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00770
			 0x4b000000,                                                  // 8388608.0                                   /// 00774
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00778
			 0x3f028f5c,                                                  // 0.51                                        /// 0077c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00780
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00788
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0078c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00794
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00798
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007a4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007a8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007ac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007b0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 007b4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007b8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007c4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c8
			 0xcb000000,                                                  // -8388608.0                                  /// 007cc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007d0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007d4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007d8
			 0xbf028f5c,                                                  // -0.51                                       /// 007dc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 007e0
			 0x80011111,                                                  // -9.7958E-41                                 /// 007e4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 007e8
			 0xff800000,                                                  // -inf                                        /// 007ec
			 0x4b000000,                                                  // 8388608.0                                   /// 007f0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007f4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 007f8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007fc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00800
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00804
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00808
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0080c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00810
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00814
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00818
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0081c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00820
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00824
			 0x0e000003,                                                  // Trailing 1s:                                /// 00828
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0082c
			 0xffc00001,                                                  // -signaling NaN                              /// 00830
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00834
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00838
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0083c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00844
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00848
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00850
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x4b000000,                                                  // 8388608.0                                   /// 00858
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00860
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00864
			 0x00000000,                                                  // zero                                        /// 00868
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0086c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00870
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00874
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00878
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0087c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00880
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00884
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00888
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0088c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00890
			 0x7f800000,                                                  // inf                                         /// 00894
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00898
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008a0
			 0x0e000001,                                                  // Trailing 1s:                                /// 008a4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008a8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008b0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008b4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 008b8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008bc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 008c0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 008c8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x80000000,                                                  // -zero                                       /// 008d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 008d4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008d8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008dc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008e0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008e8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008f0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008f8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008fc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00904
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00908
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0090c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00910
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00914
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00918
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 0091c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00920
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00924
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00928
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00934
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00938
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0093c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00940
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00944
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00948
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0094c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00950
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00954
			 0x0c700000,                                                  // Leading 1s:                                 /// 00958
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00960
			 0x0c700000,                                                  // Leading 1s:                                 /// 00964
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00968
			 0x55555555,                                                  // 4 random values                             /// 0096c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00970
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00974
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00978
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0097c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00984
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00990
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00994
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00998
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0099c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009a0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009a4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 009a8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 009ac
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 009b8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 009c4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 009cc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009d4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009e4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009e8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009ec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009f0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009f4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009f8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009fc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00a00
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a04
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00a08
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a0c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a18
			 0x3f028f5c,                                                  // 0.51                                        /// 00a1c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a20
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a24
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a28
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a2c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a30
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a34
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00a38
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a3c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a40
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00a48
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a50
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00a54
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00a58
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a5c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a60
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a64
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a6c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a74
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a78
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a80
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a84
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a88
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a8c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a90
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a94
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a98
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a9c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00aa0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00aa4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00aac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ab4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ab8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00abc
			 0x7fc00001,                                                  // signaling NaN                               /// 00ac0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ac4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00ac8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00acc
			 0xffc00001,                                                  // -signaling NaN                              /// 00ad0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00adc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ae4
			 0xbf028f5c,                                                  // -0.51                                       /// 00ae8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00aec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00af0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00af4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00af8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00afc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b00
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b04
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00b08
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00b0c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x33333333,                                                  // 4 random values                             /// 00b14
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b1c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b24
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b28
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b2c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b30
			 0xbf028f5c,                                                  // -0.51                                       /// 00b34
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b3c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b40
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b48
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b4c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b50
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00b54
			 0xffc00001,                                                  // -signaling NaN                              /// 00b58
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b5c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b60
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b64
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b68
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b78
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b88
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b90
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00b94
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b98
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ba0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ba4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ba8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bac
			 0x4b000000,                                                  // 8388608.0                                   /// 00bb0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bb8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bc0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00bc4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bc8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00bcc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00bd0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00bd4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00be0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00be4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00be8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00bec
			 0x55555555,                                                  // 4 random values                             /// 00bf0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00bf4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bf8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00c00
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c04
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c08
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c0c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c10
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c14
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c18
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c1c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c20
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c24
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c28
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c2c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c30
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c34
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c38
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c3c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c40
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c44
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c48
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c4c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c50
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c54
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c5c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c60
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00c64
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c68
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c74
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c78
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c7c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c80
			 0x55555555,                                                  // 4 random values                             /// 00c84
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c88
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c90
			 0xbf028f5c,                                                  // -0.51                                       /// 00c94
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c9c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cac
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00cb0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00cb4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00cb8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cbc
			 0x4b000000,                                                  // 8388608.0                                   /// 00cc0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cc8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ccc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cd0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cd4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00cd8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ce0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ce4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ce8
			 0x7fc00001,                                                  // signaling NaN                               /// 00cec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0xbf028f5c,                                                  // -0.51                                       /// 00cf4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cf8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00cfc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d00
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00d04
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0xbf028f5c,                                                  // -0.51                                       /// 00d10
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00d14
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d18
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d1c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d20
			 0x55555555,                                                  // 4 random values                             /// 00d24
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d28
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d2c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d30
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d34
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d38
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d40
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d48
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00d4c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d54
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d5c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00d60
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d64
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00d70
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d74
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d78
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d7c
			 0xff800000,                                                  // -inf                                        /// 00d80
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d88
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d8c
			 0x55555555,                                                  // 4 random values                             /// 00d90
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d94
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d98
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00d9c
			 0xbf028f5c,                                                  // -0.51                                       /// 00da0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00da8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00db0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00db4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00db8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00dbc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00dc0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00dc4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00dd0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00dd4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00dd8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00de0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00de4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00de8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00dec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00df0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00df4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00dfc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e00
			 0xff800000,                                                  // -inf                                        /// 00e04
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e08
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e0c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e14
			 0x0e000007,                                                  // Trailing 1s:                                /// 00e18
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e1c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00e20
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e24
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e2c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00e30
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e3c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e40
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e44
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e48
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00e50
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00e58
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e60
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e64
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00e68
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e6c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e70
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00e74
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e78
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e7c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e80
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e84
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e88
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e8c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e90
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e94
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00e98
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e9c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00ea0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ea4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ea8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00eb0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ebc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ec0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ec4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ec8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ecc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ed0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00edc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ee4
			 0xff800000,                                                  // -inf                                        /// 00ee8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eec
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ef0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ef4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ef8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00efc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f00
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00f08
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f0c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f10
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f14
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00f18
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f1c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f20
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00f24
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f28
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f2c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00f30
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f34
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f40
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f48
			 0xff800000,                                                  // -inf                                        /// 00f4c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f50
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f54
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f58
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f5c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f60
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f64
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f68
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f6c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00f70
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f74
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f7c
			 0x00000000,                                                  // zero                                        /// 00f80
			 0x0c600000,                                                  // Leading 1s:                                 /// 00f84
			 0xcb000000,                                                  // -8388608.0                                  /// 00f88
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f8c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f90
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x0c700000,                                                  // Leading 1s:                                 /// 00f98
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f9c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00fa4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fac
			 0x55555555,                                                  // 4 random values                             /// 00fb0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00fb4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fc0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00fc4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fc8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fcc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fd4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fd8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fdc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fe0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00fe4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fe8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00fec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ff0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00ff8
			 0x3f800000 // 1                                             // SP +ve numbers                              /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00008
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0000c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00010
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00014
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00018
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0001c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00024
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00028
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0002c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00030
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00034
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00038
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0003c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00040
			 0x0c400000,                                                  // Leading 1s:                                 /// 00044
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00048
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0004c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00050
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00054
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00058
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0005c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00060
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00064
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0006c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00070
			 0x0e000003,                                                  // Trailing 1s:                                /// 00074
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00078
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00080
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00088
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0008c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00090
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00098
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 0009c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000a0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000a8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000ac
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 000b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0xcb000000,                                                  // -8388608.0                                  /// 000b8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000c0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000c4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000c8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000cc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000d0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000d4
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x0e000003,                                                  // Trailing 1s:                                /// 000dc
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000e4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000e8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 000ec
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000f8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000fc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00100
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00104
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00108
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0010c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00114
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00118
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0011c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00120
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00124
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0012c
			 0x4b000000,                                                  // 8388608.0                                   /// 00130
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00134
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00138
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0013c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00140
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00144
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00148
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0014c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00150
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00154
			 0x0c400000,                                                  // Leading 1s:                                 /// 00158
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0015c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00160
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00164
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00168
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0016c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00170
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00174
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00178
			 0xAAAAAAAA,                                                  // 4 random values                             /// 0017c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00180
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00184
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00188
			 0x33333333,                                                  // 4 random values                             /// 0018c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00190
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00194
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00198
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0019c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001a4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 001a8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 001ac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 001b0
			 0xcb000000,                                                  // -8388608.0                                  /// 001b4
			 0x0c780000,                                                  // Leading 1s:                                 /// 001b8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001bc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001c0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001c8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 001cc
			 0x00000000,                                                  // zero                                        /// 001d0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001d4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001d8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 001dc
			 0xffc00001,                                                  // -signaling NaN                              /// 001e0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 001e8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ec
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001f0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 001f4
			 0x0c600000,                                                  // Leading 1s:                                 /// 001f8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001fc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00200
			 0x7f800000,                                                  // inf                                         /// 00204
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00208
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0020c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00210
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00214
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00218
			 0x33333333,                                                  // 4 random values                             /// 0021c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00220
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00224
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00228
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0022c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00230
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00234
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0023c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00240
			 0x0c600000,                                                  // Leading 1s:                                 /// 00244
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00248
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0024c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00250
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00254
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00258
			 0xffc00001,                                                  // -signaling NaN                              /// 0025c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00264
			 0x7fc00001,                                                  // signaling NaN                               /// 00268
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0026c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00270
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00274
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00278
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0027c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00280
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00288
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0028c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00290
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00294
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00298
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 002a0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 002a4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 002a8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002ac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002b0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002b4
			 0x0e000003,                                                  // Trailing 1s:                                /// 002b8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 002bc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002c8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002cc
			 0x00011111,                                                  // 9.7958E-41                                  /// 002d0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002dc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002e0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002e4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 002e8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002ec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002f8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00300
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00304
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00308
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0030c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00310
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00318
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0031c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00320
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00324
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00328
			 0x80000000,                                                  // -zero                                       /// 0032c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00334
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00338
			 0x7f800000,                                                  // inf                                         /// 0033c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00340
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00344
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00348
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0034c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00350
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00354
			 0x55555555,                                                  // 4 random values                             /// 00358
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0035c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00360
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00364
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00368
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0036c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00370
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00374
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00378
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0037c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00380
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00384
			 0x33333333,                                                  // 4 random values                             /// 00388
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0038c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00390
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00394
			 0x00011111,                                                  // 9.7958E-41                                  /// 00398
			 0x80000000,                                                  // -zero                                       /// 0039c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003a0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003a8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003ac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003b8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 003bc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003c0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003c4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003c8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003cc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d0
			 0xbf028f5c,                                                  // -0.51                                       /// 003d4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003d8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 003dc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 003e0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003e4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003e8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 003ec
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003f0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003f8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00400
			 0x7fc00001,                                                  // signaling NaN                               /// 00404
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0040c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00410
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00418
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0041c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00420
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00424
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00428
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00430
			 0x33333333,                                                  // 4 random values                             /// 00434
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00438
			 0x00011111,                                                  // 9.7958E-41                                  /// 0043c
			 0xffc00001,                                                  // -signaling NaN                              /// 00440
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00444
			 0x00011111,                                                  // 9.7958E-41                                  /// 00448
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0044c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00450
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00454
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00458
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0045c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00460
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00464
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00468
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0046c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00470
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00474
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00478
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00480
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00484
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00488
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0048c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00490
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00494
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00498
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0049c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004a0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004ac
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004b0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x80011111,                                                  // -9.7958E-41                                 /// 004b8
			 0x80011111,                                                  // -9.7958E-41                                 /// 004bc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c0
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004c4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004cc
			 0x7fc00001,                                                  // signaling NaN                               /// 004d0
			 0x55555555,                                                  // 4 random values                             /// 004d4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 004d8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 004dc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 004e0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004e8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 004ec
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 004f0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004f4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 004f8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00500
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00504
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00508
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0050c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00510
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00514
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00518
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0051c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00520
			 0x0c780000,                                                  // Leading 1s:                                 /// 00524
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00528
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00534
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00538
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0053c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00540
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00544
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00548
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0054c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00550
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00554
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00558
			 0x33333333,                                                  // 4 random values                             /// 0055c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00560
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00564
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00568
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0056c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00570
			 0x0e000001,                                                  // Trailing 1s:                                /// 00574
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00578
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x80000000,                                                  // -zero                                       /// 00580
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00584
			 0x80000000,                                                  // -zero                                       /// 00588
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0058c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00590
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00594
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00598
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0059c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005a0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005a4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 005a8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005b0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 005b4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005b8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005c0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 005c4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005c8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005cc
			 0x4b000000,                                                  // 8388608.0                                   /// 005d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 005d4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005d8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005dc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005e0
			 0xbf028f5c,                                                  // -0.51                                       /// 005e4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 005ec
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 005f0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005f4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005f8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00604
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00608
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0060c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00610
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00614
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00618
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0061c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00620
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00624
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x3f028f5c,                                                  // 0.51                                        /// 00630
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00634
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00638
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0063c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00640
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00644
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00648
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0064c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00650
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00654
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00658
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0065c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00660
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00664
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0066c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00670
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00674
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00678
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0067c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00680
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00684
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00688
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00694
			 0xff800000,                                                  // -inf                                        /// 00698
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0069c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 006a0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006a4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 006a8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ac
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006b0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 006b8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 006bc
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006c4
			 0x80011111,                                                  // -9.7958E-41                                 /// 006c8
			 0xffc00001,                                                  // -signaling NaN                              /// 006cc
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 006d0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006d4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006d8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006e0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 006e4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006e8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 006ec
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006f0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 006f4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00700
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00708
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0070c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00710
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x80011111,                                                  // -9.7958E-41                                 /// 00718
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0071c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00720
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x0e000003,                                                  // Trailing 1s:                                /// 00728
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0072c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00730
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00734
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00738
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0073c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00740
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00744
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00748
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0074c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00750
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 0075c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00764
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00768
			 0x0c700000,                                                  // Leading 1s:                                 /// 0076c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00770
			 0x0c400000,                                                  // Leading 1s:                                 /// 00774
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00778
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0077c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00780
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00784
			 0x0e000003,                                                  // Trailing 1s:                                /// 00788
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0078c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00790
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00794
			 0x00011111,                                                  // 9.7958E-41                                  /// 00798
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0079c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007a0
			 0x3f028f5c,                                                  // 0.51                                        /// 007a4
			 0x33333333,                                                  // 4 random values                             /// 007a8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007ac
			 0x80011111,                                                  // -9.7958E-41                                 /// 007b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0xcb000000,                                                  // -8388608.0                                  /// 007b8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007bc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 007c0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007c4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007c8
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007cc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007d0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007d4
			 0x0e000007,                                                  // Trailing 1s:                                /// 007d8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 007dc
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 007e0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 007e4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007e8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 007f0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007f4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 007f8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00804
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00808
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0080c
			 0x3f028f5c,                                                  // 0.51                                        /// 00810
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00814
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00818
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0081c
			 0xcb000000,                                                  // -8388608.0                                  /// 00820
			 0x00011111,                                                  // 9.7958E-41                                  /// 00824
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0082c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00830
			 0x7f800000,                                                  // inf                                         /// 00834
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0083c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00840
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00844
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0084c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00850
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00854
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0085c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00860
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00868
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0086c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00870
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00874
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00878
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0087c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00880
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00884
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00888
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0088c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00890
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00894
			 0x0c600000,                                                  // Leading 1s:                                 /// 00898
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0089c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008a0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008a8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 008ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008b0
			 0x33333333,                                                  // 4 random values                             /// 008b4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008b8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008bc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 008c0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008d0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 008d8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008dc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008e0
			 0x0e000003,                                                  // Trailing 1s:                                /// 008e4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008e8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008ec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008f0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x33333333,                                                  // 4 random values                             /// 00904
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00908
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0090c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00910
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00914
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00918
			 0x80000000,                                                  // -zero                                       /// 0091c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00920
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00928
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0092c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00930
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00934
			 0xffc00001,                                                  // -signaling NaN                              /// 00938
			 0xbf028f5c,                                                  // -0.51                                       /// 0093c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00940
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00948
			 0x0c700000,                                                  // Leading 1s:                                 /// 0094c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00954
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0095c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00960
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00964
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00968
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0096c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00974
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00978
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0097c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00980
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00984
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00988
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0098c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00990
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00994
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x7fc00001,                                                  // signaling NaN                               /// 0099c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009a4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009a8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 009ac
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 009b0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009bc
			 0x0c780000,                                                  // Leading 1s:                                 /// 009c0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 009c8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 009cc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009d4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 009dc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009e4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009ec
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009f4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 009fc
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a00
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00a04
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00a08
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a0c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a10
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a14
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00a18
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a1c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a20
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a24
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00a2c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a30
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a34
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a38
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a40
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a48
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00a4c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a50
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a58
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a5c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00a60
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a64
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00a68
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a6c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a70
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a74
			 0xff800000,                                                  // -inf                                        /// 00a78
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a7c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a80
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00a84
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a88
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a8c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a90
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a94
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a98
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a9c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00aa0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00aa4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00aa8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00aac
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ab0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ab4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ab8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ac0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00acc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ad0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00ad4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ad8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00adc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ae0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ae4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ae8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00af0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00af4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00afc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b00
			 0x33333333,                                                  // 4 random values                             /// 00b04
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b08
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b0c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b10
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b14
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b18
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00b1c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b24
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b2c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b30
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b34
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b38
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00b3c
			 0xffc00001,                                                  // -signaling NaN                              /// 00b40
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b44
			 0x55555555,                                                  // 4 random values                             /// 00b48
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00b4c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b50
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b54
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b58
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b5c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b60
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00b64
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00b68
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b6c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b70
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b74
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b7c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b80
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b84
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b88
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b8c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b90
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b94
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b98
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b9c
			 0x7fc00001,                                                  // signaling NaN                               /// 00ba0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ba4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ba8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bb0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bbc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bc0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00bc4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bc8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00bcc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bd0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bd4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bd8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bdc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00be0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00be4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00be8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bec
			 0x3f028f5c,                                                  // 0.51                                        /// 00bf0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00bf8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00bfc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c04
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c08
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c18
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c1c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00c20
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00c24
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00c28
			 0x0e000007,                                                  // Trailing 1s:                                /// 00c2c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00c38
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c3c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c44
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c48
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c4c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00c50
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c58
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00c5c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c60
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c64
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00c68
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c6c
			 0x00000000,                                                  // zero                                        /// 00c70
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c78
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c7c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c84
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00c88
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c8c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00c90
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c94
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00c98
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c9c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ca0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00ca4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ca8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00cac
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cb0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cb4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00cb8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00cbc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cc0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cc4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00cc8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ccc
			 0xbf028f5c,                                                  // -0.51                                       /// 00cd0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cd4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cd8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cdc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ce4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ce8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cec
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00cf0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cf4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00cf8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cfc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d00
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d04
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d08
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0xcb000000,                                                  // -8388608.0                                  /// 00d14
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00d1c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d20
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d24
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d28
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d34
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d38
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d3c
			 0x4b000000,                                                  // 8388608.0                                   /// 00d40
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d4c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d50
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d58
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d5c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00d60
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d64
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d68
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d70
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d74
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d78
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d7c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d80
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d84
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d88
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d8c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00d90
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00d94
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d98
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d9c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00da0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00da4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00dac
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00db0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00db4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00dbc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00dc0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00dc4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00dc8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00dcc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00dd4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00dd8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ddc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00de0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00de4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00de8
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0xcb000000,                                                  // -8388608.0                                  /// 00df0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00df4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00df8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00dfc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00e00
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e04
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e08
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e0c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e14
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00e18
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e1c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e24
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e28
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e30
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e38
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e40
			 0xff800000,                                                  // -inf                                        /// 00e44
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e48
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e50
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e54
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00e58
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e5c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e60
			 0x7f800000,                                                  // inf                                         /// 00e64
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e68
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e6c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e70
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e74
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e78
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e7c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00e84
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e88
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e8c
			 0xcb000000,                                                  // -8388608.0                                  /// 00e90
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e94
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e9c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea0
			 0x7f800000,                                                  // inf                                         /// 00ea4
			 0xcb000000,                                                  // -8388608.0                                  /// 00ea8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00eac
			 0x80011111,                                                  // -9.7958E-41                                 /// 00eb0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00eb8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ebc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ec0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ec4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ec8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00ed0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00ed4
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00ed8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00edc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ee0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ee4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ee8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eec
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00ef0
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ef4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00ef8
			 0x33333333,                                                  // 4 random values                             /// 00efc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00f00
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f04
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f08
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00f0c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f10
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f18
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00f1c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00f20
			 0x4b000000,                                                  // 8388608.0                                   /// 00f24
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f28
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f2c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f30
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f34
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f44
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00f48
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f4c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f54
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f58
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f5c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f60
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f64
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f68
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f6c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f70
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00f78
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f7c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f84
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f88
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f8c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f90
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f94
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f98
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f9c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00fa4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fa8
			 0x7fc00001,                                                  // signaling NaN                               /// 00fac
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fb0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fbc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00fc0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fc4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fc8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fcc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fd0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00fd4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fdc
			 0xbf028f5c,                                                  // -0.51                                       /// 00fe0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00fe8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00fec
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00ff0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ff4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x00000100                                                  // SP - 1 bit alone set in mantissa +ve        /// last
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
			 0x00000610,
			 0x000001a4,
			 0x000006fc,
			 0x00000700,
			 0x000006e4,
			 0x0000013c,
			 0x000000dc,
			 0x000003b0,

			 /// vpu register f2
			 0x41500000,
			 0x41400000,
			 0x42000000,
			 0x40e00000,
			 0x41d80000,
			 0x40a00000,
			 0x41500000,
			 0x41600000,

			 /// vpu register f3
			 0x40000000,
			 0x41200000,
			 0x41200000,
			 0x41d80000,
			 0x40000000,
			 0x41900000,
			 0x40c00000,
			 0x41300000,

			 /// vpu register f4
			 0x40c00000,
			 0x41000000,
			 0x41300000,
			 0x41100000,
			 0x40a00000,
			 0x41100000,
			 0x41c80000,
			 0x41a00000,

			 /// vpu register f5
			 0x40800000,
			 0x41f80000,
			 0x41300000,
			 0x41200000,
			 0x41e00000,
			 0x41500000,
			 0x40c00000,
			 0x41f80000,

			 /// vpu register f6
			 0x41a00000,
			 0x41e00000,
			 0x41980000,
			 0x42000000,
			 0x41980000,
			 0x40a00000,
			 0x41200000,
			 0x42000000,

			 /// vpu register f7
			 0x41d80000,
			 0x40e00000,
			 0x41880000,
			 0x41b00000,
			 0x40800000,
			 0x41980000,
			 0x40400000,
			 0x40400000,

			 /// vpu register f8
			 0x40a00000,
			 0x41300000,
			 0x40000000,
			 0x40c00000,
			 0x41400000,
			 0x41700000,
			 0x41500000,
			 0x40c00000,

			 /// vpu register f9
			 0x41e80000,
			 0x41800000,
			 0x42000000,
			 0x41400000,
			 0x41900000,
			 0x41500000,
			 0x40a00000,
			 0x41d80000,

			 /// vpu register f10
			 0x3f800000,
			 0x41800000,
			 0x40800000,
			 0x40e00000,
			 0x42000000,
			 0x41880000,
			 0x40800000,
			 0x40a00000,

			 /// vpu register f11
			 0x41c00000,
			 0x41e00000,
			 0x41000000,
			 0x41c80000,
			 0x41400000,
			 0x41f00000,
			 0x41a80000,
			 0x41500000,

			 /// vpu register f12
			 0x41b80000,
			 0x41c80000,
			 0x41a00000,
			 0x41a00000,
			 0x41f80000,
			 0x41900000,
			 0x41d80000,
			 0x41f00000,

			 /// vpu register f13
			 0x40800000,
			 0x41e80000,
			 0x41500000,
			 0x41c80000,
			 0x41000000,
			 0x40000000,
			 0x41000000,
			 0x40e00000,

			 /// vpu register f14
			 0x41000000,
			 0x41b80000,
			 0x42000000,
			 0x41900000,
			 0x41a80000,
			 0x41500000,
			 0x40c00000,
			 0x41900000,

			 /// vpu register f15
			 0x41600000,
			 0x41900000,
			 0x40400000,
			 0x41a00000,
			 0x41880000,
			 0x40a00000,
			 0x41500000,
			 0x41e80000,

			 /// vpu register f16
			 0x40a00000,
			 0x41600000,
			 0x41800000,
			 0x41700000,
			 0x41f80000,
			 0x41a00000,
			 0x41c80000,
			 0x41900000,

			 /// vpu register f17
			 0x41c80000,
			 0x41f00000,
			 0x42000000,
			 0x41980000,
			 0x3f800000,
			 0x41d00000,
			 0x40000000,
			 0x41700000,

			 /// vpu register f18
			 0x41e80000,
			 0x40c00000,
			 0x40c00000,
			 0x40a00000,
			 0x42000000,
			 0x40c00000,
			 0x41880000,
			 0x40000000,

			 /// vpu register f19
			 0x40400000,
			 0x41880000,
			 0x41200000,
			 0x41c00000,
			 0x41c80000,
			 0x41a80000,
			 0x41d00000,
			 0x41200000,

			 /// vpu register f20
			 0x41400000,
			 0x41300000,
			 0x41800000,
			 0x41200000,
			 0x42000000,
			 0x41800000,
			 0x41d00000,
			 0x41300000,

			 /// vpu register f21
			 0x40c00000,
			 0x41980000,
			 0x41d00000,
			 0x40000000,
			 0x41b80000,
			 0x41980000,
			 0x41400000,
			 0x41b80000,

			 /// vpu register f22
			 0x40800000,
			 0x41800000,
			 0x41f80000,
			 0x41600000,
			 0x41b80000,
			 0x41400000,
			 0x41700000,
			 0x40800000,

			 /// vpu register f23
			 0x41900000,
			 0x41700000,
			 0x41800000,
			 0x41a00000,
			 0x41300000,
			 0x41880000,
			 0x41980000,
			 0x41800000,

			 /// vpu register f24
			 0x40000000,
			 0x41d00000,
			 0x41900000,
			 0x41900000,
			 0x41800000,
			 0x41d00000,
			 0x42000000,
			 0x41500000,

			 /// vpu register f25
			 0x41b80000,
			 0x40800000,
			 0x41b00000,
			 0x41c00000,
			 0x40800000,
			 0x41980000,
			 0x41100000,
			 0x41c00000,

			 /// vpu register f26
			 0x40800000,
			 0x41600000,
			 0x41880000,
			 0x41d80000,
			 0x41100000,
			 0x40c00000,
			 0x40c00000,
			 0x41000000,

			 /// vpu register f27
			 0x41200000,
			 0x41d00000,
			 0x41b00000,
			 0x40000000,
			 0x41e00000,
			 0x41c80000,
			 0x41880000,
			 0x41a00000,

			 /// vpu register f28
			 0x41f80000,
			 0x41500000,
			 0x41600000,
			 0x41700000,
			 0x41880000,
			 0x40a00000,
			 0x41200000,
			 0x41f00000,

			 /// vpu register f29
			 0x41900000,
			 0x40400000,
			 0x41b80000,
			 0x41100000,
			 0x3f800000,
			 0x41f80000,
			 0x41880000,
			 0x41500000,

			 /// vpu register f30
			 0x41b00000,
			 0x41b80000,
			 0x41100000,
			 0x41c80000,
			 0x41880000,
			 0x40a00000,
			 0x41b00000,
			 0x41d00000,

			 /// vpu register f31
			 0x41500000,
			 0x41700000,
			 0x41c80000,
			 0x41300000,
			 0x41900000,
			 0x41b80000,
			 0x3f800000,
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
		"maskand m6, m1, m4\n"                                ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m4, m1, m1\n"                                ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m2, m0, m3\n"                                ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m5, m6, m2\n"                                ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m3, m3, m3\n"                                ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m4, m5, m5\n"                                ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m7, m7, m6\n"                                ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m4, m6, m6\n"                                ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m4, m3, m1\n"                                ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m0, m7, m1\n"                                ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m4, m3, m0\n"                                ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m0, m5, m0\n"                                ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m6, m0, m1\n"                                ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m2, m0, m4\n"                                ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m5, m2, m1\n"                                ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m2, m7, m2\n"                                ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m5, m2, m0\n"                                ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m0, m2, m7\n"                                ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m7, m5, m3\n"                                ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m6, m0, m4\n"                                ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m1, m4, m5\n"                                ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m0, m7, m1\n"                                ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m2, m6, m7\n"                                ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m5, m3, m4\n"                                ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m0, m1, m0\n"                                ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m5, m5, m5\n"                                ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m6, m0, m5\n"                                ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m0, m4, m3\n"                                ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m3, m6, m7\n"                                ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m0, m5, m7\n"                                ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m5, m0, m2\n"                                ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m6, m2, m7\n"                                ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m0, m4, m7\n"                                ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m2, m3, m4\n"                                ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m3, m4, m1\n"                                ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m2, m2, m6\n"                                ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m1, m1, m4\n"                                ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m1, m5, m6\n"                                ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m6, m4, m1\n"                                ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m5, m1, m5\n"                                ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m5, m4, m7\n"                                ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m4, m1, m6\n"                                ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m5, m0, m5\n"                                ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m2, m4, m3\n"                                ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m1, m7, m3\n"                                ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m7, m0, m6\n"                                ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m1, m2, m7\n"                                ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m1, m2, m0\n"                                ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m5, m2, m7\n"                                ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m3, m2, m5\n"                                ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m6, m2, m4\n"                                ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m1, m0, m4\n"                                ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m2, m5, m3\n"                                ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m2, m7, m5\n"                                ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m6, m3, m4\n"                                ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m7, m3, m7\n"                                ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m4, m2, m2\n"                                ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m0, m5, m1\n"                                ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m4, m5, m0\n"                                ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m0, m2, m1\n"                                ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m7, m6, m6\n"                                ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m6, m0, m5\n"                                ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m4, m1, m4\n"                                ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m7, m2, m6\n"                                ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m3, m2, m0\n"                                ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m1, m0, m4\n"                                ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m6, m4, m7\n"                                ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m2, m2, m6\n"                                ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m0, m3, m6\n"                                ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m2, m5, m3\n"                                ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m0, m6, m7\n"                                ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m4, m1, m5\n"                                ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m0, m4, m1\n"                                ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m2, m7, m4\n"                                ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m6, m4, m6\n"                                ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m5, m0, m7\n"                                ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m2, m4, m1\n"                                ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m4, m3, m3\n"                                ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m7, m4, m7\n"                                ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m4, m2, m1\n"                                ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m5, m7, m7\n"                                ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m1, m5, m3\n"                                ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m3, m6, m0\n"                                ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m5, m3, m3\n"                                ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m5, m3, m3\n"                                ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m3, m7, m7\n"                                ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m0, m1, m0\n"                                ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m0, m4, m0\n"                                ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m2, m4, m7\n"                                ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m1, m0, m5\n"                                ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m4, m1, m3\n"                                ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m7, m2, m6\n"                                ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m0, m4, m5\n"                                ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m6, m4, m1\n"                                ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m1, m0, m2\n"                                ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m4, m0, m4\n"                                ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m2, m3, m7\n"                                ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m7, m6, m2\n"                                ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"maskand m4, m5, m5\n"                                ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
