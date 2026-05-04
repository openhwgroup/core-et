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
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00000
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00004
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00008
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0000c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00018
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0001c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00020
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00024
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00028
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 0002c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00030
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00038
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0003c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00040
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00044
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0004c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00050
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00054
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00058
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00060
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00064
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00068
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0006c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00070
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00078
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00080
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00084
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00088
			 0xffc00001,                                                  // -signaling NaN                              /// 0008c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00090
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00098
			 0x80000000,                                                  // -zero                                       /// 0009c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 000a4
			 0xffc00001,                                                  // -signaling NaN                              /// 000a8
			 0xffc00001,                                                  // -signaling NaN                              /// 000ac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000b4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000b8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000bc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 000c0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000c4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 000c8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 000cc
			 0xcb000000,                                                  // -8388608.0                                  /// 000d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 000d8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 000dc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000e4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 000ec
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 000f8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000fc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00104
			 0x0e000007,                                                  // Trailing 1s:                                /// 00108
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0010c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00110
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00114
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00118
			 0xff800000,                                                  // -inf                                        /// 0011c
			 0x4b000000,                                                  // 8388608.0                                   /// 00120
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00124
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0012c
			 0x55555555,                                                  // 4 random values                             /// 00130
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00138
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00144
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00148
			 0x80000000,                                                  // -zero                                       /// 0014c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00150
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00154
			 0x7f800000,                                                  // inf                                         /// 00158
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0015c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00160
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00164
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00168
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0016c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00170
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00174
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00178
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0017c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00180
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00184
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00188
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0018c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00190
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00194
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00198
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0019c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001a4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001a8
			 0x0e000007,                                                  // Trailing 1s:                                /// 001ac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001b4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 001bc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001c0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001c4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001c8
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 001cc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001d8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001dc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 001e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 001e8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 001ec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001f0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 001f4
			 0x0c400000,                                                  // Leading 1s:                                 /// 001f8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001fc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00200
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00204
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00210
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00214
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00218
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x80000000,                                                  // -zero                                       /// 00220
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00224
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00228
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0022c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00230
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00234
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00238
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0023c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00240
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00244
			 0x80011111,                                                  // -9.7958E-41                                 /// 00248
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 0024c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00250
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00254
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00258
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0025c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00260
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00264
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00268
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0026c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00270
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00278
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0027c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00280
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00284
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00288
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0028c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00290
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00294
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00298
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002a0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 002a4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002ac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002b0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b4
			 0xcb000000,                                                  // -8388608.0                                  /// 002b8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002bc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002c0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002c4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002c8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002cc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002d0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 002d4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002dc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 002e0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 002e4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002e8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002ec
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 002f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x7fc00001,                                                  // signaling NaN                               /// 002f8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00304
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00308
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0030c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00310
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00314
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00318
			 0x0e000003,                                                  // Trailing 1s:                                /// 0031c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00320
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00324
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0032c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00330
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00334
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00338
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0033c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00340
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00348
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00350
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00354
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00358
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0035c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00360
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00364
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00368
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00370
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00374
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00378
			 0x0c600000,                                                  // Leading 1s:                                 /// 0037c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00380
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00384
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00388
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0038c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00390
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00394
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00398
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0039c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 003a4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 003a8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003ac
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003b0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x4b000000,                                                  // 8388608.0                                   /// 003b8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003bc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 003c4
			 0x3f028f5c,                                                  // 0.51                                        /// 003c8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003cc
			 0xff800000,                                                  // -inf                                        /// 003d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003d4
			 0x00011111,                                                  // 9.7958E-41                                  /// 003d8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 003dc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003e0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003e4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 003f0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 003f4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003f8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 003fc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00400
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00404
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00408
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0040c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00410
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00414
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00418
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0041c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00420
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00424
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00428
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0042c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00430
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00434
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00438
			 0xbf028f5c,                                                  // -0.51                                       /// 0043c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00440
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00444
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00448
			 0x7f800000,                                                  // inf                                         /// 0044c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00450
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00458
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0045c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00460
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00464
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0xff800000,                                                  // -inf                                        /// 0046c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00474
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00478
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0047c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00488
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0048c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00494
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00498
			 0x4b000000,                                                  // 8388608.0                                   /// 0049c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 004a0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 004a4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004a8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004ac
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004b4
			 0x80000000,                                                  // -zero                                       /// 004b8
			 0x0c780000,                                                  // Leading 1s:                                 /// 004bc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 004c4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004c8
			 0x0e000007,                                                  // Trailing 1s:                                /// 004cc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 004d0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004d4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004dc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004e0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004e8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004ec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004f4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00500
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00504
			 0xff800000,                                                  // -inf                                        /// 00508
			 0x80000000,                                                  // -zero                                       /// 0050c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00510
			 0x7f800000,                                                  // inf                                         /// 00514
			 0xcb000000,                                                  // -8388608.0                                  /// 00518
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0051c
			 0x7f800000,                                                  // inf                                         /// 00520
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00524
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0052c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00530
			 0x00000000,                                                  // zero                                        /// 00534
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00538
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0053c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00540
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00544
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00548
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0054c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00550
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00554
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00558
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0055c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00560
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00568
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0056c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00570
			 0x0e000001,                                                  // Trailing 1s:                                /// 00574
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00578
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0057c
			 0x80000000,                                                  // -zero                                       /// 00580
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00584
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00588
			 0x80000000,                                                  // -zero                                       /// 0058c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00590
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00594
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00598
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0059c
			 0x0e000003,                                                  // Trailing 1s:                                /// 005a0
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 005a4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005a8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 005ac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 005c0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 005c4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005c8
			 0xff800000,                                                  // -inf                                        /// 005cc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005d8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005e0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 005e8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 005f0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005f4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005f8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005fc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00600
			 0x0e000001,                                                  // Trailing 1s:                                /// 00604
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0060c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00610
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00614
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00618
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00620
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00624
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00628
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0062c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00630
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00634
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0063c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00640
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00644
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00648
			 0x0c700000,                                                  // Leading 1s:                                 /// 0064c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00650
			 0x7f800000,                                                  // inf                                         /// 00654
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00658
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0065c
			 0x4b000000,                                                  // 8388608.0                                   /// 00660
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00664
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00668
			 0x0c400000,                                                  // Leading 1s:                                 /// 0066c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00670
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00674
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00678
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0067c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00680
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00684
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00688
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0068c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00690
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00694
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00698
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0069c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006a0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 006ac
			 0x0e000001,                                                  // Trailing 1s:                                /// 006b0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006b8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 006c0
			 0x0c600000,                                                  // Leading 1s:                                 /// 006c4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006d0
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006d8
			 0x0e000001,                                                  // Trailing 1s:                                /// 006dc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 006e0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006ec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 006f0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 006f4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006f8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 006fc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00700
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00708
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 0070c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00714
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00718
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0071c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00720
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00728
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0072c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00730
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00734
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00738
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0073c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00740
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00744
			 0x0c600000,                                                  // Leading 1s:                                 /// 00748
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0074c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00750
			 0x80000000,                                                  // -zero                                       /// 00754
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00758
			 0x0c700000,                                                  // Leading 1s:                                 /// 0075c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00760
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00764
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00768
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 0076c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00770
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00778
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0077c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00780
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00784
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0xcb000000,                                                  // -8388608.0                                  /// 0078c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00790
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0079c
			 0x4b000000,                                                  // 8388608.0                                   /// 007a0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007a4
			 0xCCCCCCCC,                                                  // 4 random values                             /// 007a8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007ac
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007b8
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x4b000000,                                                  // 8388608.0                                   /// 007c0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c4
			 0x00000000,                                                  // zero                                        /// 007c8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 007cc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 007d4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007d8
			 0x0c780000,                                                  // Leading 1s:                                 /// 007dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007e0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007e4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007e8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 007f0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 007f4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007f8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 007fc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00800
			 0xffc00001,                                                  // -signaling NaN                              /// 00804
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00808
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0080c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00810
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00814
			 0x0c780000,                                                  // Leading 1s:                                 /// 00818
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0081c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00820
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00824
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00828
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0082c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00830
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00834
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00838
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00840
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00844
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00848
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00850
			 0x0c780000,                                                  // Leading 1s:                                 /// 00854
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00858
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0085c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00860
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00864
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00868
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0086c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00870
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00874
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x0e000007,                                                  // Trailing 1s:                                /// 0087c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00880
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00884
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00888
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0088c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00894
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00898
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0089c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008ac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008b0
			 0x0c700000,                                                  // Leading 1s:                                 /// 008b4
			 0x7f800000,                                                  // inf                                         /// 008b8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008bc
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 008c0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008c8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008d0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d4
			 0xbf028f5c,                                                  // -0.51                                       /// 008d8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 008dc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008ec
			 0x33333333,                                                  // 4 random values                             /// 008f0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x00011111,                                                  // 9.7958E-41                                  /// 008f8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008fc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00900
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00904
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00910
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00914
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0091c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00920
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00924
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00928
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0092c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00930
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00934
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00938
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0093c
			 0x3f028f5c,                                                  // 0.51                                        /// 00940
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00944
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00948
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0094c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00950
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x00011111,                                                  // 9.7958E-41                                  /// 0095c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00960
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00968
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0096c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00970
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00974
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00978
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0097c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00984
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00988
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0098c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00990
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00994
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0099c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009a0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 009a4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009a8
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 009ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009b0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 009b4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009b8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x80000000,                                                  // -zero                                       /// 009c4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c8
			 0x7fc00001,                                                  // signaling NaN                               /// 009cc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 009d0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009d4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009d8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009dc
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009e0
			 0x80000000,                                                  // -zero                                       /// 009e4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 009e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 009f0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009f8
			 0x00011111,                                                  // 9.7958E-41                                  /// 009fc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a00
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a04
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00a08
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a10
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a14
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a18
			 0x3f028f5c,                                                  // 0.51                                        /// 00a1c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00a20
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a24
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a28
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a2c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a30
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a34
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a3c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00a40
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a48
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a4c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a50
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a54
			 0x7f800000,                                                  // inf                                         /// 00a58
			 0x55555555,                                                  // 4 random values                             /// 00a5c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a60
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00a64
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a68
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a6c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x33333333,                                                  // 4 random values                             /// 00a78
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x00000000,                                                  // zero                                        /// 00a80
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a84
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a88
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a8c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00a90
			 0x55555555,                                                  // 4 random values                             /// 00a94
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a98
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a9c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00aa0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00aa4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00aa8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00aac
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ab4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ab8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00abc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac0
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ac4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00ac8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0xbf028f5c,                                                  // -0.51                                       /// 00ad0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ad4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ad8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00adc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ae0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ae4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00ae8
			 0x3f028f5c,                                                  // 0.51                                        /// 00aec
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00af0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00af8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00afc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00b00
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b04
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b0c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00b14
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b1c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b20
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b24
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b28
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b2c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b30
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b34
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b38
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b3c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00b40
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b44
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b48
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b4c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b50
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b54
			 0x7fc00001,                                                  // signaling NaN                               /// 00b58
			 0x00000000,                                                  // zero                                        /// 00b5c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00b60
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b64
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b68
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b6c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b70
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b74
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b7c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b80
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b84
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b88
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b8c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b90
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b94
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b98
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b9c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ba0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ba4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ba8
			 0xffc00001,                                                  // -signaling NaN                              /// 00bac
			 0xbf028f5c,                                                  // -0.51                                       /// 00bb0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00bb4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bbc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00bc4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bc8
			 0xff800000,                                                  // -inf                                        /// 00bcc
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00bd0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bd4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bd8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bdc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00be0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00be8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bec
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bf0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00bf4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c04
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c0c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00c10
			 0xff800000,                                                  // -inf                                        /// 00c14
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00c18
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00c1c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c20
			 0x80000000,                                                  // -zero                                       /// 00c24
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c28
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c2c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c30
			 0x7f800000,                                                  // inf                                         /// 00c34
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c38
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c40
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c44
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c48
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c4c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c54
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00c58
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c5c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00c60
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c64
			 0x55555555,                                                  // 4 random values                             /// 00c68
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c6c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00c70
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c74
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c78
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c7c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c80
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c84
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c88
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c8c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c90
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00c94
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00c98
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00c9c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ca0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ca4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00cac
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cb0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00cb4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00cb8
			 0xffc00001,                                                  // -signaling NaN                              /// 00cbc
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cc0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00cc4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00cc8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x7fc00001,                                                  // signaling NaN                               /// 00cd0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00cd8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cdc
			 0xffc00001,                                                  // -signaling NaN                              /// 00ce0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ce4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ce8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cec
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cf0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cf4
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cf8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cfc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d00
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00d04
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d08
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d0c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00d10
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00d18
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d1c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d24
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d28
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d34
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d38
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d3c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00d40
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d44
			 0x0c700000,                                                  // Leading 1s:                                 /// 00d48
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d4c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00d54
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d58
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d5c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d60
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00d64
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d68
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00d70
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d74
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d78
			 0x55555555,                                                  // 4 random values                             /// 00d7c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d80
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d84
			 0x33333333,                                                  // 4 random values                             /// 00d88
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d8c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d90
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d94
			 0xbf028f5c,                                                  // -0.51                                       /// 00d98
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00d9c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00da0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00da4
			 0x7fc00001,                                                  // signaling NaN                               /// 00da8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00dac
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00db0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00dbc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dc4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dd0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00dd8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ddc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00de4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00de8
			 0x7fc00001,                                                  // signaling NaN                               /// 00dec
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00df0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00df8
			 0xffc00001,                                                  // -signaling NaN                              /// 00dfc
			 0x80000000,                                                  // -zero                                       /// 00e00
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e04
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e08
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e0c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00e10
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e14
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e18
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e1c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x0e000003,                                                  // Trailing 1s:                                /// 00e24
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e2c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e30
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00e34
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e38
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e3c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00e40
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e44
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e48
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e4c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00e50
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e58
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00e5c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e60
			 0x7f800000,                                                  // inf                                         /// 00e64
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e68
			 0xbf028f5c,                                                  // -0.51                                       /// 00e6c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00e70
			 0xcb000000,                                                  // -8388608.0                                  /// 00e74
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e7c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e80
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e84
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00e88
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e8c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00e90
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00e94
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e98
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e9c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ea0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00ea8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eac
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00eb0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00eb4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00eb8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ebc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ec4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ec8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ecc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ed4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ed8
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00edc
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00ee0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ee4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ee8
			 0x55555555,                                                  // 4 random values                             /// 00eec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ef8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00efc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f00
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f04
			 0xcb000000,                                                  // -8388608.0                                  /// 00f08
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f0c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f10
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f14
			 0x55555555,                                                  // 4 random values                             /// 00f18
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f1c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f20
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f24
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f28
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00f2c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f30
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f34
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f38
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f3c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f40
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f44
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f48
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f4c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f50
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f54
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f58
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f60
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f64
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f68
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f6c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00f70
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f74
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f78
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f7c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f80
			 0x0e000007,                                                  // Trailing 1s:                                /// 00f84
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f88
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f8c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f90
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f94
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f98
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00f9c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00fa0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00fa4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fa8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fb4
			 0x33333333,                                                  // 4 random values                             /// 00fb8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00fbc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00fc0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fc4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00fc8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fcc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fd0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fd4
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fd8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fdc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00fe0
			 0x3f028f5c,                                                  // 0.51                                        /// 00fe4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fe8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00ff0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ff4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00ff8
			 0x0e1fffff                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0xeb36caa5, /// 0x0
			 0xb875fa64, /// 0x4
			 0x4c686084, /// 0x8
			 0x52d1676a, /// 0xc
			 0x2cf5083e, /// 0x10
			 0xa43aebca, /// 0x14
			 0x13bd4e77, /// 0x18
			 0x9d0d553c, /// 0x1c
			 0x2961d59e, /// 0x20
			 0x829bdc78, /// 0x24
			 0x8d9cdec8, /// 0x28
			 0xa8c430dc, /// 0x2c
			 0xfbf84deb, /// 0x30
			 0x0dcc36ff, /// 0x34
			 0x6010764e, /// 0x38
			 0x646fdaa8, /// 0x3c
			 0x2dbf734c, /// 0x40
			 0x0592e269, /// 0x44
			 0x6a2b497d, /// 0x48
			 0x3d0c614b, /// 0x4c
			 0x94e475b6, /// 0x50
			 0x517d24bc, /// 0x54
			 0x4d096f68, /// 0x58
			 0x416f1a61, /// 0x5c
			 0xd7648410, /// 0x60
			 0x25ba12aa, /// 0x64
			 0xb15786e9, /// 0x68
			 0x18a39a70, /// 0x6c
			 0xa7282cfe, /// 0x70
			 0xe3f83f55, /// 0x74
			 0xb837b887, /// 0x78
			 0x5eae412a, /// 0x7c
			 0xfb6aeec6, /// 0x80
			 0x52d7102e, /// 0x84
			 0xead2ae40, /// 0x88
			 0x3a44bbdc, /// 0x8c
			 0x1b7c0c86, /// 0x90
			 0x47d95fc8, /// 0x94
			 0x5c2cfa82, /// 0x98
			 0xc3680ef7, /// 0x9c
			 0xa95b1189, /// 0xa0
			 0x53e34f8f, /// 0xa4
			 0x7543efe6, /// 0xa8
			 0x32913e74, /// 0xac
			 0x0a4dbfa8, /// 0xb0
			 0xa7fc7995, /// 0xb4
			 0x6a6674dc, /// 0xb8
			 0xdc90e23c, /// 0xbc
			 0x47065ac8, /// 0xc0
			 0x07e56126, /// 0xc4
			 0x254b0475, /// 0xc8
			 0xc8b81559, /// 0xcc
			 0xec98e8f7, /// 0xd0
			 0xa9aa984a, /// 0xd4
			 0x0c3237a6, /// 0xd8
			 0x04b94972, /// 0xdc
			 0x596e2fe2, /// 0xe0
			 0x38823378, /// 0xe4
			 0xaca3e31a, /// 0xe8
			 0x5bbee4aa, /// 0xec
			 0x0bb9596e, /// 0xf0
			 0x44f1a6e3, /// 0xf4
			 0x29b15dc8, /// 0xf8
			 0x15ec5a65, /// 0xfc
			 0x5c809d1c, /// 0x100
			 0x7732fc50, /// 0x104
			 0x4d034d20, /// 0x108
			 0x8f6223e2, /// 0x10c
			 0x4b1eddaf, /// 0x110
			 0x4cff6ed2, /// 0x114
			 0x954c8cc7, /// 0x118
			 0xbeb97b5d, /// 0x11c
			 0x37c4e1ed, /// 0x120
			 0xe06b906b, /// 0x124
			 0x2fb89931, /// 0x128
			 0xf40e8b77, /// 0x12c
			 0x8e4ee90b, /// 0x130
			 0x898c73e8, /// 0x134
			 0x2639badc, /// 0x138
			 0x147f82cf, /// 0x13c
			 0xac7aad8e, /// 0x140
			 0x59714f40, /// 0x144
			 0xc6180247, /// 0x148
			 0xa96a60a7, /// 0x14c
			 0x6c83f2bd, /// 0x150
			 0x56d10f5a, /// 0x154
			 0xdb0708f2, /// 0x158
			 0xf3fe8fd7, /// 0x15c
			 0xc291b0e4, /// 0x160
			 0x3924b5c7, /// 0x164
			 0x4c49c27e, /// 0x168
			 0xfedcbf55, /// 0x16c
			 0x46647658, /// 0x170
			 0x587b86af, /// 0x174
			 0xb2f9e963, /// 0x178
			 0xeab89e36, /// 0x17c
			 0x530b56a6, /// 0x180
			 0xbf2b47ea, /// 0x184
			 0xb6568eff, /// 0x188
			 0x15769244, /// 0x18c
			 0x63a93d14, /// 0x190
			 0xbe22ed85, /// 0x194
			 0x24056118, /// 0x198
			 0x6fc34816, /// 0x19c
			 0x8b09d266, /// 0x1a0
			 0x4ae45bb2, /// 0x1a4
			 0xf40a6e5b, /// 0x1a8
			 0x7dbeca72, /// 0x1ac
			 0x02d8c4b2, /// 0x1b0
			 0xc74a03dd, /// 0x1b4
			 0x7ee465c2, /// 0x1b8
			 0x44c329ed, /// 0x1bc
			 0x22768534, /// 0x1c0
			 0x92bfd947, /// 0x1c4
			 0xf228fc2f, /// 0x1c8
			 0x0f369757, /// 0x1cc
			 0x27c7a0af, /// 0x1d0
			 0x2c42a144, /// 0x1d4
			 0x4a2e7030, /// 0x1d8
			 0xed9d2d62, /// 0x1dc
			 0x082eea9e, /// 0x1e0
			 0xd3335cf6, /// 0x1e4
			 0xa67380d0, /// 0x1e8
			 0x0169acde, /// 0x1ec
			 0x0acbe9cb, /// 0x1f0
			 0x7c40a0bf, /// 0x1f4
			 0xd3107867, /// 0x1f8
			 0x9b5f4137, /// 0x1fc
			 0x47dc56bd, /// 0x200
			 0x78fb0194, /// 0x204
			 0xcd8cdbe1, /// 0x208
			 0x70fc3f65, /// 0x20c
			 0x26a11fe2, /// 0x210
			 0x47e03627, /// 0x214
			 0x69ef87c3, /// 0x218
			 0xb2359e4b, /// 0x21c
			 0x385edc27, /// 0x220
			 0x89856d19, /// 0x224
			 0xbfdaeca6, /// 0x228
			 0x6175c8fd, /// 0x22c
			 0xbb46aa81, /// 0x230
			 0x096155f6, /// 0x234
			 0x511605c1, /// 0x238
			 0xac428a2e, /// 0x23c
			 0xe9e25d38, /// 0x240
			 0x598e8b2e, /// 0x244
			 0xb26dba50, /// 0x248
			 0x913e309f, /// 0x24c
			 0xcad8c357, /// 0x250
			 0x8418d71d, /// 0x254
			 0xe8c7eb19, /// 0x258
			 0x837e3362, /// 0x25c
			 0x016724c1, /// 0x260
			 0x06f58d4f, /// 0x264
			 0x11f8a29d, /// 0x268
			 0x574a080a, /// 0x26c
			 0x828df3f6, /// 0x270
			 0x003f797c, /// 0x274
			 0xf35a6ca4, /// 0x278
			 0xa37f940c, /// 0x27c
			 0x5df46812, /// 0x280
			 0x624f16b2, /// 0x284
			 0x064e2be0, /// 0x288
			 0x5400ef6e, /// 0x28c
			 0x11eb1043, /// 0x290
			 0x07ee3a49, /// 0x294
			 0xb409a0ed, /// 0x298
			 0xfd5ee466, /// 0x29c
			 0xe8375f0b, /// 0x2a0
			 0x37400aa4, /// 0x2a4
			 0x239875eb, /// 0x2a8
			 0x9e37ec47, /// 0x2ac
			 0xd48df192, /// 0x2b0
			 0x9895d9d1, /// 0x2b4
			 0xc3a3cd86, /// 0x2b8
			 0xec331104, /// 0x2bc
			 0xb51cea67, /// 0x2c0
			 0x9b130dd8, /// 0x2c4
			 0xcf9a8edd, /// 0x2c8
			 0x00d4caff, /// 0x2cc
			 0x34197d1c, /// 0x2d0
			 0x09e0351f, /// 0x2d4
			 0x31f8aad1, /// 0x2d8
			 0x9f7f8ecd, /// 0x2dc
			 0x7db203f1, /// 0x2e0
			 0x95b4a3d1, /// 0x2e4
			 0x009a098b, /// 0x2e8
			 0x13a5b969, /// 0x2ec
			 0x9a291a2c, /// 0x2f0
			 0x9b78a138, /// 0x2f4
			 0xbc3932a4, /// 0x2f8
			 0xe37d2ea9, /// 0x2fc
			 0xb24b06ae, /// 0x300
			 0x468e64e6, /// 0x304
			 0xa512a25a, /// 0x308
			 0x7170d34e, /// 0x30c
			 0x5e8a19ee, /// 0x310
			 0x3d8bfdb5, /// 0x314
			 0xd4f9e455, /// 0x318
			 0x5c5a8521, /// 0x31c
			 0xa105a451, /// 0x320
			 0x8ee7c1eb, /// 0x324
			 0x88b8f577, /// 0x328
			 0x7c2b79ce, /// 0x32c
			 0xd7cb7bdc, /// 0x330
			 0xf0c48be0, /// 0x334
			 0xb82d85f6, /// 0x338
			 0x92d7f138, /// 0x33c
			 0x70afaca0, /// 0x340
			 0x464c09fe, /// 0x344
			 0xb236c845, /// 0x348
			 0xf406ffbf, /// 0x34c
			 0x3b57d565, /// 0x350
			 0x727deb48, /// 0x354
			 0xdec6f495, /// 0x358
			 0x7389d70a, /// 0x35c
			 0x2d3c4d55, /// 0x360
			 0x4bedf7c6, /// 0x364
			 0xe7b8281a, /// 0x368
			 0x00b929b4, /// 0x36c
			 0x0256ed96, /// 0x370
			 0xbad70050, /// 0x374
			 0xe77a1285, /// 0x378
			 0x207d95dc, /// 0x37c
			 0xb50a3d1f, /// 0x380
			 0x1a7022fd, /// 0x384
			 0xae269761, /// 0x388
			 0xad74e6c1, /// 0x38c
			 0xa7d48cc1, /// 0x390
			 0x0c24064a, /// 0x394
			 0x3bab490f, /// 0x398
			 0xef34ad8a, /// 0x39c
			 0x3c86f591, /// 0x3a0
			 0xd9676c1f, /// 0x3a4
			 0x5a9b193a, /// 0x3a8
			 0x41436f72, /// 0x3ac
			 0x865d61f7, /// 0x3b0
			 0xb7a0c8ca, /// 0x3b4
			 0xebcfc576, /// 0x3b8
			 0xd9f61c90, /// 0x3bc
			 0x77dc3bd5, /// 0x3c0
			 0xb7046deb, /// 0x3c4
			 0x89e4a600, /// 0x3c8
			 0xd464702e, /// 0x3cc
			 0x9117b099, /// 0x3d0
			 0xc48f1491, /// 0x3d4
			 0x1ced4eb8, /// 0x3d8
			 0xce60531c, /// 0x3dc
			 0xa544498e, /// 0x3e0
			 0x97ecf7f1, /// 0x3e4
			 0xe13d55a0, /// 0x3e8
			 0xd0da285d, /// 0x3ec
			 0xb71b68a7, /// 0x3f0
			 0x718d6cd0, /// 0x3f4
			 0x83d76429, /// 0x3f8
			 0xc5a4ffe1, /// 0x3fc
			 0x828bf426, /// 0x400
			 0x52f418c0, /// 0x404
			 0x06c446f6, /// 0x408
			 0xa07f6bd1, /// 0x40c
			 0x58cbc730, /// 0x410
			 0x8f28493b, /// 0x414
			 0xc789fa31, /// 0x418
			 0x8bbef5da, /// 0x41c
			 0x8ac1ba98, /// 0x420
			 0x2945ae5c, /// 0x424
			 0xeb97e6e4, /// 0x428
			 0xe942e30b, /// 0x42c
			 0x6b09ce3b, /// 0x430
			 0x7ba5ceae, /// 0x434
			 0x99ecbf80, /// 0x438
			 0xdcbc3f70, /// 0x43c
			 0xd622d7ad, /// 0x440
			 0x3be09dcc, /// 0x444
			 0xd4aefa13, /// 0x448
			 0x2532b4ea, /// 0x44c
			 0xc7adf9e9, /// 0x450
			 0xeeea2648, /// 0x454
			 0x6222d50a, /// 0x458
			 0x0cd087a0, /// 0x45c
			 0x42d36875, /// 0x460
			 0x08fb0ee9, /// 0x464
			 0x6c209d17, /// 0x468
			 0xb696de3e, /// 0x46c
			 0x7bc81323, /// 0x470
			 0x25c9e628, /// 0x474
			 0x62eaa9b4, /// 0x478
			 0xee0af453, /// 0x47c
			 0x645e84d2, /// 0x480
			 0x8ac97edc, /// 0x484
			 0x0821e1ba, /// 0x488
			 0x85419e50, /// 0x48c
			 0x05c39e0d, /// 0x490
			 0xd120002e, /// 0x494
			 0x30ffd6ec, /// 0x498
			 0xf0f3648f, /// 0x49c
			 0x4e255766, /// 0x4a0
			 0x7af4f5fe, /// 0x4a4
			 0xe0af09d8, /// 0x4a8
			 0x6182b065, /// 0x4ac
			 0xa00478d5, /// 0x4b0
			 0x6962f170, /// 0x4b4
			 0x838cb4d2, /// 0x4b8
			 0x7c4673e5, /// 0x4bc
			 0x731e547c, /// 0x4c0
			 0x117cee7c, /// 0x4c4
			 0xb0af5b8b, /// 0x4c8
			 0x4ae7a6a2, /// 0x4cc
			 0x6a4c061b, /// 0x4d0
			 0xd0b760a7, /// 0x4d4
			 0xdba2a204, /// 0x4d8
			 0x3fd2ea73, /// 0x4dc
			 0x43c7695c, /// 0x4e0
			 0x9d8e03b5, /// 0x4e4
			 0x35bc7357, /// 0x4e8
			 0x9e18c4b1, /// 0x4ec
			 0xc08cae5a, /// 0x4f0
			 0x6c084742, /// 0x4f4
			 0x45dbc91c, /// 0x4f8
			 0x682a73b1, /// 0x4fc
			 0xb2558770, /// 0x500
			 0x11014dd7, /// 0x504
			 0x253c9bcf, /// 0x508
			 0xbc19fc91, /// 0x50c
			 0x5ad69327, /// 0x510
			 0x87ebcd8e, /// 0x514
			 0xfb71b578, /// 0x518
			 0x79873ce0, /// 0x51c
			 0xf706fa33, /// 0x520
			 0x226690bd, /// 0x524
			 0x883f208c, /// 0x528
			 0x07f72e77, /// 0x52c
			 0x0d9f67c5, /// 0x530
			 0x3da24394, /// 0x534
			 0xe692725b, /// 0x538
			 0x8b28c482, /// 0x53c
			 0x5890624b, /// 0x540
			 0x780999dd, /// 0x544
			 0xb90b55f9, /// 0x548
			 0xa33d926c, /// 0x54c
			 0xc068814f, /// 0x550
			 0x029a94fb, /// 0x554
			 0x1db2f6fe, /// 0x558
			 0x72b6a33b, /// 0x55c
			 0x19ce4a8f, /// 0x560
			 0x3a3509a0, /// 0x564
			 0xc9b706f1, /// 0x568
			 0xa5c7b88a, /// 0x56c
			 0x4f50d32a, /// 0x570
			 0x05d8b779, /// 0x574
			 0xb130ffd3, /// 0x578
			 0xfc8cbaeb, /// 0x57c
			 0x1feb9852, /// 0x580
			 0x01e73a11, /// 0x584
			 0x1b0f0b00, /// 0x588
			 0x39914fb9, /// 0x58c
			 0x1cdb97ce, /// 0x590
			 0x714fd654, /// 0x594
			 0xbefd237f, /// 0x598
			 0xda077bb6, /// 0x59c
			 0x00c3e315, /// 0x5a0
			 0x19581c25, /// 0x5a4
			 0x7ebbc273, /// 0x5a8
			 0xdd49c7d1, /// 0x5ac
			 0xee08a921, /// 0x5b0
			 0xdcb5801b, /// 0x5b4
			 0x575bcf8b, /// 0x5b8
			 0xee881442, /// 0x5bc
			 0x0ad4ae8b, /// 0x5c0
			 0x766fd521, /// 0x5c4
			 0x705b7acf, /// 0x5c8
			 0x51ddc33e, /// 0x5cc
			 0x0dfad14f, /// 0x5d0
			 0x541394ea, /// 0x5d4
			 0x244898be, /// 0x5d8
			 0xe9f56f02, /// 0x5dc
			 0x2efacddf, /// 0x5e0
			 0xc65c4c16, /// 0x5e4
			 0x474de14b, /// 0x5e8
			 0xe3a75172, /// 0x5ec
			 0xb6fafb1e, /// 0x5f0
			 0x75e2662a, /// 0x5f4
			 0xebb2c6d3, /// 0x5f8
			 0xa0efaaea, /// 0x5fc
			 0x1ea69890, /// 0x600
			 0xfc9de467, /// 0x604
			 0x0b31faba, /// 0x608
			 0x45741c3c, /// 0x60c
			 0x0ddb5957, /// 0x610
			 0xa1dc8fae, /// 0x614
			 0xc157de55, /// 0x618
			 0xd9db9532, /// 0x61c
			 0x379827f3, /// 0x620
			 0xfa918ee3, /// 0x624
			 0x5fd83309, /// 0x628
			 0x168b9f3f, /// 0x62c
			 0x685c53ce, /// 0x630
			 0xf29ad7f4, /// 0x634
			 0xb65639f5, /// 0x638
			 0x6aa70908, /// 0x63c
			 0x6f715cc1, /// 0x640
			 0x2c00f7a1, /// 0x644
			 0x52508415, /// 0x648
			 0xbd2bef17, /// 0x64c
			 0x129b67ee, /// 0x650
			 0xd04af905, /// 0x654
			 0xec6864db, /// 0x658
			 0x1de11c13, /// 0x65c
			 0x6060204f, /// 0x660
			 0x13a6134f, /// 0x664
			 0xec66ac1f, /// 0x668
			 0x7afcfaca, /// 0x66c
			 0x36e7bbbd, /// 0x670
			 0xb34705c6, /// 0x674
			 0x4ed584c0, /// 0x678
			 0x1bc01d48, /// 0x67c
			 0x5c55fea1, /// 0x680
			 0xe4cb9542, /// 0x684
			 0x92e46e12, /// 0x688
			 0x3ca5926a, /// 0x68c
			 0xe95b81fa, /// 0x690
			 0x8449d051, /// 0x694
			 0x32fcdc30, /// 0x698
			 0x3a53342f, /// 0x69c
			 0xe731e350, /// 0x6a0
			 0x522460ac, /// 0x6a4
			 0x19ea0d4d, /// 0x6a8
			 0x68f2d0bc, /// 0x6ac
			 0x7b156550, /// 0x6b0
			 0x5ad96202, /// 0x6b4
			 0xcfecdbd3, /// 0x6b8
			 0x6b357f8f, /// 0x6bc
			 0x44d7fe20, /// 0x6c0
			 0xbb590e16, /// 0x6c4
			 0x3f770e6e, /// 0x6c8
			 0xabf4e923, /// 0x6cc
			 0xebda169d, /// 0x6d0
			 0xd09f1a81, /// 0x6d4
			 0xbb78a3b0, /// 0x6d8
			 0xec124e15, /// 0x6dc
			 0xf4bba0b5, /// 0x6e0
			 0x20484102, /// 0x6e4
			 0xea6204d8, /// 0x6e8
			 0x0431a8d0, /// 0x6ec
			 0xba192a2d, /// 0x6f0
			 0x7cd922cc, /// 0x6f4
			 0xb56b6599, /// 0x6f8
			 0x2fcd41a0, /// 0x6fc
			 0x39196121, /// 0x700
			 0x4dffe358, /// 0x704
			 0x1096eaa9, /// 0x708
			 0x7fb36877, /// 0x70c
			 0x8d00f861, /// 0x710
			 0xd2f66014, /// 0x714
			 0x672dbd58, /// 0x718
			 0xd157e32e, /// 0x71c
			 0x4a857fe5, /// 0x720
			 0xf8af463a, /// 0x724
			 0xdb52d1f0, /// 0x728
			 0x17e1ad74, /// 0x72c
			 0xe2f14321, /// 0x730
			 0x97b3b4b3, /// 0x734
			 0xecf0dbf4, /// 0x738
			 0xa4a7046a, /// 0x73c
			 0xff24b7ee, /// 0x740
			 0x1c11defa, /// 0x744
			 0xc36d52c4, /// 0x748
			 0xeb499886, /// 0x74c
			 0x0c2caa1e, /// 0x750
			 0xc8813ced, /// 0x754
			 0xbb8adcf1, /// 0x758
			 0xb4935c45, /// 0x75c
			 0x09c48b78, /// 0x760
			 0x993e9571, /// 0x764
			 0x4d172c53, /// 0x768
			 0xe9dd0f83, /// 0x76c
			 0xba8151a7, /// 0x770
			 0xbbd2e5d6, /// 0x774
			 0x8a7eb7aa, /// 0x778
			 0xb90ebc55, /// 0x77c
			 0xb8c3a149, /// 0x780
			 0x18311017, /// 0x784
			 0xb3e35c71, /// 0x788
			 0x5ce126d4, /// 0x78c
			 0x0f748bf2, /// 0x790
			 0xfd76c083, /// 0x794
			 0x33f4a84b, /// 0x798
			 0xabdf4a62, /// 0x79c
			 0xb5c4fbd0, /// 0x7a0
			 0x26359576, /// 0x7a4
			 0x03a7db77, /// 0x7a8
			 0x08242510, /// 0x7ac
			 0x21663edb, /// 0x7b0
			 0x2d4856f6, /// 0x7b4
			 0x7d2f99d2, /// 0x7b8
			 0x7b7d5f32, /// 0x7bc
			 0x7c7a3c66, /// 0x7c0
			 0x1c4afe30, /// 0x7c4
			 0x0b1cd524, /// 0x7c8
			 0x3af26b6c, /// 0x7cc
			 0xb490afcd, /// 0x7d0
			 0xedfc4b62, /// 0x7d4
			 0x0e01d557, /// 0x7d8
			 0x2e444754, /// 0x7dc
			 0x54b0622f, /// 0x7e0
			 0x23bbccc2, /// 0x7e4
			 0xa53d82ec, /// 0x7e8
			 0x2fa50e94, /// 0x7ec
			 0x1a44f3cc, /// 0x7f0
			 0x5e505994, /// 0x7f4
			 0xa3534a35, /// 0x7f8
			 0x9c1d5fb9, /// 0x7fc
			 0x7e9263e8, /// 0x800
			 0x2eae0407, /// 0x804
			 0x432e88c5, /// 0x808
			 0x5b983ff7, /// 0x80c
			 0xeeb9882c, /// 0x810
			 0x8574b875, /// 0x814
			 0x2f9515b9, /// 0x818
			 0xb762eb1f, /// 0x81c
			 0x2c4bf621, /// 0x820
			 0xe9507448, /// 0x824
			 0x436c4854, /// 0x828
			 0x7b256007, /// 0x82c
			 0x3c26c39f, /// 0x830
			 0x0c5ab584, /// 0x834
			 0x78f157ac, /// 0x838
			 0xb12cbccb, /// 0x83c
			 0xe054b33a, /// 0x840
			 0xe4a041e6, /// 0x844
			 0x42923f46, /// 0x848
			 0x0ce3530a, /// 0x84c
			 0x6eff9aeb, /// 0x850
			 0x215f8536, /// 0x854
			 0xbecdd17f, /// 0x858
			 0x34694183, /// 0x85c
			 0x2ca1c1db, /// 0x860
			 0x85d3243e, /// 0x864
			 0x061ee694, /// 0x868
			 0x0c4b0752, /// 0x86c
			 0x5eb84e7d, /// 0x870
			 0xa594f8db, /// 0x874
			 0x5e7d541b, /// 0x878
			 0x3775bfd9, /// 0x87c
			 0xe433abd7, /// 0x880
			 0x62bb3451, /// 0x884
			 0x38f31668, /// 0x888
			 0x8fe60e0c, /// 0x88c
			 0xc3a2842b, /// 0x890
			 0xa85185f7, /// 0x894
			 0xe72c636b, /// 0x898
			 0x0e401911, /// 0x89c
			 0x98c39da0, /// 0x8a0
			 0xfd4c92f8, /// 0x8a4
			 0x04b880b2, /// 0x8a8
			 0x62fe813b, /// 0x8ac
			 0x6b0a0f2d, /// 0x8b0
			 0xce3c8382, /// 0x8b4
			 0xe946e6bc, /// 0x8b8
			 0x9120ebba, /// 0x8bc
			 0xb9afbdeb, /// 0x8c0
			 0x29532d6f, /// 0x8c4
			 0x03bc76fa, /// 0x8c8
			 0x5b1b9828, /// 0x8cc
			 0x752b7e96, /// 0x8d0
			 0xa472e90a, /// 0x8d4
			 0x5ed687e7, /// 0x8d8
			 0xb2534db7, /// 0x8dc
			 0xb500026f, /// 0x8e0
			 0x2466844e, /// 0x8e4
			 0x302e5097, /// 0x8e8
			 0x79840445, /// 0x8ec
			 0xb8f0218b, /// 0x8f0
			 0xe22889cc, /// 0x8f4
			 0x447e4497, /// 0x8f8
			 0xa59bfeba, /// 0x8fc
			 0x0be7568e, /// 0x900
			 0xdd77b729, /// 0x904
			 0x8ae66b6c, /// 0x908
			 0xaa0e906b, /// 0x90c
			 0x1cecf0de, /// 0x910
			 0xe43afb56, /// 0x914
			 0x681e484a, /// 0x918
			 0x79d55333, /// 0x91c
			 0xaeed4157, /// 0x920
			 0xa9049e1a, /// 0x924
			 0xa9e38120, /// 0x928
			 0x73c8894f, /// 0x92c
			 0x56bab91e, /// 0x930
			 0x85334631, /// 0x934
			 0xfbcc51a9, /// 0x938
			 0x390ea685, /// 0x93c
			 0x28b4313f, /// 0x940
			 0xdae2f994, /// 0x944
			 0xf9391322, /// 0x948
			 0xf52bb3af, /// 0x94c
			 0xa23d8ada, /// 0x950
			 0x34e496c1, /// 0x954
			 0x19abbba2, /// 0x958
			 0x900dafdb, /// 0x95c
			 0x2220e4e2, /// 0x960
			 0x54f316c7, /// 0x964
			 0x6a3ab972, /// 0x968
			 0x20e95be2, /// 0x96c
			 0x25282354, /// 0x970
			 0x14b90f87, /// 0x974
			 0xa3f602ad, /// 0x978
			 0x76efbcac, /// 0x97c
			 0x212e1d55, /// 0x980
			 0x52c7a8db, /// 0x984
			 0x9e8636c8, /// 0x988
			 0x6e895628, /// 0x98c
			 0xfa926c48, /// 0x990
			 0x0737df4c, /// 0x994
			 0x59fc7c41, /// 0x998
			 0x44f123ef, /// 0x99c
			 0xbc60219e, /// 0x9a0
			 0x34dd7d74, /// 0x9a4
			 0xef0e2851, /// 0x9a8
			 0x122c6453, /// 0x9ac
			 0x9700950f, /// 0x9b0
			 0xd35e0545, /// 0x9b4
			 0xe45eb03b, /// 0x9b8
			 0x757d1e4b, /// 0x9bc
			 0xaa8d9814, /// 0x9c0
			 0xaf37d0b0, /// 0x9c4
			 0xfb5ac092, /// 0x9c8
			 0x1a34aa86, /// 0x9cc
			 0xc1112459, /// 0x9d0
			 0x2ef1a3a3, /// 0x9d4
			 0xec8780c2, /// 0x9d8
			 0x82748c4b, /// 0x9dc
			 0x2da0633d, /// 0x9e0
			 0x4a4a7c1f, /// 0x9e4
			 0xe4f01565, /// 0x9e8
			 0x0f9ee070, /// 0x9ec
			 0x76f2206a, /// 0x9f0
			 0x26f25b40, /// 0x9f4
			 0xdeeb6524, /// 0x9f8
			 0x5d386ae1, /// 0x9fc
			 0x58ddcd5e, /// 0xa00
			 0x5ad62555, /// 0xa04
			 0xe47b7499, /// 0xa08
			 0x90b52860, /// 0xa0c
			 0x15a5cad0, /// 0xa10
			 0x963f1332, /// 0xa14
			 0xdec1a51a, /// 0xa18
			 0x280b5249, /// 0xa1c
			 0x5264f9a7, /// 0xa20
			 0x8ba8d119, /// 0xa24
			 0x89c2919e, /// 0xa28
			 0xf8d5830e, /// 0xa2c
			 0x787e68cb, /// 0xa30
			 0xee226f86, /// 0xa34
			 0x4b283c63, /// 0xa38
			 0xaccece11, /// 0xa3c
			 0x891cb109, /// 0xa40
			 0x3d96f957, /// 0xa44
			 0xa07988b6, /// 0xa48
			 0x58f4be7e, /// 0xa4c
			 0xa4dee95d, /// 0xa50
			 0xe1b6b2ca, /// 0xa54
			 0x415621ad, /// 0xa58
			 0x456878ee, /// 0xa5c
			 0xeea9a7dd, /// 0xa60
			 0x1297affb, /// 0xa64
			 0x57442180, /// 0xa68
			 0xa7b33517, /// 0xa6c
			 0x1649dd9c, /// 0xa70
			 0x78d00a2b, /// 0xa74
			 0x41d6d919, /// 0xa78
			 0xfd43170c, /// 0xa7c
			 0x7a078948, /// 0xa80
			 0xcd3405e9, /// 0xa84
			 0xb484bc2e, /// 0xa88
			 0x2feb2ec2, /// 0xa8c
			 0xb0694465, /// 0xa90
			 0xb87f5623, /// 0xa94
			 0x331fcfbc, /// 0xa98
			 0x7b45d65f, /// 0xa9c
			 0x31108806, /// 0xaa0
			 0xe67dc8e5, /// 0xaa4
			 0xe9adaa37, /// 0xaa8
			 0x2fe4a647, /// 0xaac
			 0x3be56e18, /// 0xab0
			 0xaa1521c9, /// 0xab4
			 0x72924f6c, /// 0xab8
			 0x99487095, /// 0xabc
			 0xded8c396, /// 0xac0
			 0x5c24455d, /// 0xac4
			 0xa3965674, /// 0xac8
			 0xf924e7ad, /// 0xacc
			 0x4709b14f, /// 0xad0
			 0x26cd7540, /// 0xad4
			 0x52c8f912, /// 0xad8
			 0xc479ad84, /// 0xadc
			 0x8d38e781, /// 0xae0
			 0x4fedd041, /// 0xae4
			 0x6581c1cd, /// 0xae8
			 0x30dfbe44, /// 0xaec
			 0x67232b7c, /// 0xaf0
			 0x6a082e94, /// 0xaf4
			 0x944ace20, /// 0xaf8
			 0x66b01931, /// 0xafc
			 0x56fde4e4, /// 0xb00
			 0x1246d882, /// 0xb04
			 0xbfdd8581, /// 0xb08
			 0xab09967f, /// 0xb0c
			 0xdea0deff, /// 0xb10
			 0xa0e4e078, /// 0xb14
			 0x8d02e14f, /// 0xb18
			 0x2348d9ff, /// 0xb1c
			 0xd6f3c976, /// 0xb20
			 0xa5e2c235, /// 0xb24
			 0x64c38ca1, /// 0xb28
			 0x96e0949f, /// 0xb2c
			 0x0b1d3c36, /// 0xb30
			 0xbce562f0, /// 0xb34
			 0x6d22667d, /// 0xb38
			 0x7cdc0625, /// 0xb3c
			 0x409df4e5, /// 0xb40
			 0xea177ab8, /// 0xb44
			 0x4b25291b, /// 0xb48
			 0x17065827, /// 0xb4c
			 0xd8475618, /// 0xb50
			 0x96416b49, /// 0xb54
			 0x8e444d5b, /// 0xb58
			 0x7f2158cd, /// 0xb5c
			 0xecc5855b, /// 0xb60
			 0x7d5ea40c, /// 0xb64
			 0x09a27e73, /// 0xb68
			 0x05fa0306, /// 0xb6c
			 0xaf51dbb8, /// 0xb70
			 0x895901a5, /// 0xb74
			 0xa8b83911, /// 0xb78
			 0x08323bc6, /// 0xb7c
			 0x94d47a54, /// 0xb80
			 0x0e10129d, /// 0xb84
			 0xd0865abe, /// 0xb88
			 0x21b81001, /// 0xb8c
			 0x4657b81e, /// 0xb90
			 0xe920f343, /// 0xb94
			 0x55e4aec8, /// 0xb98
			 0x7e5e8dc2, /// 0xb9c
			 0x04b5a2b6, /// 0xba0
			 0xa6c0e861, /// 0xba4
			 0xb0231381, /// 0xba8
			 0xf1f5c51a, /// 0xbac
			 0xb5d6f019, /// 0xbb0
			 0x71f1f586, /// 0xbb4
			 0xc730e029, /// 0xbb8
			 0x367b6a95, /// 0xbbc
			 0x2a6d17aa, /// 0xbc0
			 0xb3c6eb1a, /// 0xbc4
			 0x1734da5a, /// 0xbc8
			 0x725d6964, /// 0xbcc
			 0xc6dbd2d9, /// 0xbd0
			 0x98b55da1, /// 0xbd4
			 0x2742f51c, /// 0xbd8
			 0x90d6148e, /// 0xbdc
			 0xab7338b8, /// 0xbe0
			 0xeb012dde, /// 0xbe4
			 0x27c23374, /// 0xbe8
			 0x2d127d2e, /// 0xbec
			 0xa0efebc3, /// 0xbf0
			 0xe51f3fe3, /// 0xbf4
			 0xbc1295c4, /// 0xbf8
			 0x3014829a, /// 0xbfc
			 0xd622fe7a, /// 0xc00
			 0xacfed449, /// 0xc04
			 0x195e8b84, /// 0xc08
			 0x8b73f2e6, /// 0xc0c
			 0xaf786471, /// 0xc10
			 0x9ee65d8f, /// 0xc14
			 0x702f68fd, /// 0xc18
			 0x896e3e7f, /// 0xc1c
			 0x6bacb13c, /// 0xc20
			 0xcb8f12f8, /// 0xc24
			 0x54c97d15, /// 0xc28
			 0xde596866, /// 0xc2c
			 0x14313712, /// 0xc30
			 0x7d79297a, /// 0xc34
			 0xc347eb42, /// 0xc38
			 0x58afed87, /// 0xc3c
			 0x4cdfb0b3, /// 0xc40
			 0x2e3dc987, /// 0xc44
			 0xe5d70ee6, /// 0xc48
			 0x5e7fe6dd, /// 0xc4c
			 0x86fca6fa, /// 0xc50
			 0xb4a7cb66, /// 0xc54
			 0x1eda9f86, /// 0xc58
			 0xd70d141b, /// 0xc5c
			 0x277f0496, /// 0xc60
			 0xf536b19b, /// 0xc64
			 0x5ad210e1, /// 0xc68
			 0xed1bb960, /// 0xc6c
			 0xa683e19d, /// 0xc70
			 0xaaa2fe01, /// 0xc74
			 0xfc49bdb7, /// 0xc78
			 0x32f73bfc, /// 0xc7c
			 0x56edc3dd, /// 0xc80
			 0xd9f296c3, /// 0xc84
			 0x052805f0, /// 0xc88
			 0x3f75ac37, /// 0xc8c
			 0x10b6d5d6, /// 0xc90
			 0xd88e6117, /// 0xc94
			 0x4a312db9, /// 0xc98
			 0xc4445b9e, /// 0xc9c
			 0x1c5222f2, /// 0xca0
			 0x4ea9b613, /// 0xca4
			 0xa74687cc, /// 0xca8
			 0x12646e2f, /// 0xcac
			 0xc97a2875, /// 0xcb0
			 0xf2c30579, /// 0xcb4
			 0x52872fbb, /// 0xcb8
			 0xc59d5da2, /// 0xcbc
			 0x9f7bda2d, /// 0xcc0
			 0x5ffea22d, /// 0xcc4
			 0xb2d62d1f, /// 0xcc8
			 0x5dd8f1f3, /// 0xccc
			 0x6d79250c, /// 0xcd0
			 0x372d3275, /// 0xcd4
			 0x5c0d9f7e, /// 0xcd8
			 0x3ae169b7, /// 0xcdc
			 0xaad8b48b, /// 0xce0
			 0x0947279e, /// 0xce4
			 0xa9d81256, /// 0xce8
			 0x83f35c7d, /// 0xcec
			 0x0304dab3, /// 0xcf0
			 0x2e7f9b19, /// 0xcf4
			 0x89a62a01, /// 0xcf8
			 0xed1d3cf0, /// 0xcfc
			 0x28a066ae, /// 0xd00
			 0x1a28df37, /// 0xd04
			 0xed050778, /// 0xd08
			 0xc0810e4e, /// 0xd0c
			 0x2d91030c, /// 0xd10
			 0x4f7abfc8, /// 0xd14
			 0x209adb12, /// 0xd18
			 0xd804105e, /// 0xd1c
			 0x76376648, /// 0xd20
			 0x9a38b45a, /// 0xd24
			 0x3d83dfe2, /// 0xd28
			 0xa91563b5, /// 0xd2c
			 0xc4c6f347, /// 0xd30
			 0xc9be145e, /// 0xd34
			 0x9112b171, /// 0xd38
			 0x05931b42, /// 0xd3c
			 0xdf0f5311, /// 0xd40
			 0xfd78059e, /// 0xd44
			 0x901f3784, /// 0xd48
			 0x4d6fe722, /// 0xd4c
			 0xa2089f7b, /// 0xd50
			 0x235b0750, /// 0xd54
			 0x2f836601, /// 0xd58
			 0x5f6b9fc9, /// 0xd5c
			 0xc662138d, /// 0xd60
			 0x489835c2, /// 0xd64
			 0xc3243416, /// 0xd68
			 0x62c21e1c, /// 0xd6c
			 0x3fdeff7b, /// 0xd70
			 0x77ca252d, /// 0xd74
			 0x111590ed, /// 0xd78
			 0x68cc2f29, /// 0xd7c
			 0x64087941, /// 0xd80
			 0xcde999f6, /// 0xd84
			 0x157cea82, /// 0xd88
			 0x8f4e12be, /// 0xd8c
			 0xe2276537, /// 0xd90
			 0xed96a7ed, /// 0xd94
			 0x723a90f6, /// 0xd98
			 0x15a3829b, /// 0xd9c
			 0xd219c091, /// 0xda0
			 0x76d73ca9, /// 0xda4
			 0xdce5f9b8, /// 0xda8
			 0xd40f00d4, /// 0xdac
			 0x3b50049d, /// 0xdb0
			 0x67593346, /// 0xdb4
			 0x4dc59b9f, /// 0xdb8
			 0xa4dfb39c, /// 0xdbc
			 0x267d7bc0, /// 0xdc0
			 0x039fe2f9, /// 0xdc4
			 0x0bc43524, /// 0xdc8
			 0x9a0ec2d9, /// 0xdcc
			 0xfc2bb896, /// 0xdd0
			 0x44978e99, /// 0xdd4
			 0x27e6e499, /// 0xdd8
			 0x5ac4c382, /// 0xddc
			 0x52078bef, /// 0xde0
			 0xdaa695d8, /// 0xde4
			 0xd8a27d1a, /// 0xde8
			 0x01b0caab, /// 0xdec
			 0x47514d6f, /// 0xdf0
			 0xf54a4202, /// 0xdf4
			 0xb0418210, /// 0xdf8
			 0x411ad701, /// 0xdfc
			 0xb2e14405, /// 0xe00
			 0xa05dbd58, /// 0xe04
			 0x441e7bd1, /// 0xe08
			 0x22e961e0, /// 0xe0c
			 0x78ba2192, /// 0xe10
			 0xf0a04cf0, /// 0xe14
			 0x7e8085aa, /// 0xe18
			 0x3f22a7b5, /// 0xe1c
			 0x39a9c27a, /// 0xe20
			 0x2400fe6f, /// 0xe24
			 0x4aa69a34, /// 0xe28
			 0xa292fdd1, /// 0xe2c
			 0x51226b9e, /// 0xe30
			 0x2a25efe7, /// 0xe34
			 0xd8d20504, /// 0xe38
			 0xd03b2f04, /// 0xe3c
			 0xee618906, /// 0xe40
			 0xad600504, /// 0xe44
			 0xc6794d69, /// 0xe48
			 0x6543fdf5, /// 0xe4c
			 0x6e79bd76, /// 0xe50
			 0xcfba8141, /// 0xe54
			 0x5b8febd7, /// 0xe58
			 0x20ff2e05, /// 0xe5c
			 0x64b7ca84, /// 0xe60
			 0x3f57b1a3, /// 0xe64
			 0xb53ee307, /// 0xe68
			 0x0f016ca5, /// 0xe6c
			 0x740f2caf, /// 0xe70
			 0x10c04954, /// 0xe74
			 0x1f35d972, /// 0xe78
			 0xd98a0bac, /// 0xe7c
			 0x737e5613, /// 0xe80
			 0x2a01730f, /// 0xe84
			 0x7199d1eb, /// 0xe88
			 0x70346c17, /// 0xe8c
			 0x1e39c49c, /// 0xe90
			 0x5bb8122e, /// 0xe94
			 0x5e17ce89, /// 0xe98
			 0x994c2441, /// 0xe9c
			 0x8fe755f2, /// 0xea0
			 0x61c24500, /// 0xea4
			 0x9e3bbdb8, /// 0xea8
			 0xda063ae5, /// 0xeac
			 0xd057c835, /// 0xeb0
			 0x8ff96d6d, /// 0xeb4
			 0x5f514511, /// 0xeb8
			 0x935ab83f, /// 0xebc
			 0x3d3006f3, /// 0xec0
			 0x05b0bc85, /// 0xec4
			 0x50927072, /// 0xec8
			 0x023d2af8, /// 0xecc
			 0xd67dafec, /// 0xed0
			 0x0054f0fc, /// 0xed4
			 0x1d7b0d32, /// 0xed8
			 0x58b7cbb7, /// 0xedc
			 0x5861719e, /// 0xee0
			 0xb68b83e5, /// 0xee4
			 0xfeba8fb8, /// 0xee8
			 0xa741b4f8, /// 0xeec
			 0x891be973, /// 0xef0
			 0x1e9dd322, /// 0xef4
			 0x5270acb3, /// 0xef8
			 0xa4812230, /// 0xefc
			 0x678b827b, /// 0xf00
			 0x69461fdc, /// 0xf04
			 0xadb8c112, /// 0xf08
			 0x7f94a2db, /// 0xf0c
			 0x7b5e5fb7, /// 0xf10
			 0x78339451, /// 0xf14
			 0x23b92ece, /// 0xf18
			 0xe6801409, /// 0xf1c
			 0x3c29347f, /// 0xf20
			 0x4ef1d9f9, /// 0xf24
			 0x81fdc79f, /// 0xf28
			 0xc3836cba, /// 0xf2c
			 0x229c7a68, /// 0xf30
			 0x4af68814, /// 0xf34
			 0x2b7785fb, /// 0xf38
			 0x77eb0ac9, /// 0xf3c
			 0xcf4ab517, /// 0xf40
			 0x73ef1fec, /// 0xf44
			 0xfd3cbf09, /// 0xf48
			 0xfef8a042, /// 0xf4c
			 0x29937c00, /// 0xf50
			 0x227b6fcf, /// 0xf54
			 0x48a17358, /// 0xf58
			 0x982a5b88, /// 0xf5c
			 0x57bcfc56, /// 0xf60
			 0x79a51d44, /// 0xf64
			 0xa1e7a9b3, /// 0xf68
			 0x9c5a3e15, /// 0xf6c
			 0xa96d2e85, /// 0xf70
			 0x413d8da9, /// 0xf74
			 0x7ec2be79, /// 0xf78
			 0xf0712a86, /// 0xf7c
			 0x18a9c592, /// 0xf80
			 0x9f89fbc9, /// 0xf84
			 0xc8e901af, /// 0xf88
			 0xa8262e96, /// 0xf8c
			 0x9ea35a7b, /// 0xf90
			 0xeaca9381, /// 0xf94
			 0xd6d505fe, /// 0xf98
			 0xb6a65c53, /// 0xf9c
			 0x5df3d9dd, /// 0xfa0
			 0x9c3ee618, /// 0xfa4
			 0x61eb1567, /// 0xfa8
			 0x14237b07, /// 0xfac
			 0x1d6fd691, /// 0xfb0
			 0xae90db45, /// 0xfb4
			 0xd7e1a4dc, /// 0xfb8
			 0xf473b8a0, /// 0xfbc
			 0x36f8e838, /// 0xfc0
			 0x66382dde, /// 0xfc4
			 0xb48cf2d0, /// 0xfc8
			 0xf885909a, /// 0xfcc
			 0xcdc69a8e, /// 0xfd0
			 0xdc3fb5a8, /// 0xfd4
			 0x2aceb68c, /// 0xfd8
			 0x97cccb0d, /// 0xfdc
			 0x643833a6, /// 0xfe0
			 0x59a21aa4, /// 0xfe4
			 0x48fae266, /// 0xfe8
			 0xd2bfee24, /// 0xfec
			 0x345d09ac, /// 0xff0
			 0x39d1f885, /// 0xff4
			 0x7cfe4a60, /// 0xff8
			 0x16ff200c /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00000
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00008
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0000c
			 0x80000000,                                                  // -zero                                       /// 00010
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00014
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00018
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0001c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00020
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00028
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00030
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00034
			 0x33333333,                                                  // 4 random values                             /// 00038
			 0x3f028f5c,                                                  // 0.51                                        /// 0003c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00040
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00048
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0004c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00050
			 0x3f028f5c,                                                  // 0.51                                        /// 00054
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00058
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0005c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00060
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00064
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0006c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00070
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00074
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x80011111,                                                  // -9.7958E-41                                 /// 0007c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00084
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00088
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0008c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00094
			 0x55555555,                                                  // 4 random values                             /// 00098
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0009c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000a8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 000ac
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000b0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000b8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000bc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 000c0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000c4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000c8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000cc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000d0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000d8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000dc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000e0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 000ec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000f0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 000f8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 000fc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00100
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00104
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0010c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00110
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00114
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00118
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00120
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00124
			 0x3f028f5c,                                                  // 0.51                                        /// 00128
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0012c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00130
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00134
			 0xcb000000,                                                  // -8388608.0                                  /// 00138
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0013c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00140
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00144
			 0x0c600000,                                                  // Leading 1s:                                 /// 00148
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0014c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00150
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00154
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00158
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0015c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00160
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00164
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x80011111,                                                  // -9.7958E-41                                 /// 0016c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00170
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00174
			 0x80000000,                                                  // -zero                                       /// 00178
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0017c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00180
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00184
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00188
			 0x00000000,                                                  // zero                                        /// 0018c
			 0x00000000,                                                  // zero                                        /// 00190
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00194
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00198
			 0x0e000007,                                                  // Trailing 1s:                                /// 0019c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001a0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001a4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001a8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001ac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 001b0
			 0x7fc00000,                                                  // cquiet NaN                                  /// 001b4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001bc
			 0x00000000,                                                  // zero                                        /// 001c0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 001c4
			 0x33333333,                                                  // 4 random values                             /// 001c8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 001cc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 001d4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 001d8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001e0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001e4
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001ec
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001f0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 001f4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 001f8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001fc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00204
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00208
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0020c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00210
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0xff800000,                                                  // -inf                                        /// 00218
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0021c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00220
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00224
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00228
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0022c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00234
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0023c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00240
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00248
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0024c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00250
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00258
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00260
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00264
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00268
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0026c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00270
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00274
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00278
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00280
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0028c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00290
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00294
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00298
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0029c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002a0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 002a4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002a8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002ac
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 002b0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002b4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002b8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 002bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002c0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002c4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002cc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002d0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002d4
			 0xbf028f5c,                                                  // -0.51                                       /// 002d8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002dc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 002e8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002ec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002f4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 002f8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002fc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00300
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00310
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00314
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00318
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0031c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00320
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00324
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00328
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0032c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00330
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00334
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00338
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00340
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00344
			 0x0c600000,                                                  // Leading 1s:                                 /// 00348
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0034c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00350
			 0x7f800000,                                                  // inf                                         /// 00354
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00358
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0035c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00360
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00364
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00368
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00370
			 0xbf028f5c,                                                  // -0.51                                       /// 00374
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00378
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0037c
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00384
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00388
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0038c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00390
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00394
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00398
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0039c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003ac
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003b4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 003b8
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003bc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 003c0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003c8
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 003d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003d8
			 0x0c780000,                                                  // Leading 1s:                                 /// 003dc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003e0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 003e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003e8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 003ec
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003f0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 003f8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003fc
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00400
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00404
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00408
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0040c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00410
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00418
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0041c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00420
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00424
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00428
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 0042c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00430
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00434
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00440
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00444
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00448
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0044c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00450
			 0x4b000000,                                                  // 8388608.0                                   /// 00454
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00458
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0045c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00460
			 0x80011111,                                                  // -9.7958E-41                                 /// 00464
			 0xcb000000,                                                  // -8388608.0                                  /// 00468
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0046c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00470
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00474
			 0x00011111,                                                  // 9.7958E-41                                  /// 00478
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0047c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00480
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00488
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0048c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00490
			 0x7fc00001,                                                  // signaling NaN                               /// 00494
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00498
			 0x33333333,                                                  // 4 random values                             /// 0049c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004a0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a4
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a8
			 0x00011111,                                                  // 9.7958E-41                                  /// 004ac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 004b0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 004b4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004bc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004c0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004c4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 004c8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004cc
			 0x33333333,                                                  // 4 random values                             /// 004d0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004d4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 004dc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004e0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 004e4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004e8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 004f0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 004f8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004fc
			 0x0e000001,                                                  // Trailing 1s:                                /// 00500
			 0x7f800000,                                                  // inf                                         /// 00504
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0050c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00510
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00514
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00518
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0051c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00520
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00524
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00528
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0052c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00530
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00534
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00538
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0053c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00540
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00544
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00548
			 0x0c700000,                                                  // Leading 1s:                                 /// 0054c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00550
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00554
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00558
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0055c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00560
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00564
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00568
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0056c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00570
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00574
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00578
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0057c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00580
			 0x3f028f5c,                                                  // 0.51                                        /// 00584
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00588
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0058c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00590
			 0x80011111,                                                  // -9.7958E-41                                 /// 00594
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00598
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0059c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005a0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 005a4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005a8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005ac
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 005b0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 005b4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005b8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005bc
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 005c0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 005c4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005c8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005cc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005d4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005dc
			 0x00011111,                                                  // 9.7958E-41                                  /// 005e0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 005e4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005e8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 005ec
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 005f4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005fc
			 0x80000000,                                                  // -zero                                       /// 00600
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0xffc00001,                                                  // -signaling NaN                              /// 0060c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00610
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00614
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00618
			 0x0c400000,                                                  // Leading 1s:                                 /// 0061c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00620
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00624
			 0xff800000,                                                  // -inf                                        /// 00628
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0062c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00630
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00634
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0063c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00640
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00644
			 0x80000000,                                                  // -zero                                       /// 00648
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0064c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00650
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00654
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00658
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0065c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00660
			 0x0c400000,                                                  // Leading 1s:                                 /// 00664
			 0x00000000,                                                  // zero                                        /// 00668
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0066c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00670
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00674
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00678
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0067c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00680
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00684
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00688
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0068c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00690
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00694
			 0x00011111,                                                  // 9.7958E-41                                  /// 00698
			 0x7fc00001,                                                  // signaling NaN                               /// 0069c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 006a4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 006ac
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006b0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 006b8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006bc
			 0x0c780000,                                                  // Leading 1s:                                 /// 006c0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006c4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006cc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006d0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006d4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 006d8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 006dc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 006e0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 006e4
			 0x80000000,                                                  // -zero                                       /// 006e8
			 0xbf028f5c,                                                  // -0.51                                       /// 006ec
			 0x80011111,                                                  // -9.7958E-41                                 /// 006f0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006f8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006fc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00700
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00704
			 0x0e000007,                                                  // Trailing 1s:                                /// 00708
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0070c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00710
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00718
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00720
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00728
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00730
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00734
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0073c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00740
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00744
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00748
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0074c
			 0xffc00001,                                                  // -signaling NaN                              /// 00750
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00754
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00758
			 0x00000000,                                                  // zero                                        /// 0075c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00764
			 0x00011111,                                                  // 9.7958E-41                                  /// 00768
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0076c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00770
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00774
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0077c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00780
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00784
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00788
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0078c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00790
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00794
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00798
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0079c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 007a0
			 0x0e000007,                                                  // Trailing 1s:                                /// 007a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007a8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 007b0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 007b4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007bc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007c0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 007c4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x55555555,                                                  // 4 random values                             /// 007cc
			 0x80011111,                                                  // -9.7958E-41                                 /// 007d0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 007d4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 007d8
			 0xcb000000,                                                  // -8388608.0                                  /// 007dc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 007e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007e8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ec
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 007f0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007f4
			 0x80011111,                                                  // -9.7958E-41                                 /// 007f8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007fc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00800
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00804
			 0x4b000000,                                                  // 8388608.0                                   /// 00808
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0080c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00810
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00814
			 0x80011111,                                                  // -9.7958E-41                                 /// 00818
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0081c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00820
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00824
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00828
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0082c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00830
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00834
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00838
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0083c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00840
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00844
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00848
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x00000000,                                                  // zero                                        /// 00850
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00858
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 0085c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00860
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00864
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00868
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 0086c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00870
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00874
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00878
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0087c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00880
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00884
			 0x0e000007,                                                  // Trailing 1s:                                /// 00888
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0088c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00890
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00894
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0089c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 008a8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008b0
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 008b4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008b8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 008bc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008c4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 008c8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x7f800000,                                                  // inf                                         /// 008d0
			 0x0c400000,                                                  // Leading 1s:                                 /// 008d4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008d8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008dc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008e8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008ec
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 008f0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008f4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008f8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 008fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00900
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00904
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00908
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0090c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x55555555,                                                  // 4 random values                             /// 00914
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00918
			 0x007fffff,                                                  // 1.1754942E-38                               /// 0091c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00920
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00924
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00928
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0092c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00930
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0093c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00940
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00944
			 0x0e000003,                                                  // Trailing 1s:                                /// 00948
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0094c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00950
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00958
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0095c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00960
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00964
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00968
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00970
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00974
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00978
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0097c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00980
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00988
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00990
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00994
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00998
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009a0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009a4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009a8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009ac
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 009b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x7f800000,                                                  // inf                                         /// 009b8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009bc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009c0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 009c4
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009c8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 009cc
			 0x0c600000,                                                  // Leading 1s:                                 /// 009d0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009dc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 009e0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 009e4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009e8
			 0xff800000,                                                  // -inf                                        /// 009ec
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 009f0
			 0x0c600000,                                                  // Leading 1s:                                 /// 009f4
			 0x0e000001,                                                  // Trailing 1s:                                /// 009f8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 009fc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00a00
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a04
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a08
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00a0c
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0x7f800000,                                                  // inf                                         /// 00a14
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00a18
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a1c
			 0xbf028f5c,                                                  // -0.51                                       /// 00a20
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a28
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a2c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00a34
			 0xcb000000,                                                  // -8388608.0                                  /// 00a38
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a3c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00a4c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a58
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a5c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a60
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a64
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x7fc00001,                                                  // signaling NaN                               /// 00a6c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a70
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a74
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a78
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a7c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00a84
			 0xbf028f5c,                                                  // -0.51                                       /// 00a88
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a8c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a90
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a94
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a98
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00a9c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00aa0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00aa4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00aa8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00aac
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ab8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00abc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ac0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x0e000003,                                                  // Trailing 1s:                                /// 00ac8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00ad0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ad4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00ad8
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00ae0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ae4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00af0
			 0x7f800000,                                                  // inf                                         /// 00af4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00af8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00afc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b00
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b04
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b08
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b0c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b10
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b14
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b18
			 0xbf028f5c,                                                  // -0.51                                       /// 00b1c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b20
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00b24
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b28
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00b2c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b30
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b34
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b38
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00b3c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00b40
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b44
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b48
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b4c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b50
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00b54
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b58
			 0x7fc00001,                                                  // signaling NaN                               /// 00b5c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b60
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b64
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b68
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b6c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00b70
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b74
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00b78
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b80
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00b84
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b88
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b8c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00b90
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b94
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00b98
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b9c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00ba0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ba4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ba8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bac
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bb0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bb4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bbc
			 0xff800000,                                                  // -inf                                        /// 00bc0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bc4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bc8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00bcc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00bd0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00bd4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00bd8
			 0xcb000000,                                                  // -8388608.0                                  /// 00bdc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00be0
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00be4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00bec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00bf0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bf4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00bf8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c00
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c04
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c08
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00c0c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c10
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c14
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00c18
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c1c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c20
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c24
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c28
			 0x33333333,                                                  // 4 random values                             /// 00c2c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c38
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c3c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c40
			 0x33333333,                                                  // 4 random values                             /// 00c44
			 0xcb000000,                                                  // -8388608.0                                  /// 00c48
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00c4c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c50
			 0x7fc00001,                                                  // signaling NaN                               /// 00c54
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c58
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00c60
			 0x00000000,                                                  // zero                                        /// 00c64
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c68
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c6c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c70
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c74
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c78
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00c7c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00c80
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c84
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c88
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c8c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c90
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c94
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c98
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00c9c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ca0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ca8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00cac
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00cb4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00cbc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cc0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00cc4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00cc8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ccc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cd0
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00cd4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00cdc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ce0
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00ce4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00cec
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00cf4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00cfc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00d00
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d04
			 0x7fc00001,                                                  // signaling NaN                               /// 00d08
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d0c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d10
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d14
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d18
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d1c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d20
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d24
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d28
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d2c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d30
			 0x0e000001,                                                  // Trailing 1s:                                /// 00d34
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d38
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d3c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d40
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d44
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00d48
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d4c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d50
			 0x7f800000,                                                  // inf                                         /// 00d54
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d5c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00d60
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d64
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d68
			 0x4b000000,                                                  // 8388608.0                                   /// 00d6c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d70
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d74
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00d78
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d7c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d80
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d84
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00d88
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d8c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00d90
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00d94
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d98
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d9c
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00da4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00da8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00db0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00db4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00db8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dbc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00dc0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dc8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00dcc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dd4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00dd8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ddc
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00de4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00de8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00dec
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00df0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00df4
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00df8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00dfc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e00
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e08
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e0c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e10
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e14
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e18
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e1c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e20
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00e24
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e2c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00e30
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e34
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e38
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e3c
			 0x00000000,                                                  // zero                                        /// 00e40
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e44
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e48
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e50
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x0e000001,                                                  // Trailing 1s:                                /// 00e58
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00e5c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e64
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e68
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e6c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00e70
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e74
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00e78
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e7c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00e80
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e84
			 0xffc00001,                                                  // -signaling NaN                              /// 00e88
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00e8c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e90
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e98
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e9c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00ea0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ea4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ea8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00eac
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00eb4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00eb8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ebc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ec0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ec4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ec8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ecc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ed4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ed8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00edc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ee0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ee8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00eec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ef0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ef4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef8
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00efc
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00f00
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f04
			 0xcb000000,                                                  // -8388608.0                                  /// 00f08
			 0xffc00001,                                                  // -signaling NaN                              /// 00f0c
			 0x3f028f5c,                                                  // 0.51                                        /// 00f10
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f14
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f18
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f1c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f20
			 0x55555555,                                                  // 4 random values                             /// 00f24
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f28
			 0x7fc00001,                                                  // signaling NaN                               /// 00f2c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00f30
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f34
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f38
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f3c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f40
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f44
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f48
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f4c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f50
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f54
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f58
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00f5c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f60
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f64
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00f68
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00f6c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f70
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f74
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f78
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f7c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f80
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f84
			 0xff800000,                                                  // -inf                                        /// 00f88
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f8c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f90
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f94
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f98
			 0x80000000,                                                  // -zero                                       /// 00f9c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fa0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00fa8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00fac
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fb0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fb4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00fbc
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00fc4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fcc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00fd0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00fd4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00fdc
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fe4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fe8
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ff4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x00000040                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00004
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00008
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0000c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00014
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0001c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00020
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00024
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00028
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0002c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00030
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00034
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00038
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0003c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00040
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00044
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00048
			 0x0e000001,                                                  // Trailing 1s:                                /// 0004c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00050
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00054
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00058
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0005c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00060
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00064
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00068
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0006c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00070
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00074
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00078
			 0xbf028f5c,                                                  // -0.51                                       /// 0007c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00080
			 0x00000000,                                                  // zero                                        /// 00084
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00088
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0008c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00090
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00094
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00098
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0009c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a0
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000a4
			 0x80000000,                                                  // -zero                                       /// 000a8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000ac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000b0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 000b4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b8
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000bc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c0
			 0x3f028f5c,                                                  // 0.51                                        /// 000c4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 000c8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000cc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 000d0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 000d4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000dc
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 000e0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 000e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000e8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 000ec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 000f0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 000f8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 000fc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00104
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00108
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0010c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00110
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00114
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0011c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00120
			 0x0c700000,                                                  // Leading 1s:                                 /// 00124
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00128
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0012c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00134
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00138
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0013c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00140
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00144
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00148
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00150
			 0x33333333,                                                  // 4 random values                             /// 00154
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00158
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0015c
			 0x7f800000,                                                  // inf                                         /// 00160
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00164
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00168
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0016c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00170
			 0x7fc00001,                                                  // signaling NaN                               /// 00174
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00178
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00180
			 0x7f800000,                                                  // inf                                         /// 00184
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00188
			 0x80011111,                                                  // -9.7958E-41                                 /// 0018c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00190
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00194
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0019c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001a0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001a4
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001a8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 001ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 001b0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001b4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 001b8
			 0xCCCCCCCC,                                                  // 4 random values                             /// 001bc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001c0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 001c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 001c8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 001cc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 001d0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001d4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001d8
			 0x00011111,                                                  // 9.7958E-41                                  /// 001dc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001e0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 001e8
			 0x0c400000,                                                  // Leading 1s:                                 /// 001ec
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 001f0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001f4
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001fc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00204
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0020c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00210
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00214
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00218
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00220
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00224
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00228
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0022c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00230
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00234
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00238
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0023c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00240
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00244
			 0x00000000,                                                  // zero                                        /// 00248
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0024c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00250
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00254
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00258
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0025c
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00260
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00264
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00268
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0026c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00270
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00274
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00278
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0027c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00280
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00284
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00288
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0028c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00290
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00294
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00298
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0029c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 002a0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002a4
			 0x0c400000,                                                  // Leading 1s:                                 /// 002a8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 002ac
			 0x7fc00001,                                                  // signaling NaN                               /// 002b0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x00000000,                                                  // zero                                        /// 002bc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002c0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0xcb000000,                                                  // -8388608.0                                  /// 002c8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 002cc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002d0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 002d8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002dc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002e0
			 0xbf028f5c,                                                  // -0.51                                       /// 002e4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 002e8
			 0x55555555,                                                  // 4 random values                             /// 002ec
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 002f0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 002f4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002f8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00308
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0030c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00310
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00314
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00318
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0031c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00320
			 0x0c600000,                                                  // Leading 1s:                                 /// 00324
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00328
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00330
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00334
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0033c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00340
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00344
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00348
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0034c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00350
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00354
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00358
			 0x00000000,                                                  // zero                                        /// 0035c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00360
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00364
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00368
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0036c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00370
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00374
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00378
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0037c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00384
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00388
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0038c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00390
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00394
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0039c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 003a0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 003a4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003a8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003ac
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003b0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 003b4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 003b8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003bc
			 0x80011111,                                                  // -9.7958E-41                                 /// 003c0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003c4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003cc
			 0xcb000000,                                                  // -8388608.0                                  /// 003d0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 003d4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 003dc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 003e0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 003e4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 003e8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003ec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 003f0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 003f4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003f8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 003fc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00400
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00404
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 0040c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00410
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00414
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00418
			 0x0e000001,                                                  // Trailing 1s:                                /// 0041c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00428
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0042c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00430
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00434
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0043c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00440
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00444
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00448
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0044c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00450
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0x55555555,                                                  // 4 random values                             /// 00458
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0045c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00460
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00464
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00468
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0046c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00470
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00474
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x0e000003,                                                  // Trailing 1s:                                /// 0047c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00480
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00484
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00488
			 0x33333333,                                                  // 4 random values                             /// 0048c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00490
			 0x0c400000,                                                  // Leading 1s:                                 /// 00494
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00498
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0049c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004a0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 004a4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004a8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ac
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 004b0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 004b4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004b8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004bc
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 004c0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004c4
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 004c8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 004d0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004d4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004d8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 004dc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004e0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 004e8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 004ec
			 0x80000000, // 0                                             // SP - ve numbers                             /// 004f0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 004f4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 004fc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00500
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00504
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00508
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 0050c
			 0x7fc00001,                                                  // signaling NaN                               /// 00510
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00514
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00518
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0051c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00520
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00524
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00528
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0052c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00530
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00534
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00538
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 0053c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00540
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00544
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0054c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00550
			 0xcb000000,                                                  // -8388608.0                                  /// 00554
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00558
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0055c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00560
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00568
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0056c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00570
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00574
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00578
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0057c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00580
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00584
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00588
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0058c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00590
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00598
			 0x0e001fff,                                                  // Trailing 1s:                                /// 0059c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 005a0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005a4
			 0x0e000001,                                                  // Trailing 1s:                                /// 005a8
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 005b0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 005b4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 005b8
			 0x7fc00001,                                                  // signaling NaN                               /// 005bc
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 005c0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 005c4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005cc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005d0
			 0xcb000000,                                                  // -8388608.0                                  /// 005d4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 005d8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 005dc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 005e0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005e4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 005ec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 005f0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005f4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 005f8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 005fc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00600
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00604
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00608
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0060c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00614
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00618
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0061c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00620
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00624
			 0xff800000,                                                  // -inf                                        /// 00628
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0062c
			 0xff800000,                                                  // -inf                                        /// 00630
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00634
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00638
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0063c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00640
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00644
			 0x00000000,                                                  // zero                                        /// 00648
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0064c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00650
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00654
			 0x33333333,                                                  // 4 random values                             /// 00658
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0065c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00664
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00668
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0066c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00670
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00674
			 0x0e000003,                                                  // Trailing 1s:                                /// 00678
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0067c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00680
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00684
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00688
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0068c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00690
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00694
			 0x7fc00001,                                                  // signaling NaN                               /// 00698
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 0069c
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 006a8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 006ac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006b0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006b4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 006b8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 006bc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x0c600000,                                                  // Leading 1s:                                 /// 006c4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 006c8
			 0x0c600000,                                                  // Leading 1s:                                 /// 006cc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006d4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 006d8
			 0x55555555,                                                  // 4 random values                             /// 006dc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006e0
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 006e4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006e8
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006ec
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 006f8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006fc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00700
			 0xcb000000,                                                  // -8388608.0                                  /// 00704
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00708
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0070c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00710
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00714
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00718
			 0x0e000007,                                                  // Trailing 1s:                                /// 0071c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00720
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00724
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00728
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0072c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00730
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00734
			 0x7fc00001,                                                  // signaling NaN                               /// 00738
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0073c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00740
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00748
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00750
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00754
			 0x55555555,                                                  // 4 random values                             /// 00758
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0075c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00760
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00768
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0076c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00770
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00774
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00778
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0077c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00780
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x7fc00001,                                                  // signaling NaN                               /// 00788
			 0x00011111,                                                  // 9.7958E-41                                  /// 0078c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00790
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00794
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0079c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007a4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 007a8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007ac
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 007b0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007b4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 007b8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 007bc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 007c4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 007c8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 007cc
			 0xbf028f5c,                                                  // -0.51                                       /// 007d0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 007d4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 007d8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007dc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007e0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007e4
			 0x3f028f5c,                                                  // 0.51                                        /// 007e8
			 0x0c600000,                                                  // Leading 1s:                                 /// 007ec
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 007f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 007f4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 007f8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007fc
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00800
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00804
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0080c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00810
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00814
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00818
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0081c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00820
			 0x00000000,                                                  // zero                                        /// 00824
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0082c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00830
			 0x0c600000,                                                  // Leading 1s:                                 /// 00834
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00838
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0083c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00840
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x80011111,                                                  // -9.7958E-41                                 /// 00848
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0084c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00854
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00858
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0085c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00860
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00864
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00868
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0086c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00870
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00874
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00878
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 0087c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00884
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00888
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0088c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00890
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00894
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0089c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 008a4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 008ac
			 0x0e000007,                                                  // Trailing 1s:                                /// 008b0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 008bc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 008c4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008c8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008cc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008d0
			 0x0e000003,                                                  // Trailing 1s:                                /// 008d4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008d8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 008dc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 008e0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008e8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ec
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 008f0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008f8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 008fc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00900
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00904
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00908
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0090c
			 0x7fc00001,                                                  // signaling NaN                               /// 00910
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00914
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00918
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0091c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00920
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00924
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00928
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0092c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00930
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00934
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00938
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 0093c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00940
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00944
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00948
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0094c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00950
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00954
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00958
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0095c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00960
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00964
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00968
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0096c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00970
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00974
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00978
			 0x7fc00001,                                                  // signaling NaN                               /// 0097c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00984
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00988
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0098c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00990
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00994
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00998
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0099c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a4
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 009a8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009ac
			 0x80011111,                                                  // -9.7958E-41                                 /// 009b0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 009b8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 009bc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009c0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009c4
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 009c8
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 009cc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 009d0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009e0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009e4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009e8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 009ec
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009f0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009f4
			 0x0c400000,                                                  // Leading 1s:                                 /// 009f8
			 0x0e000001,                                                  // Trailing 1s:                                /// 009fc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00a00
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a04
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a08
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a0c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00a10
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00a14
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00a18
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a1c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a20
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a24
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00a28
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a2c
			 0xffc00001,                                                  // -signaling NaN                              /// 00a30
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a34
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00a3c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a40
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00a48
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a4c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a50
			 0x0e000003,                                                  // Trailing 1s:                                /// 00a54
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a58
			 0x55555555,                                                  // 4 random values                             /// 00a5c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a60
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00a68
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a6c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00a70
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a74
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a78
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a7c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a80
			 0xffc00001,                                                  // -signaling NaN                              /// 00a84
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a88
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00a8c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00a90
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a94
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00a98
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a9c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00aa0
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00aa4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00aa8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ab4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00abc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ac0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ac4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00ac8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00acc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ad0
			 0xbf028f5c,                                                  // -0.51                                       /// 00ad4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00ad8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0xff800000,                                                  // -inf                                        /// 00ae0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ae8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00aec
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00af0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00af4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00afc
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00b00
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00b04
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b08
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b0c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b10
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b14
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b18
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b20
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b28
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x55555555,                                                  // 4 random values                             /// 00b30
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b34
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b38
			 0xcb000000,                                                  // -8388608.0                                  /// 00b3c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b44
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b48
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00b4c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b50
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b54
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b58
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b5c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00b60
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b64
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00b68
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b70
			 0x33333333,                                                  // 4 random values                             /// 00b74
			 0x0c600000,                                                  // Leading 1s:                                 /// 00b78
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b7c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b80
			 0x3f028f5c,                                                  // 0.51                                        /// 00b84
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00b88
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b8c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00b90
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b94
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00b98
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b9c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ba4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00ba8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bb0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00bb4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00bb8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00bbc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bc0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bc4
			 0x7fc00001,                                                  // signaling NaN                               /// 00bc8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bcc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00bd0
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00bd4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00bd8
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00be0
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00be4
			 0x33333333,                                                  // 4 random values                             /// 00be8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00bf0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bf4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00bf8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bfc
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c00
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c04
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00c08
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c10
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00c14
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c18
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c1c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c20
			 0x00011111,                                                  // 9.7958E-41                                  /// 00c24
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c28
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c2c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00c30
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c34
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c38
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c3c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c40
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c44
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00c48
			 0x80011111,                                                  // -9.7958E-41                                 /// 00c4c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c50
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00c58
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c5c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c60
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c64
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00c68
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00c6c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c70
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c74
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c78
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c7c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c80
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c84
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c88
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00c8c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c90
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c94
			 0x80000000,                                                  // -zero                                       /// 00c98
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c9c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ca0
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00ca4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ca8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cbc
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00cc0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cc4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cc8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ccc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cd0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00cd4
			 0x00000000,                                                  // zero                                        /// 00cd8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00cdc
			 0x80000000,                                                  // -zero                                       /// 00ce0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ce4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00ce8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00cec
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00cf0
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00cf8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cfc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d00
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00d08
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00d10
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d14
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d1c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d20
			 0x0c400000,                                                  // Leading 1s:                                 /// 00d24
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d28
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d30
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00d34
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d38
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00d3c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d40
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d44
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d48
			 0xcb000000,                                                  // -8388608.0                                  /// 00d4c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d50
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d54
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00d58
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00d5c
			 0xbf028f5c,                                                  // -0.51                                       /// 00d60
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d64
			 0x80000000,                                                  // -zero                                       /// 00d68
			 0x0e000007,                                                  // Trailing 1s:                                /// 00d6c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00d70
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d74
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d78
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d7c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d80
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d84
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00d88
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00d8c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0xbf028f5c,                                                  // -0.51                                       /// 00d94
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d98
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00da4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00da8
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00db0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00db4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00db8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00dbc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00dc0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00dc4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dc8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dcc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00dd0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00dd4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00dd8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ddc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00de0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00de4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00de8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00df0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00df4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00dfc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00e00
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00e04
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e0c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e10
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e14
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e18
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00e1c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e20
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e24
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e28
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00e2c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e30
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e34
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e40
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e44
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e48
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e4c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e50
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00e54
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e58
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e5c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e60
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00e64
			 0x3f028f5c,                                                  // 0.51                                        /// 00e68
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00e70
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e78
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e7c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00e80
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e84
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e88
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e8c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e90
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e94
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e98
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00e9c
			 0xff800000,                                                  // -inf                                        /// 00ea0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ea8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00eac
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00eb0
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00eb4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00eb8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ebc
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00ec0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00ec8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ecc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00ed8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00edc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00ee0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ee4
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ee8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00eec
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00ef0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ef4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00ef8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00efc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f00
			 0x0e000003,                                                  // Trailing 1s:                                /// 00f04
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f08
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f0c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f10
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f14
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f18
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f20
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f24
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f28
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00f2c
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00f30
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00f34
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f38
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f3c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f40
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f44
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f48
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00f4c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f50
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f54
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f58
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00f5c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00f60
			 0xbf028f5c,                                                  // -0.51                                       /// 00f64
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f68
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00f6c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f74
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f78
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f7c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f80
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00f84
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f88
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f8c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00f90
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f94
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f98
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f9c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fa0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00fa4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fa8
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fac
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fb0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fb4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fb8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fbc
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00fc0
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00fc4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00fc8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fcc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00fd0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x3f028f5c,                                                  // 0.51                                        /// 00fd8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fdc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fe0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00fe4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00fe8
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ff0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ff4
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ff8
			 0x00000001                                                  // SP - 1 bit alone set in mantissa +ve        /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00000
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00004
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00008
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0000c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00010
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00014
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00018
			 0x7f800000,                                                  // inf                                         /// 0001c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00020
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00024
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00028
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0002c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00030
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00034
			 0x0e000003,                                                  // Trailing 1s:                                /// 00038
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0003c
			 0x33333333,                                                  // 4 random values                             /// 00040
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00044
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00048
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 0004c
			 0xcb000000,                                                  // -8388608.0                                  /// 00050
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00054
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00058
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0005c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00060
			 0x0c780000,                                                  // Leading 1s:                                 /// 00064
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00068
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 0006c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00070
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00074
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00078
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0007c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00080
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00084
			 0x0e000003,                                                  // Trailing 1s:                                /// 00088
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0008c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00090
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00094
			 0x55555555,                                                  // 4 random values                             /// 00098
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0009c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000a0
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000a4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 000a8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000ac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 000b0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 000b8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 000bc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 000c0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000c4
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000c8
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 000cc
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000d4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 000d8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 000dc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 000e8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000ec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 000f0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000f4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000f8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000fc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00100
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00104
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00108
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0010c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00110
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00114
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00118
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0011c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00120
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00128
			 0xff800000,                                                  // -inf                                        /// 0012c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00130
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00134
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00138
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0013c
			 0x80000000,                                                  // -zero                                       /// 00140
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00144
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00148
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0014c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00150
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00154
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00158
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0015c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00160
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00164
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00168
			 0x00000000,                                                  // zero                                        /// 0016c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00170
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00174
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00178
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0017c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00180
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00184
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00188
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00190
			 0x33333333,                                                  // 4 random values                             /// 00194
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00198
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0019c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 001a0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 001a4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 001a8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001ac
			 0x7fc00001,                                                  // signaling NaN                               /// 001b0
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001b8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 001bc
			 0x0e000003,                                                  // Trailing 1s:                                /// 001c0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001cc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001d4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 001d8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 001e0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 001e8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001ec
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 001f0
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001f8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 001fc
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00200
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00204
			 0xcb000000,                                                  // -8388608.0                                  /// 00208
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0020c
			 0x7fc00001,                                                  // signaling NaN                               /// 00210
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00214
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00218
			 0x00000000, // 0                                             // SP +ve numbers                              /// 0021c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00220
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00224
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00228
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0022c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00230
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00234
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00238
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0023c
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00240
			 0x0c400000,                                                  // Leading 1s:                                 /// 00244
			 0x3f028f5c,                                                  // 0.51                                        /// 00248
			 0x00011111,                                                  // 9.7958E-41                                  /// 0024c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00250
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00254
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00258
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00260
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00264
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00268
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0026c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00270
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00274
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00278
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0027c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00280
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00284
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00288
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0028c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00290
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00298
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0029c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x00011111,                                                  // 9.7958E-41                                  /// 002a4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 002a8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002b0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 002b4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002b8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002bc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 002c0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002c4
			 0x0c600000,                                                  // Leading 1s:                                 /// 002c8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 002cc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 002d0
			 0x0c700000,                                                  // Leading 1s:                                 /// 002d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002dc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002e4
			 0x0e001fff,                                                  // Trailing 1s:                                /// 002e8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 002ec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 002f4
			 0x3f028f5c,                                                  // 0.51                                        /// 002f8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002fc
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00300
			 0x7f800000,                                                  // inf                                         /// 00304
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00308
			 0xffc00001,                                                  // -signaling NaN                              /// 0030c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00310
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00314
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00318
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0031c
			 0xffc00001,                                                  // -signaling NaN                              /// 00320
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00324
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00328
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0032c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00330
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00334
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00338
			 0x80000000,                                                  // -zero                                       /// 0033c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00340
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00344
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00348
			 0xbf028f5c,                                                  // -0.51                                       /// 0034c
			 0xcb000000,                                                  // -8388608.0                                  /// 00350
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00354
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00358
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0035c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00360
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00364
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00368
			 0x7f800000,                                                  // inf                                         /// 0036c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00370
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00374
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00378
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0037c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00380
			 0x7f800000,                                                  // inf                                         /// 00384
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00388
			 0xffc00001,                                                  // -signaling NaN                              /// 0038c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00390
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00394
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00398
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0039c
			 0x0c400000,                                                  // Leading 1s:                                 /// 003a0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 003a4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x0c400000,                                                  // Leading 1s:                                 /// 003ac
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 003b0
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 003b4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003b8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003c0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 003c4
			 0x0c400000,                                                  // Leading 1s:                                 /// 003c8
			 0xff800000,                                                  // -inf                                        /// 003cc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003d0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003d4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003d8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 003dc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003e0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003e8
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00400
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00404
			 0x3f028f5c,                                                  // 0.51                                        /// 00408
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0040c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00410
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00414
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00418
			 0x0e000001,                                                  // Trailing 1s:                                /// 0041c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00420
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00424
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0042c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00430
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00434
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00438
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0043c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00440
			 0xff800000,                                                  // -inf                                        /// 00444
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00448
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0044c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00450
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00454
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00458
			 0xCCCCCCCC,                                                  // 4 random values                             /// 0045c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00460
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00464
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00468
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0046c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00470
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00478
			 0x0e000001,                                                  // Trailing 1s:                                /// 0047c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00480
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00484
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00488
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00490
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00494
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00498
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0049c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 004a0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004a4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 004a8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004ac
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 004b0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004b4
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 004b8
			 0x0e001fff,                                                  // Trailing 1s:                                /// 004bc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004c4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 004c8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x4b000000,                                                  // 8388608.0                                   /// 004d0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 004d4
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004dc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 004e0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 004e4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004e8
			 0x7fc00001,                                                  // signaling NaN                               /// 004ec
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004f0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 004f4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004fc
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00500
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00504
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00508
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0050c
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00510
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00514
			 0x0c400000,                                                  // Leading 1s:                                 /// 00518
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0051c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00520
			 0x0c600000,                                                  // Leading 1s:                                 /// 00524
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0052c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00530
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00534
			 0x7f800000,                                                  // inf                                         /// 00538
			 0x00000000,                                                  // zero                                        /// 0053c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00540
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00544
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00548
			 0x00011111,                                                  // 9.7958E-41                                  /// 0054c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00558
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0055c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00560
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00564
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00568
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0056c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x0c700000,                                                  // Leading 1s:                                 /// 00574
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00578
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0057c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00580
			 0x33333333,                                                  // 4 random values                             /// 00584
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00588
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0058c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00590
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00594
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00598
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0059c
			 0x0e000007,                                                  // Trailing 1s:                                /// 005a0
			 0xff800000,                                                  // -inf                                        /// 005a4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 005a8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ac
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 005b0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 005b4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005b8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005c0
			 0x7f800000,                                                  // inf                                         /// 005c4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 005d0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005d8
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005dc
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 005e0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 005e4
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005ec
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005f0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005f4
			 0x55555555,                                                  // 4 random values                             /// 005f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005fc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00600
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00604
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00608
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0060c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00610
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00614
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00618
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0061c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00620
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00624
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00628
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 0062c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00630
			 0x55555555,                                                  // 4 random values                             /// 00634
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0063c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00640
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00644
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00648
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0064c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00650
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00654
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00658
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0065c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00660
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00664
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00668
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0066c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00670
			 0x33333333,                                                  // 4 random values                             /// 00674
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00678
			 0x0e000007,                                                  // Trailing 1s:                                /// 0067c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00680
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00684
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00688
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00690
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00694
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00698
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0069c
			 0x00000000,                                                  // zero                                        /// 006a0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 006a4
			 0x0c400000,                                                  // Leading 1s:                                 /// 006a8
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006ac
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006b0
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006b4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b8
			 0x0c600000,                                                  // Leading 1s:                                 /// 006bc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006c8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 006cc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006d0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 006d4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 006d8
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006dc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 006e0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006e4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 006ec
			 0x7f800000,                                                  // inf                                         /// 006f0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 006f4
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006f8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 006fc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00700
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00704
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00708
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0070c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00710
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00714
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00720
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00724
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00728
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0072c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00730
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00734
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00738
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 0073c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00740
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00744
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00748
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0074c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00750
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00754
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00758
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00760
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00768
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0076c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00770
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00778
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0077c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00780
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00784
			 0x55555555,                                                  // 4 random values                             /// 00788
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0078c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00790
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00798
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0079c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 007a0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 007a4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007a8
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 007ac
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007b0
			 0x7f800000,                                                  // inf                                         /// 007b4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007b8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007bc
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007c0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 007c4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007c8
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007cc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 007d0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007d4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 007d8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 007e0
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007e8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 007ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007f0
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 007f4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007f8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 007fc
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00800
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00804
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00808
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0080c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00810
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00814
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00818
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0081c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00820
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00824
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00828
			 0xffc00000,                                                  // -cquiet NaN                                 /// 0082c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00830
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00834
			 0x4b000000,                                                  // 8388608.0                                   /// 00838
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0083c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00844
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00848
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0084c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00850
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00854
			 0x55555555,                                                  // 4 random values                             /// 00858
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 0085c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00860
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00864
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00868
			 0x0e000007,                                                  // Trailing 1s:                                /// 0086c
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00870
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00874
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00878
			 0x0e000007,                                                  // Trailing 1s:                                /// 0087c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00880
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00884
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00888
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0088c
			 0x80000000,                                                  // -zero                                       /// 00890
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00894
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 008a4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 008a8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ac
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 008b0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 008b4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008bc
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008c0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 008c4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x0d000ff0,                                                  // Set of 1s                                   /// 008cc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 008d0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008d4
			 0x0e000003,                                                  // Trailing 1s:                                /// 008d8
			 0x00000000,                                                  // zero                                        /// 008dc
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 008e0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 008e4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 008e8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008ec
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008f0
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008fc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00900
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00904
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00908
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0090c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00910
			 0x0c780000,                                                  // Leading 1s:                                 /// 00914
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00918
			 0x0e000007,                                                  // Trailing 1s:                                /// 0091c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00920
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00924
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00928
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0092c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00930
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00934
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00938
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0093c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00940
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0xcb000000,                                                  // -8388608.0                                  /// 00948
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 0094c
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00950
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00954
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00958
			 0x4b000000,                                                  // 8388608.0                                   /// 0095c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00960
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00964
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00968
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0096c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00970
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00974
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00978
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0097c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00980
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00988
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0098c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00990
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00994
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00998
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0099c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 009a0
			 0xff800000,                                                  // -inf                                        /// 009a4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 009ac
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 009b0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 009b4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009b8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009bc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009c0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009c8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009cc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009d0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 009d4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 009dc
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009e4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 009f0
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009f4
			 0x0e000001,                                                  // Trailing 1s:                                /// 009f8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 009fc
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00a00
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00a04
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a08
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a0c
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00a10
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a14
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00a18
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a20
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a24
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00a28
			 0x7fc00001,                                                  // signaling NaN                               /// 00a2c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a30
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a34
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a38
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00a3c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00a40
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00a44
			 0x7f800000,                                                  // inf                                         /// 00a48
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00a4c
			 0x80000000,                                                  // -zero                                       /// 00a50
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00a54
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00a58
			 0x3f028f5c,                                                  // 0.51                                        /// 00a5c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a60
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a64
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00a68
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00a6c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a70
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00a78
			 0x0c400000,                                                  // Leading 1s:                                 /// 00a7c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a80
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00a84
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00a88
			 0x0c780000,                                                  // Leading 1s:                                 /// 00a8c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00a90
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a94
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00a9c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00aa4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aa8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00aac
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ab0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ab4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00abc
			 0x55555555,                                                  // 4 random values                             /// 00ac0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ac4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ac8
			 0xff800000,                                                  // -inf                                        /// 00acc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ad0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00ad4
			 0xbf028f5c,                                                  // -0.51                                       /// 00ad8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00adc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ae8
			 0xff800000,                                                  // -inf                                        /// 00aec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00af0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af4
			 0x00000000,                                                  // zero                                        /// 00af8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00afc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b00
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00b04
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b08
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b0c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b10
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b14
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00b18
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b1c
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b20
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00b24
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b28
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b30
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00b34
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b38
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b3c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00b40
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b44
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00b48
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b4c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00b50
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b54
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b58
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b5c
			 0x0e000007,                                                  // Trailing 1s:                                /// 00b60
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b64
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00b68
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b6c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b70
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b74
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b78
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b7c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00b80
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b84
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b88
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b90
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00b94
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b98
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b9c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00ba0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ba4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00ba8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00bac
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bb0
			 0x0c780000,                                                  // Leading 1s:                                 /// 00bb4
			 0x00000000,                                                  // zero                                        /// 00bb8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bbc
			 0x55555555,                                                  // 4 random values                             /// 00bc0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00bc4
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00bc8
			 0x0c600000,                                                  // Leading 1s:                                 /// 00bcc
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bd4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00bd8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bdc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00be0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00be4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00bec
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00bf0
			 0x33333333,                                                  // 4 random values                             /// 00bf4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bf8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00bfc
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c00
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c04
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c08
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c0c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c10
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c14
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c18
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c1c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00c20
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x55555555,                                                  // 4 random values                             /// 00c2c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c30
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c34
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00c38
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c3c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00c40
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c44
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c48
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c4c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00c50
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c58
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c5c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x0c400000,                                                  // Leading 1s:                                 /// 00c64
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c68
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c6c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00c70
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00c74
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c78
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c7c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00c80
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c84
			 0xff800000,                                                  // -inf                                        /// 00c88
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c8c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00c90
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c94
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00c98
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c9c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ca4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00ca8
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00cac
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00cb0
			 0xcb000000,                                                  // -8388608.0                                  /// 00cb4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cbc
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cc4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00cc8
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ccc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cd0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cd8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00cdc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ce0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ce4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00cec
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00cf0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00cf4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00cf8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cfc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d00
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d04
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00d08
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d0c
			 0x7fc00001,                                                  // signaling NaN                               /// 00d10
			 0x33333333,                                                  // 4 random values                             /// 00d14
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d18
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d1c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d20
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d24
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d28
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d2c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00d30
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d38
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00d3c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d40
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d48
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d4c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d50
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d54
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00d58
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d5c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d60
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00d64
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d68
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d70
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d74
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d78
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d80
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d84
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d88
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d8c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00d90
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d94
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d98
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d9c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00da0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00da4
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00da8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dac
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00db0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00db4
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00db8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00dbc
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00dc0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00dc4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00dc8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00dcc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00dd0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00dd4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ddc
			 0x0c400000,                                                  // Leading 1s:                                 /// 00de0
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00de8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00dec
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00df4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00dfc
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e00
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e04
			 0x7f800000,                                                  // inf                                         /// 00e08
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e0c
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e10
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e14
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e18
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e20
			 0xff800000,                                                  // -inf                                        /// 00e24
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00e28
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00e2c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00e30
			 0x55555555,                                                  // 4 random values                             /// 00e34
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e38
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e3c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e40
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e4c
			 0x7fc00001,                                                  // signaling NaN                               /// 00e50
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e54
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e58
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e5c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e60
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e64
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e68
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e6c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e70
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e74
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e78
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00e7c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e80
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00e84
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e88
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00e8c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00e90
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00e94
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e98
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e9c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ea0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ea4
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00ea8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00eac
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00eb0
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00eb4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00ebc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ec0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00ec4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00ec8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00ecc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ed0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ed4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ed8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00edc
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ee0
			 0xffc00001,                                                  // -signaling NaN                              /// 00ee4
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00ee8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00eec
			 0xcb000000,                                                  // -8388608.0                                  /// 00ef0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00ef4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ef8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00efc
			 0x55555555,                                                  // 4 random values                             /// 00f00
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f04
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00f08
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f0c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f10
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f14
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f18
			 0x3f028f5c,                                                  // 0.51                                        /// 00f1c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f20
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f24
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f28
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00f2c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00f30
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00f34
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f38
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00f3c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00f44
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f48
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f4c
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f50
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00f54
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00f58
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f5c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00f60
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f64
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f68
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f6c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f70
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f74
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f78
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f7c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00f80
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f84
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f88
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f8c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f90
			 0xff800000,                                                  // -inf                                        /// 00f94
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f98
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f9c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00fa0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fa4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00fa8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00fac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb0
			 0x7f800000,                                                  // inf                                         /// 00fb4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00fb8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00fbc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00fc0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fc4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00fc8
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fcc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fd0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00fd8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00fdc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00fe4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00fe8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fec
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ff4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x00800000 // min norm                                      // SP +ve numbers                              /// last
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
			 0x0000054c,
			 0x000004b8,
			 0x000000f0,
			 0x0000011c,
			 0x00000568,
			 0x0000058c,
			 0x000000e4,
			 0x0000020c,

			 /// vpu register f2
			 0x41600000,
			 0x40000000,
			 0x41500000,
			 0x41100000,
			 0x41d80000,
			 0x41900000,
			 0x42000000,
			 0x42000000,

			 /// vpu register f3
			 0x40000000,
			 0x41f00000,
			 0x41200000,
			 0x40000000,
			 0x41100000,
			 0x40800000,
			 0x40400000,
			 0x41f80000,

			 /// vpu register f4
			 0x41c80000,
			 0x41b80000,
			 0x41b80000,
			 0x41c80000,
			 0x41400000,
			 0x41980000,
			 0x40400000,
			 0x41980000,

			 /// vpu register f5
			 0x41600000,
			 0x41e00000,
			 0x41200000,
			 0x41e00000,
			 0x40000000,
			 0x41900000,
			 0x40c00000,
			 0x41e80000,

			 /// vpu register f6
			 0x40800000,
			 0x41b80000,
			 0x41e80000,
			 0x41300000,
			 0x41b00000,
			 0x41700000,
			 0x3f800000,
			 0x41700000,

			 /// vpu register f7
			 0x41100000,
			 0x3f800000,
			 0x41980000,
			 0x41900000,
			 0x40a00000,
			 0x41b80000,
			 0x40800000,
			 0x41f80000,

			 /// vpu register f8
			 0x41b00000,
			 0x3f800000,
			 0x41e00000,
			 0x40800000,
			 0x41880000,
			 0x41400000,
			 0x41980000,
			 0x40800000,

			 /// vpu register f9
			 0x40000000,
			 0x40e00000,
			 0x41c00000,
			 0x40a00000,
			 0x40800000,
			 0x42000000,
			 0x41a00000,
			 0x41e80000,

			 /// vpu register f10
			 0x40000000,
			 0x40000000,
			 0x41700000,
			 0x41e00000,
			 0x41b00000,
			 0x41d00000,
			 0x41500000,
			 0x41d80000,

			 /// vpu register f11
			 0x41d00000,
			 0x40400000,
			 0x40000000,
			 0x41a00000,
			 0x41e00000,
			 0x41b80000,
			 0x40c00000,
			 0x41700000,

			 /// vpu register f12
			 0x41d00000,
			 0x41c80000,
			 0x41f80000,
			 0x40800000,
			 0x41980000,
			 0x41a00000,
			 0x42000000,
			 0x41f80000,

			 /// vpu register f13
			 0x40800000,
			 0x41700000,
			 0x41d00000,
			 0x40e00000,
			 0x40400000,
			 0x41f80000,
			 0x41880000,
			 0x42000000,

			 /// vpu register f14
			 0x40000000,
			 0x42000000,
			 0x41980000,
			 0x40c00000,
			 0x40000000,
			 0x41f80000,
			 0x40c00000,
			 0x41e80000,

			 /// vpu register f15
			 0x41100000,
			 0x41d00000,
			 0x40000000,
			 0x3f800000,
			 0x41b80000,
			 0x42000000,
			 0x41a00000,
			 0x41d00000,

			 /// vpu register f16
			 0x41980000,
			 0x41f00000,
			 0x41b00000,
			 0x41b00000,
			 0x41200000,
			 0x41800000,
			 0x40e00000,
			 0x41f00000,

			 /// vpu register f17
			 0x41300000,
			 0x41c80000,
			 0x40000000,
			 0x41a00000,
			 0x41f00000,
			 0x41c00000,
			 0x41600000,
			 0x41880000,

			 /// vpu register f18
			 0x41b00000,
			 0x41500000,
			 0x41400000,
			 0x41400000,
			 0x41d00000,
			 0x41a00000,
			 0x41900000,
			 0x41c80000,

			 /// vpu register f19
			 0x41000000,
			 0x40800000,
			 0x41b00000,
			 0x41f80000,
			 0x41b00000,
			 0x41500000,
			 0x40a00000,
			 0x41a80000,

			 /// vpu register f20
			 0x40a00000,
			 0x41600000,
			 0x41300000,
			 0x41e00000,
			 0x41600000,
			 0x41980000,
			 0x41900000,
			 0x40400000,

			 /// vpu register f21
			 0x40c00000,
			 0x41d00000,
			 0x41000000,
			 0x41800000,
			 0x41b80000,
			 0x41100000,
			 0x40400000,
			 0x41b00000,

			 /// vpu register f22
			 0x41300000,
			 0x41800000,
			 0x41d00000,
			 0x41900000,
			 0x41d00000,
			 0x41400000,
			 0x41a00000,
			 0x41300000,

			 /// vpu register f23
			 0x41f00000,
			 0x41a80000,
			 0x3f800000,
			 0x40800000,
			 0x41e00000,
			 0x41700000,
			 0x41600000,
			 0x40800000,

			 /// vpu register f24
			 0x41d80000,
			 0x41700000,
			 0x41a00000,
			 0x41e00000,
			 0x41b00000,
			 0x40c00000,
			 0x40800000,
			 0x41200000,

			 /// vpu register f25
			 0x40e00000,
			 0x41f80000,
			 0x41880000,
			 0x41900000,
			 0x40800000,
			 0x41f80000,
			 0x40c00000,
			 0x41300000,

			 /// vpu register f26
			 0x41a00000,
			 0x41400000,
			 0x3f800000,
			 0x41c80000,
			 0x41b80000,
			 0x41d00000,
			 0x40e00000,
			 0x41c00000,

			 /// vpu register f27
			 0x3f800000,
			 0x40c00000,
			 0x41300000,
			 0x41c00000,
			 0x41300000,
			 0x41900000,
			 0x41100000,
			 0x40c00000,

			 /// vpu register f28
			 0x41880000,
			 0x41f00000,
			 0x41b80000,
			 0x40800000,
			 0x41f00000,
			 0x41c80000,
			 0x41800000,
			 0x40e00000,

			 /// vpu register f29
			 0x40a00000,
			 0x41900000,
			 0x41d00000,
			 0x41500000,
			 0x41b00000,
			 0x41900000,
			 0x41b00000,
			 0x41c80000,

			 /// vpu register f30
			 0x41f00000,
			 0x41400000,
			 0x41200000,
			 0x41e80000,
			 0x41400000,
			 0x41c00000,
			 0x41200000,
			 0x41200000,

			 /// vpu register f31
			 0x41e00000,
			 0x41800000,
			 0x3f800000,
			 0x41b00000,
			 0x41b00000,
			 0x41800000,
			 0x40c00000,
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
		"fnot.pi f10, f29\n"                                  ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f22, f24\n"                                  ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f14, f27\n"                                  ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f8, f21\n"                                   ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f19, f11\n"                                  ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f23, f21\n"                                  ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f12, f25\n"                                  ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f3, f12\n"                                   ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f3, f11\n"                                   ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f24, f15\n"                                  ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f14, f19\n"                                  ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f23, f28\n"                                  ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f18, f7\n"                                   ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f7, f3\n"                                    ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f2, f21\n"                                   ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f19, f23\n"                                  ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f3, f22\n"                                   ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f23, f12\n"                                  ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f2, f0\n"                                    ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f6, f12\n"                                   ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f10, f13\n"                                  ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f3, f17\n"                                   ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f6, f21\n"                                   ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f6, f9\n"                                    ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f25, f12\n"                                  ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f23, f28\n"                                  ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f23, f29\n"                                  ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f25, f29\n"                                  ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f5, f10\n"                                   ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f18, f27\n"                                  ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f10, f3\n"                                   ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f26, f20\n"                                  ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f2, f16\n"                                   ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f23, f6\n"                                   ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f23, f6\n"                                   ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f7, f30\n"                                   ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f28, f13\n"                                  ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f27, f8\n"                                   ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f9, f26\n"                                   ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f14, f23\n"                                  ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f10, f1\n"                                   ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f26, f0\n"                                   ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f10, f14\n"                                  ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f2, f1\n"                                    ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f7, f2\n"                                    ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f23, f21\n"                                  ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f4, f14\n"                                   ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f21, f16\n"                                  ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f5, f26\n"                                   ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f13, f4\n"                                   ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f13, f29\n"                                  ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f28, f23\n"                                  ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f13, f11\n"                                  ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f29, f1\n"                                   ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f10, f0\n"                                   ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f3, f12\n"                                   ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f29, f27\n"                                  ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f17, f8\n"                                   ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f5, f12\n"                                   ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f5, f8\n"                                    ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f12, f16\n"                                  ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f29, f17\n"                                  ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f21, f13\n"                                  ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f26, f2\n"                                   ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f8, f25\n"                                   ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f9, f3\n"                                    ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f13, f16\n"                                  ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f9, f10\n"                                   ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f23, f10\n"                                  ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f30, f26\n"                                  ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f15, f16\n"                                  ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f6, f30\n"                                   ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f13, f19\n"                                  ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f23, f26\n"                                  ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f19, f30\n"                                  ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f24, f23\n"                                  ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f3, f23\n"                                   ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f24, f17\n"                                  ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f8, f20\n"                                   ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f22, f26\n"                                  ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f20, f7\n"                                   ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f11, f25\n"                                  ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f21, f5\n"                                   ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f27, f26\n"                                  ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f12, f3\n"                                   ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f7, f3\n"                                    ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f23, f28\n"                                  ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f30, f21\n"                                  ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f21, f18\n"                                  ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f18, f28\n"                                  ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f3, f22\n"                                   ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f11, f16\n"                                  ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f5, f7\n"                                    ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f29, f17\n"                                  ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f25, f13\n"                                  ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f5, f3\n"                                    ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f6, f24\n"                                   ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f8, f15\n"                                   ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"fnot.pi f3, f30\n"                                   ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
