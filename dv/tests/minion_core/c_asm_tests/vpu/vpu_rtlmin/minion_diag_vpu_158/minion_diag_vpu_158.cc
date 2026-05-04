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
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00000
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00004
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00008
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 0000c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00010
			 0xffc00001,                                                  // -signaling NaN                              /// 00014
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0001c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00020
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00024
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00028
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00030
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00034
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00038
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0003c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00040
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00044
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00048
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0004c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00050
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00054
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00058
			 0x0c700000,                                                  // Leading 1s:                                 /// 0005c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00060
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00064
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00068
			 0x4b000000,                                                  // 8388608.0                                   /// 0006c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00070
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00074
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00078
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0007c
			 0x3f028f5c,                                                  // 0.51                                        /// 00080
			 0xcb000000,                                                  // -8388608.0                                  /// 00084
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00088
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0008c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00090
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00094
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00098
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0009c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 000a0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 000a4
			 0x00000000,                                                  // zero                                        /// 000a8
			 0x33333333,                                                  // 4 random values                             /// 000ac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 000b0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 000b4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 000b8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 000bc
			 0x0e000001,                                                  // Trailing 1s:                                /// 000c0
			 0x7fc00001,                                                  // signaling NaN                               /// 000c4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000c8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 000cc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000d0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 000d4
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000d8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000dc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 000e4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f0
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 000f4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x0e000001,                                                  // Trailing 1s:                                /// 000fc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00100
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00104
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00108
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0010c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00110
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00114
			 0x00011111,                                                  // 9.7958E-41                                  /// 00118
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0011c
			 0x3f028f5c,                                                  // 0.51                                        /// 00120
			 0x00000000,                                                  // zero                                        /// 00124
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00128
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0012c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00130
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00134
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00138
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0013c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00140
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00144
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00148
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0014c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00150
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00154
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00158
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0015c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00160
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00164
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00168
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0016c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00174
			 0x0e000003,                                                  // Trailing 1s:                                /// 00178
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0017c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00180
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00184
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00188
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0018c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00194
			 0x0e000003,                                                  // Trailing 1s:                                /// 00198
			 0x0e000003,                                                  // Trailing 1s:                                /// 0019c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001a0
			 0x0e000003,                                                  // Trailing 1s:                                /// 001a4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 001ac
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001b0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 001b8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 001bc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 001c0
			 0x0c600000,                                                  // Leading 1s:                                 /// 001c4
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 001c8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 001cc
			 0x0e000007,                                                  // Trailing 1s:                                /// 001d0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001d4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 001d8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 001dc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001e0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001e4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 001e8
			 0x0c600000,                                                  // Leading 1s:                                 /// 001ec
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001f0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001f4
			 0xffc00001,                                                  // -signaling NaN                              /// 001f8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 001fc
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00200
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00204
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00208
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0020c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00210
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00214
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00218
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0021c
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00220
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00224
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00230
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00234
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00238
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0023c
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00240
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00244
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00248
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0024c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00250
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00254
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00258
			 0x0e000007,                                                  // Trailing 1s:                                /// 0025c
			 0x7fc00001,                                                  // signaling NaN                               /// 00260
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00264
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00268
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0026c
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00270
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00274
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0027c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00280
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00284
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00288
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0028c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00290
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00294
			 0x0c700000,                                                  // Leading 1s:                                 /// 00298
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 0029c
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002a4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 002ac
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002b4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002b8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002bc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 002c0
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 002c4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002cc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002d0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002d4
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 002d8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 002dc
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 002e0
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 002e8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 002ec
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002f0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00300
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00304
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00308
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0030c
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00310
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00314
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00318
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0031c
			 0x33333333,                                                  // 4 random values                             /// 00320
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00324
			 0x80011111,                                                  // -9.7958E-41                                 /// 00328
			 0x0e000007,                                                  // Trailing 1s:                                /// 0032c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00330
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00334
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00338
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0033c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00340
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00344
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00348
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0034c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00350
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00354
			 0x80000000,                                                  // -zero                                       /// 00358
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 0035c
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00360
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00364
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00368
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0036c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00370
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00374
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00378
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0037c
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00380
			 0x3f028f5c,                                                  // 0.51                                        /// 00384
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00388
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0038c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00390
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00394
			 0xffc00001,                                                  // -signaling NaN                              /// 00398
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0039c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 003a0
			 0x0d00fff0,                                                  // Set of 1s                                   /// 003a4
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 003a8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0x7fc00001,                                                  // signaling NaN                               /// 003b0
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003b4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 003b8
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003bc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 003c0
			 0xff800000,                                                  // -inf                                        /// 003c4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 003c8
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 003d0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 003d4
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003d8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003dc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003e0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 003e4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 003e8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ec
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 003f0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 003f4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 003f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003fc
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00400
			 0x0c780000,                                                  // Leading 1s:                                 /// 00404
			 0x00000000,                                                  // zero                                        /// 00408
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0040c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00410
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00414
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00418
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0041c
			 0xffc00001,                                                  // -signaling NaN                              /// 00420
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00424
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00428
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0042c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00430
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00434
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00438
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0043c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00440
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00444
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00448
			 0x0e007fff,                                                  // Trailing 1s:                                /// 0044c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00450
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00454
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00458
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0045c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00460
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00464
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00468
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0046c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00470
			 0x55555555,                                                  // 4 random values                             /// 00474
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00478
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0047c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00480
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00484
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00488
			 0xff800000,                                                  // -inf                                        /// 0048c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00490
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00494
			 0x7fc00001,                                                  // signaling NaN                               /// 00498
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0049c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004a0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004a4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004a8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 004ac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004b0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 004b4
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 004b8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004bc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 004c0
			 0x0e00007f,                                                  // Trailing 1s:                                /// 004c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004cc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004d0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 004d4
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 004d8
			 0x80800000, // min norm                                      // subnormals -ve                              /// 004dc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 004e0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 004e4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004e8
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004ec
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 004f0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004f4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004fc
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00500
			 0x00000000,                                                  // zero                                        /// 00504
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00508
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 0050c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00510
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00514
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00518
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 0051c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00520
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00524
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00528
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 0052c
			 0xbf028f5c,                                                  // -0.51                                       /// 00530
			 0x80011111,                                                  // -9.7958E-41                                 /// 00534
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00538
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0053c
			 0x4b000000,                                                  // 8388608.0                                   /// 00540
			 0x3f028f5c,                                                  // 0.51                                        /// 00544
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00548
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 0054c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00550
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00558
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 0055c
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00560
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00564
			 0x00000000,                                                  // zero                                        /// 00568
			 0x0e003fff,                                                  // Trailing 1s:                                /// 0056c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00570
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00574
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00578
			 0x0c600000,                                                  // Leading 1s:                                 /// 0057c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00580
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00584
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00588
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0058c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00590
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00594
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00598
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 0059c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 005a0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005a4
			 0x0d00fff0,                                                  // Set of 1s                                   /// 005a8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005ac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 005b0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 005b4
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 005b8
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 005bc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 005c0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 005c4
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005c8
			 0xff800000,                                                  // -inf                                        /// 005cc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 005d0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 005d8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 005dc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 005e0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e4
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 005e8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 005ec
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005f0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005f4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 005f8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 005fc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00600
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00604
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0060c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00610
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00614
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00618
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0061c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00624
			 0x55555555,                                                  // 4 random values                             /// 00628
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 0062c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00630
			 0x0c400000,                                                  // Leading 1s:                                 /// 00634
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00638
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0063c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00640
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00644
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00648
			 0x33333333,                                                  // 4 random values                             /// 0064c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00650
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00654
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00658
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0065c
			 0x0c780000,                                                  // Leading 1s:                                 /// 00660
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00664
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00668
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0066c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00670
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00674
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00678
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00680
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00684
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00688
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0068c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00690
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00694
			 0x00000000,                                                  // zero                                        /// 00698
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0069c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006a0
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006a4
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006a8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 006ac
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 006b0
			 0xbf028f5c,                                                  // -0.51                                       /// 006b4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 006b8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 006bc
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 006c0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 006c4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006c8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006cc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006d0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 006d4
			 0x0c400000,                                                  // Leading 1s:                                 /// 006d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006dc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 006ec
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 006f0
			 0x0e001fff,                                                  // Trailing 1s:                                /// 006f4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 006f8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 006fc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00700
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00704
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0070c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00710
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00714
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00718
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0071c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00720
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00724
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00728
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0072c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00730
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00734
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0073c
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00740
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00744
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00748
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0074c
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00750
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00754
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00758
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0075c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00760
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00764
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00768
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0076c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00770
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00774
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00778
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 0077c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00780
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00784
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00788
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00790
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00794
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00798
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x4b000000,                                                  // 8388608.0                                   /// 007a0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 007a4
			 0x00000000,                                                  // zero                                        /// 007a8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007ac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007b0
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 007b8
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 007bc
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 007c8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007cc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 007d0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 007d4
			 0x0e000001,                                                  // Trailing 1s:                                /// 007d8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007e4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 007e8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007ec
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007f4
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007f8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 007fc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00800
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00804
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00808
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0080c
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00810
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00814
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00818
			 0x80011111,                                                  // -9.7958E-41                                 /// 0081c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00820
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00824
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00828
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00830
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00834
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00838
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0083c
			 0x7fc00001,                                                  // signaling NaN                               /// 00840
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00844
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00848
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 0084c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00850
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00858
			 0x0e000003,                                                  // Trailing 1s:                                /// 0085c
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00860
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00864
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00868
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0086c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00870
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00874
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00878
			 0x4b000000,                                                  // 8388608.0                                   /// 0087c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00880
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00884
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00888
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0088c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00890
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00894
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00898
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0089c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 008a0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008a4
			 0x0e000007,                                                  // Trailing 1s:                                /// 008a8
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008ac
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 008b4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 008b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 008bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 008c0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 008c4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 008c8
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 008cc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 008d0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008d4
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 008d8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008dc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 008e0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 008e4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 008ec
			 0x00011111,                                                  // 9.7958E-41                                  /// 008f0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008f4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008f8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 008fc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00900
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00904
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00908
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0090c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00910
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00914
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00918
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 0091c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00920
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00924
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00928
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 0092c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00934
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00938
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0093c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00940
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00944
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00948
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 0094c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00950
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00954
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00958
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0095c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00960
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00964
			 0x0e000003,                                                  // Trailing 1s:                                /// 00968
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0096c
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00970
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00974
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00978
			 0x0c780000,                                                  // Leading 1s:                                 /// 0097c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00980
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00984
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00988
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 0098c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00990
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0099c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 009a0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 009a4
			 0x0e000003,                                                  // Trailing 1s:                                /// 009a8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ac
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009b0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009b8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 009bc
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009c0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c4
			 0xcb000000,                                                  // -8388608.0                                  /// 009c8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009cc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 009d0
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 009d4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 009d8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009dc
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 009e0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009e4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 009e8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 009ec
			 0x7fc00001,                                                  // signaling NaN                               /// 009f0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 009f4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009f8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009fc
			 0xbf028f5c,                                                  // -0.51                                       /// 00a00
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00a04
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a08
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a0c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a10
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00a14
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a18
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00a1c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a20
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00a24
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00a28
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a2c
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00a30
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00a34
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a38
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a3c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a40
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a44
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00a48
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a4c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a50
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a54
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a60
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00a64
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00a68
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00a6c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a70
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a74
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a78
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00a7c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a80
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a84
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00a88
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00a8c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00a90
			 0x0e000001,                                                  // Trailing 1s:                                /// 00a94
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a98
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a9c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00aa0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00aa8
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00aac
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ab8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00ac0
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ac8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00acc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00ad0
			 0xcb000000,                                                  // -8388608.0                                  /// 00ad4
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00ad8
			 0x80000000,                                                  // -zero                                       /// 00adc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00ae0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00ae4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ae8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00aec
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00af0
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00af4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af8
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00afc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b00
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00b04
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b08
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00b0c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b10
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00b14
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b18
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b1c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b20
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b24
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00b28
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00b30
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b34
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b38
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00b3c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b40
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00b44
			 0x33333333,                                                  // 4 random values                             /// 00b48
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b4c
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b50
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b58
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00b5c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00b60
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b64
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00b68
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b6c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00b70
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00b74
			 0x33333333,                                                  // 4 random values                             /// 00b78
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b7c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b80
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b84
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00b88
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b90
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b94
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00b98
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b9c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ba0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ba4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00ba8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00bac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00bb4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bbc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00bc0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00bc4
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00bc8
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bcc
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00bd4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00bdc
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00be0
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00be4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00be8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00bec
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00bf0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bf4
			 0xbf028f5c,                                                  // -0.51                                       /// 00bf8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00bfc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00c00
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00c04
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c08
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00c10
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c14
			 0x7f800000,                                                  // inf                                         /// 00c18
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c1c
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00c20
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c24
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c28
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c2c
			 0x33333333,                                                  // 4 random values                             /// 00c30
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c34
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00c38
			 0x0c600000,                                                  // Leading 1s:                                 /// 00c3c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c40
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c44
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c48
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c4c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c50
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00c54
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c58
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c5c
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c60
			 0xbf028f5c,                                                  // -0.51                                       /// 00c64
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00c68
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c6c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00c70
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c74
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c78
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c7c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c80
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00c88
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c8c
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c90
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c94
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c98
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c9c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ca4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ca8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00cb4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cb8
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cbc
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00cc0
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00cc4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00cc8
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00ccc
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd0
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd4
			 0x0c700000,                                                  // Leading 1s:                                 /// 00cd8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ce0
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ce4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00ce8
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cec
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf0
			 0x0c600000,                                                  // Leading 1s:                                 /// 00cf4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf8
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00cfc
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d00
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d04
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d08
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d0c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00d10
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d14
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d18
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00d1c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00d20
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d24
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00d28
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00d2c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d34
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00d38
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00d3c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d40
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d44
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d48
			 0xcb000000,                                                  // -8388608.0                                  /// 00d4c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d50
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d58
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00d60
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00d64
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d68
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d6c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00d70
			 0x55555555,                                                  // 4 random values                             /// 00d74
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d78
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d7c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d80
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00d84
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00d88
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00d8c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d90
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00d94
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d98
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00d9c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00da4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00da8
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00dac
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00db0
			 0x80000000,                                                  // -zero                                       /// 00db4
			 0xffc00001,                                                  // -signaling NaN                              /// 00db8
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00dbc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00dc0
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00dc4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00dc8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00dcc
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00dd0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00dd4
			 0xbf028f5c,                                                  // -0.51                                       /// 00dd8
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00ddc
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00de0
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00de8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dec
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00df8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00dfc
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e00
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00e04
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00e08
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00e0c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00e10
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e14
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00e18
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e1c
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00e24
			 0x4b000000,                                                  // 8388608.0                                   /// 00e28
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00e2c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e30
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e34
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e38
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00e3c
			 0xff800000,                                                  // -inf                                        /// 00e40
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e48
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e4c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00e50
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e54
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e58
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e5c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e60
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e64
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00e6c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00e70
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e74
			 0x80000000,                                                  // -zero                                       /// 00e78
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00e7c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00e80
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00e84
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00e8c
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00e90
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e94
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00e98
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e9c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00ea0
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00ea4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ea8
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00eac
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00eb0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00eb4
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ebc
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00ec0
			 0x0c700000,                                                  // Leading 1s:                                 /// 00ec4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ec8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ecc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ed0
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00ed4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ed8
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00edc
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ee0
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ee4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00ee8
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00eec
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ef0
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ef4
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00ef8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00efc
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00f00
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f04
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00f08
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00f0c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00f10
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00f14
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00f18
			 0x00000000,                                                  // zero                                        /// 00f1c
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f20
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00f24
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f28
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00f2c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f30
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00f34
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00f38
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f3c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f40
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f48
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f4c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00f50
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f54
			 0x7f800000,                                                  // inf                                         /// 00f58
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f60
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00f64
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00f68
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f6c
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00f70
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f74
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f78
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00f7c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f80
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f84
			 0x33333333,                                                  // 4 random values                             /// 00f88
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00f8c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f94
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00f98
			 0x7f800000,                                                  // inf                                         /// 00f9c
			 0x4b000000,                                                  // 8388608.0                                   /// 00fa0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00fa4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00fa8
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00fac
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00fb0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00fb4
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fb8
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00fbc
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc0
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fc4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00fc8
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fcc
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00fd0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00fd4
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00fd8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00fdc
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00fe0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fe4
			 0x0c600000,                                                  // Leading 1s:                                 /// 00fe8
			 0xcb000000,                                                  // -8388608.0                                  /// 00fec
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ff0
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00ff4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ff8
			 0xbf800000 // 1                                             // SP - ve numbers                             /// last
	};
	volatile uint32_t m_12[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00000
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00008
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 0000c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00010
			 0x7f800000,                                                  // inf                                         /// 00014
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00018
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0001c
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00020
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00024
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00028
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00030
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00034
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00038
			 0x0e00000f,                                                  // Trailing 1s:                                /// 0003c
			 0xffc00001,                                                  // -signaling NaN                              /// 00040
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00044
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00048
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0004c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00050
			 0x4b000000,                                                  // 8388608.0                                   /// 00054
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00058
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0005c
			 0xffc00001,                                                  // -signaling NaN                              /// 00060
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00064
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00068
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0006c
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00070
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00074
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00078
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 0007c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00080
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00084
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00088
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00090
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00094
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00098
			 0x00000000,                                                  // zero                                        /// 0009c
			 0x00011111,                                                  // 9.7958E-41                                  /// 000a0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000a4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 000ac
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 000b4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000b8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000bc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 000c0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000c4
			 0x0c780000,                                                  // Leading 1s:                                 /// 000c8
			 0xcb000000,                                                  // -8388608.0                                  /// 000cc
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000d0
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 000d4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000d8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000dc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 000e0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 000e4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000e8
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000ec
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 000f0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 000f4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 000f8
			 0x0e000007,                                                  // Trailing 1s:                                /// 000fc
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00100
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00104
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00108
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 0010c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00110
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00114
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00118
			 0x0e00003f,                                                  // Trailing 1s:                                /// 0011c
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00120
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0012c
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00130
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00134
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00138
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0013c
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00140
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00144
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00148
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0014c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00150
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00154
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00158
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0015c
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00160
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00164
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00168
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 0016c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00170
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00174
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00178
			 0x0e000001,                                                  // Trailing 1s:                                /// 0017c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00180
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00184
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00188
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0018c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00190
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00194
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00198
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 0019c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 001a0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 001a4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 001a8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 001ac
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001b0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001b4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 001b8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 001bc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001c0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 001c4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001c8
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001cc
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001d0
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 001d4
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001d8
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 001dc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001e0
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001e4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 001e8
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 001ec
			 0x0c400000,                                                  // Leading 1s:                                 /// 001f0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 001f4
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x0e000003,                                                  // Trailing 1s:                                /// 001fc
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00200
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00204
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0020c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00210
			 0x00011111,                                                  // 9.7958E-41                                  /// 00214
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00218
			 0x33333333,                                                  // 4 random values                             /// 0021c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00220
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00224
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00228
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0022c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00230
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00234
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0023c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00240
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00244
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00248
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0024c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00250
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00254
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00258
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 0025c
			 0x0e000003,                                                  // Trailing 1s:                                /// 00260
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00264
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00268
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0026c
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00270
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00274
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00278
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0027c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00280
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00284
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00288
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0028c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00290
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00294
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00298
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0029c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002a0
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002a4
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 002a8
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x00000000,                                                  // zero                                        /// 002b0
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 002b4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002b8
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 002bc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 002c0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 002c4
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 002c8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 002cc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 002d0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 002d4
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 002d8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 002dc
			 0x00000000, // 0                                             // SP +ve numbers                              /// 002e0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 002e4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002e8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 002ec
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 002f0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002f8
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002fc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00300
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00304
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00308
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 0030c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00310
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00314
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00318
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0031c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00320
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00324
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00328
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00330
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00334
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00338
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00340
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00344
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00348
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0034c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00350
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00354
			 0x55555555,                                                  // 4 random values                             /// 00358
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0035c
			 0xbf028f5c,                                                  // -0.51                                       /// 00360
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00364
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00368
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 0036c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00370
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00374
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00378
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0037c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00380
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00384
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00388
			 0x55555555,                                                  // 4 random values                             /// 0038c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00390
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00394
			 0x55555555,                                                  // 4 random values                             /// 00398
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0039c
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 003a0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003a4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 003a8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003ac
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003b0
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 003b4
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003b8
			 0xff800000,                                                  // -inf                                        /// 003bc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003c0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 003cc
			 0x80800000, // min norm                                      // subnormals -ve                              /// 003d0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 003d4
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 003d8
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 003dc
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e0
			 0x0e000003,                                                  // Trailing 1s:                                /// 003e4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003e8
			 0x0e000003,                                                  // Trailing 1s:                                /// 003ec
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 003f0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 003f4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003f8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003fc
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00400
			 0x00011111,                                                  // 9.7958E-41                                  /// 00404
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00408
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 0040c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00410
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00414
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00418
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0041c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00420
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00424
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0042c
			 0xff800000,                                                  // -inf                                        /// 00430
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00434
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00438
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 0043c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00440
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00444
			 0x80011111,                                                  // -9.7958E-41                                 /// 00448
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0044c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00450
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00454
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00458
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 0045c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00460
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00464
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00468
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0046c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00470
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00474
			 0x55555555,                                                  // 4 random values                             /// 00478
			 0x0e000fff,                                                  // Trailing 1s:                                /// 0047c
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00480
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00488
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0048c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00490
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00494
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00498
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0049c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004a0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 004a4
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004a8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 004ac
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 004b0
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004b4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 004b8
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004bc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 004c4
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c8
			 0x7fc00000,                                                  // cquiet NaN                                  /// 004cc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 004d0
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 004d4
			 0x00011111,                                                  // 9.7958E-41                                  /// 004d8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 004dc
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004e0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 004e4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004e8
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 004ec
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004f0
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 004f4
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004f8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 004fc
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00500
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00504
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00508
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0050c
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00510
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00514
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00518
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0051c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00520
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00524
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00528
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0052c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00530
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00534
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00538
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 0053c
			 0x7fc00001,                                                  // signaling NaN                               /// 00540
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00544
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x3f028f5c,                                                  // 0.51                                        /// 0054c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00550
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00554
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00558
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0055c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00560
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00568
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0056c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00570
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00574
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00578
			 0x00000000,                                                  // zero                                        /// 0057c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00580
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00584
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00588
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 0058c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00590
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00594
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00598
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0059c
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 005a0
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 005a4
			 0x55555555,                                                  // 4 random values                             /// 005a8
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005ac
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 005b0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005b4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005b8
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005bc
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005c0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 005c4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005c8
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005cc
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005d0
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d4
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 005d8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 005dc
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 005e0
			 0x0c400000,                                                  // Leading 1s:                                 /// 005e4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 005e8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 005ec
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 005f0
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 005f4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005f8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005fc
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00600
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00604
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00608
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0060c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00610
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00614
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00618
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0061c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00620
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00624
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00628
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 0062c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00630
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00634
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00638
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0063c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00640
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00644
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00648
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 0064c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00650
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00654
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00658
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 0065c
			 0x3f028f5c,                                                  // 0.51                                        /// 00660
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00664
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00668
			 0x0e000003,                                                  // Trailing 1s:                                /// 0066c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00670
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00674
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00678
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0067c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00680
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00684
			 0x0c700000,                                                  // Leading 1s:                                 /// 00688
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0068c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00690
			 0x55555555,                                                  // 4 random values                             /// 00694
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00698
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0069c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 006a0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006a4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 006a8
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 006ac
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 006b0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006b4
			 0x55555555,                                                  // 4 random values                             /// 006b8
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 006bc
			 0x0c780000,                                                  // Leading 1s:                                 /// 006c0
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 006c4
			 0x0c700000,                                                  // Leading 1s:                                 /// 006c8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006cc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x00011111,                                                  // 9.7958E-41                                  /// 006d4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 006d8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 006dc
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e0
			 0xffc00001,                                                  // -signaling NaN                              /// 006e4
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006e8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 006ec
			 0x00011111,                                                  // 9.7958E-41                                  /// 006f0
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 006f4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 006f8
			 0x0c780000,                                                  // Leading 1s:                                 /// 006fc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00700
			 0x80011111,                                                  // -9.7958E-41                                 /// 00704
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00708
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0070c
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00710
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00714
			 0x7f800000,                                                  // inf                                         /// 00718
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 0071c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00720
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00724
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00728
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0072c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00730
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00734
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00738
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0073c
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00740
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00744
			 0x55555555,                                                  // 4 random values                             /// 00748
			 0x80800000, // min norm                                      // subnormals -ve                              /// 0074c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00750
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00754
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00758
			 0x0c780000,                                                  // Leading 1s:                                 /// 0075c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00760
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00764
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00768
			 0x0c600000,                                                  // Leading 1s:                                 /// 0076c
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00770
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00774
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00778
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0077c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00780
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00784
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00788
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0078c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00790
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00794
			 0x0e000007,                                                  // Trailing 1s:                                /// 00798
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0079c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 007a0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 007a4
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 007a8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 007ac
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b0
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007b4
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 007b8
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 007bc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 007c0
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 007c4
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007c8
			 0x0e00001f,                                                  // Trailing 1s:                                /// 007cc
			 0x00011111,                                                  // 9.7958E-41                                  /// 007d0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 007d8
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 007dc
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007e0
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 007e4
			 0x00000000,                                                  // zero                                        /// 007e8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 007ec
			 0x0e001fff,                                                  // Trailing 1s:                                /// 007f0
			 0x0e003fff,                                                  // Trailing 1s:                                /// 007f4
			 0x0e000007,                                                  // Trailing 1s:                                /// 007f8
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 007fc
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00800
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00804
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00808
			 0x0e000003,                                                  // Trailing 1s:                                /// 0080c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00810
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00814
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00818
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 0081c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00820
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00824
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00828
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0082c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00830
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00834
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00838
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0083c
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00840
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00844
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00848
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0084c
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00850
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00854
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00858
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0085c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00860
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00864
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00868
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0086c
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00870
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00874
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00878
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 0087c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00880
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00884
			 0x4b000000,                                                  // 8388608.0                                   /// 00888
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 0088c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00890
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00894
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00898
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0089c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 008a0
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008a4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 008a8
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 008ac
			 0xcb000000,                                                  // -8388608.0                                  /// 008b0
			 0x3f028f5c,                                                  // 0.51                                        /// 008b4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 008bc
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 008c0
			 0xffc00001,                                                  // -signaling NaN                              /// 008c4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 008c8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 008cc
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 008d0
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 008d4
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008d8
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 008dc
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 008e0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 008e4
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 008e8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008ec
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 008f0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 008f4
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008f8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 008fc
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00900
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00904
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0090c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00910
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00914
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00918
			 0xbf028f5c,                                                  // -0.51                                       /// 0091c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00920
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00924
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00928
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00930
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00934
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00938
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 0093c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00940
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00944
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00948
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 0094c
			 0x00000000,                                                  // zero                                        /// 00950
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00954
			 0x3f028f5c,                                                  // 0.51                                        /// 00958
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0095c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00960
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00964
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00968
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0096c
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00970
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00974
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00978
			 0x0c600000,                                                  // Leading 1s:                                 /// 0097c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00980
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00984
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00988
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0098c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00990
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00994
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00998
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0099c
			 0xff800000,                                                  // -inf                                        /// 009a0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 009a4
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009a8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 009ac
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009b0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009b4
			 0xcb000000,                                                  // -8388608.0                                  /// 009b8
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009bc
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009c0
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 009c4
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 009c8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 009cc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 009d0
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 009d4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 009d8
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 009dc
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 009e0
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009e4
			 0x0e00001f,                                                  // Trailing 1s:                                /// 009e8
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009ec
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 009f0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 009f4
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 009f8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 009fc
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a00
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a08
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a10
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a14
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00a18
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a1c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a20
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00a24
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a28
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a2c
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a30
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00a34
			 0x0e000007,                                                  // Trailing 1s:                                /// 00a38
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a3c
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a40
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a44
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a48
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00a4c
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a50
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a54
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00a58
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a5c
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00a60
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a68
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a6c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00a70
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a74
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00a78
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a7c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00a80
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00a84
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00a88
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00a8c
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a90
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00a94
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00a9c
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00aa0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00aa4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00aa8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aac
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00ab0
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ab4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ab8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00abc
			 0x55555555,                                                  // 4 random values                             /// 00ac0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00ac4
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ac8
			 0x0e000003,                                                  // Trailing 1s:                                /// 00acc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00ad0
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00ad4
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ad8
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00adc
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ae0
			 0x0e000007,                                                  // Trailing 1s:                                /// 00ae4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ae8
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00aec
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00af0
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00af4
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00af8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00afc
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00b00
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00b04
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00b08
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b0c
			 0x0c700000,                                                  // Leading 1s:                                 /// 00b10
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b14
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b18
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00b1c
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00b20
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b24
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00b28
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b2c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00b30
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00b34
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00b38
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b3c
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00b40
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b44
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b48
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00b4c
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b54
			 0x55555555,                                                  // 4 random values                             /// 00b58
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b5c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00b60
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00b64
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00b68
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00b70
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b74
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00b78
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00b7c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b80
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b84
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b88
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b8c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00b90
			 0x0e000001,                                                  // Trailing 1s:                                /// 00b94
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b98
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00b9c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ba0
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00ba4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ba8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bac
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00bb0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00bb4
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bb8
			 0xff800000,                                                  // -inf                                        /// 00bbc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00bc0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00bc4
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bcc
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 00bd0
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00bd4
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00bd8
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00bdc
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00be0
			 0x3f028f5c,                                                  // 0.51                                        /// 00be4
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00be8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bec
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf0
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00bf4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00bf8
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bfc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00c00
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00c04
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c08
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c0c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00c10
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00c14
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c18
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00c1c
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00c20
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00c24
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c28
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c2c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00c30
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00c34
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00c38
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c3c
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00c40
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00c44
			 0x00000000,                                                  // zero                                        /// 00c48
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00c4c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00c50
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c54
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00c58
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c5c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00c64
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c68
			 0x0c780000,                                                  // Leading 1s:                                 /// 00c6c
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c70
			 0x4b000000,                                                  // 8388608.0                                   /// 00c74
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c78
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00c7c
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00c80
			 0x80000000,                                                  // -zero                                       /// 00c84
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c88
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c8c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00c90
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00c94
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c98
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c9c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00ca0
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ca4
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00ca8
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 00cac
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cb0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cb4
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00cb8
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00cbc
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cc0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00cc4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00cc8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00ccc
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00cd0
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00cd4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00cd8
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00cdc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ce0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ce4
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce8
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00cec
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00cf0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00cf4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00cf8
			 0x80000000,                                                  // -zero                                       /// 00cfc
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00d00
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00d04
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00d08
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d0c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d10
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00d14
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d18
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d1c
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00d20
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00d24
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00d28
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00d30
			 0xffc00001,                                                  // -signaling NaN                              /// 00d34
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00d38
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d3c
			 0x7f800000,                                                  // inf                                         /// 00d40
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00d44
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00d48
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d4c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d50
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00d54
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00d58
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d5c
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d60
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00d64
			 0x00011111,                                                  // 9.7958E-41                                  /// 00d68
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d6c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00d70
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00d74
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00d78
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d7c
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00d80
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00d84
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00d88
			 0x7fc00001,                                                  // signaling NaN                               /// 00d8c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d90
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d94
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00d98
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d9c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00da0
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00da4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00da8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00dac
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00db0
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00db4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00db8
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00dbc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00dc0
			 0x00011111,                                                  // 9.7958E-41                                  /// 00dc4
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00dc8
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00dcc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00dd0
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00dd4
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00dd8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ddc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00de0
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00de4
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00dec
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00df0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00df4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00df8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00dfc
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e00
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00e04
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00e08
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00e0c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e10
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e14
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e18
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00e1c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e20
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e24
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00e28
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00e2c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e30
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00e34
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e38
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00e3c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e40
			 0x80000000,                                                  // -zero                                       /// 00e44
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 00e48
			 0x00000000,                                                  // zero                                        /// 00e4c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00e50
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e54
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00e58
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e5c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00e60
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00e64
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e68
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00e6c
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e70
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00e74
			 0x55555555,                                                  // 4 random values                             /// 00e78
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e7c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00e80
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00e84
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e88
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00e8c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e90
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e94
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e98
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00e9c
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ea0
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00ea4
			 0x7f800001, // SNaN                                          // SP +ve numbers                              /// 00ea8
			 0x7fc00001,                                                  // signaling NaN                               /// 00eac
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00eb0
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00eb4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00eb8
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ebc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00ec0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00ec4
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00ec8
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00ecc
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00ed0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ed4
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00ed8
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00edc
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee0
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ee4
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00ee8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00eec
			 0x4b000000,                                                  // 8388608.0                                   /// 00ef0
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ef4
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ef8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00efc
			 0xcb000000,                                                  // -8388608.0                                  /// 00f00
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00f04
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f08
			 0x33333333,                                                  // 4 random values                             /// 00f0c
			 0xbf028f5c,                                                  // -0.51                                       /// 00f10
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f14
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00f18
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f1c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00f20
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00f24
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00f28
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00f2c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f30
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00f34
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00f38
			 0x0e01ffff,                                                  // Trailing 1s:                                /// 00f3c
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f40
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f44
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f48
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00f4c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00f50
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f54
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f58
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f5c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00f60
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00f64
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f68
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00f6c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00f70
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00f74
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00f78
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00f7c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f80
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00f84
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f88
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00f8c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f90
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f94
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f98
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f9c
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00fa0
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fa4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fa8
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fac
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb0
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x00011111,                                                  // 9.7958E-41                                  /// 00fb8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00fbc
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00fc0
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00fc4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc8
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00fcc
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00fd0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd4
			 0x3f028f5c,                                                  // 0.51                                        /// 00fd8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00fdc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00fe0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00fe4
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00fe8
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00fec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00ff4
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ff8
			 0x0e00ffff                                                  // Trailing 1s:                                /// last
	};
	volatile uint32_t m_13[1024] __attribute__ ((aligned (4096))) = {
			 0xd014729f, /// 0x0
			 0x5ef3c593, /// 0x4
			 0x89deccf8, /// 0x8
			 0xb78a942b, /// 0xc
			 0x5ff1258e, /// 0x10
			 0x82b4faf4, /// 0x14
			 0x1f75b706, /// 0x18
			 0x2e9c9ff4, /// 0x1c
			 0x29a1b186, /// 0x20
			 0x85d484f1, /// 0x24
			 0x3b7c3937, /// 0x28
			 0x5af2fb2d, /// 0x2c
			 0xd8f1b2d9, /// 0x30
			 0xd6df52ac, /// 0x34
			 0x0b8c64c4, /// 0x38
			 0x310d9639, /// 0x3c
			 0xfa1837df, /// 0x40
			 0x888f383c, /// 0x44
			 0x58f39c44, /// 0x48
			 0x4bdfab4d, /// 0x4c
			 0x49fba83a, /// 0x50
			 0x2a6859f6, /// 0x54
			 0xcb26e7e8, /// 0x58
			 0x4c8b5529, /// 0x5c
			 0x3ee05504, /// 0x60
			 0x2f0535d0, /// 0x64
			 0xc1f1e325, /// 0x68
			 0x0f528075, /// 0x6c
			 0xd53c9e8d, /// 0x70
			 0xf0df1dc9, /// 0x74
			 0xede2753c, /// 0x78
			 0xa90dd37d, /// 0x7c
			 0x3842da11, /// 0x80
			 0xd5e40741, /// 0x84
			 0x137abf9a, /// 0x88
			 0x848d768f, /// 0x8c
			 0x7fc9df8b, /// 0x90
			 0x86cfc27d, /// 0x94
			 0x8c9c2a1e, /// 0x98
			 0x10f916a2, /// 0x9c
			 0x6c27fe14, /// 0xa0
			 0x4eab7868, /// 0xa4
			 0xb9772680, /// 0xa8
			 0xbbad5d20, /// 0xac
			 0x5d69a9c5, /// 0xb0
			 0x68130ec1, /// 0xb4
			 0x89c06004, /// 0xb8
			 0x2bdf66f0, /// 0xbc
			 0x71dec280, /// 0xc0
			 0x543dd056, /// 0xc4
			 0x61ea90cf, /// 0xc8
			 0x33641a35, /// 0xcc
			 0x220cfcd3, /// 0xd0
			 0x1677478a, /// 0xd4
			 0xec5a2e9e, /// 0xd8
			 0x04873fd8, /// 0xdc
			 0x7490dc50, /// 0xe0
			 0x00482eb4, /// 0xe4
			 0x74f49a36, /// 0xe8
			 0xcaa3ba01, /// 0xec
			 0x23cd8973, /// 0xf0
			 0xcdd68727, /// 0xf4
			 0x0a739711, /// 0xf8
			 0x04a85b33, /// 0xfc
			 0x5e6db95a, /// 0x100
			 0x74eadf5b, /// 0x104
			 0x9cc75922, /// 0x108
			 0xf98969c9, /// 0x10c
			 0xca6d3246, /// 0x110
			 0xcca97234, /// 0x114
			 0x99528e31, /// 0x118
			 0xb5216920, /// 0x11c
			 0xfb151d83, /// 0x120
			 0x37505c5b, /// 0x124
			 0x2e95189a, /// 0x128
			 0x663a9057, /// 0x12c
			 0x34c65d61, /// 0x130
			 0xfc3108ce, /// 0x134
			 0x573a5b66, /// 0x138
			 0x33e40d31, /// 0x13c
			 0xa529f1a0, /// 0x140
			 0xc0a29e9b, /// 0x144
			 0xcf6ddd7a, /// 0x148
			 0x0296578f, /// 0x14c
			 0x5b88ffc3, /// 0x150
			 0x14e834dd, /// 0x154
			 0x6938aaba, /// 0x158
			 0x586ea1bf, /// 0x15c
			 0x97d1b806, /// 0x160
			 0x9231f2be, /// 0x164
			 0x2c438a82, /// 0x168
			 0xd4c2b879, /// 0x16c
			 0x0d786640, /// 0x170
			 0x003e43ac, /// 0x174
			 0x02188c2e, /// 0x178
			 0xbe563a14, /// 0x17c
			 0x3362c139, /// 0x180
			 0x5240cb34, /// 0x184
			 0x6414399a, /// 0x188
			 0xcd53d653, /// 0x18c
			 0x01b6f0de, /// 0x190
			 0x2263357b, /// 0x194
			 0x295bc6c0, /// 0x198
			 0x1675d369, /// 0x19c
			 0xd14d0b9b, /// 0x1a0
			 0x606945c3, /// 0x1a4
			 0x20e55fe2, /// 0x1a8
			 0xff93609d, /// 0x1ac
			 0x1fb26414, /// 0x1b0
			 0xc89a7f9c, /// 0x1b4
			 0x773f5375, /// 0x1b8
			 0x95ff2350, /// 0x1bc
			 0x42d1a095, /// 0x1c0
			 0x2e4918ef, /// 0x1c4
			 0x08777f68, /// 0x1c8
			 0x2d842d62, /// 0x1cc
			 0xbd2200f9, /// 0x1d0
			 0xebc7e26a, /// 0x1d4
			 0x180abc0f, /// 0x1d8
			 0x3a8a1209, /// 0x1dc
			 0x1958617c, /// 0x1e0
			 0x539455f7, /// 0x1e4
			 0xceed347a, /// 0x1e8
			 0xe979443c, /// 0x1ec
			 0x04110ccb, /// 0x1f0
			 0x8b117624, /// 0x1f4
			 0x39728d7e, /// 0x1f8
			 0xdbda56f4, /// 0x1fc
			 0x9d1e0fe4, /// 0x200
			 0x86fe1452, /// 0x204
			 0xfa4b5858, /// 0x208
			 0xd481e55e, /// 0x20c
			 0xb256a163, /// 0x210
			 0x12c76fc4, /// 0x214
			 0xe79ecd7c, /// 0x218
			 0xa5ed80c4, /// 0x21c
			 0xe31cc9cd, /// 0x220
			 0x51e4d87f, /// 0x224
			 0x773867ee, /// 0x228
			 0x15b44e17, /// 0x22c
			 0x67076c9c, /// 0x230
			 0xeff81802, /// 0x234
			 0x271e7c07, /// 0x238
			 0x2257e26f, /// 0x23c
			 0x98dff1be, /// 0x240
			 0x0185775f, /// 0x244
			 0x5ce9313e, /// 0x248
			 0x80212889, /// 0x24c
			 0xad7009ab, /// 0x250
			 0xfad4fcd2, /// 0x254
			 0x02c64d57, /// 0x258
			 0x3d95678e, /// 0x25c
			 0x2f395067, /// 0x260
			 0x352b4249, /// 0x264
			 0x33df51ad, /// 0x268
			 0xe9865389, /// 0x26c
			 0x0a976900, /// 0x270
			 0x7eeff21a, /// 0x274
			 0x4e4a0c02, /// 0x278
			 0xc78f1431, /// 0x27c
			 0xadc1ee91, /// 0x280
			 0x7f44403b, /// 0x284
			 0xbbf1cab6, /// 0x288
			 0x7920e140, /// 0x28c
			 0x06f184d6, /// 0x290
			 0xe36f724a, /// 0x294
			 0xdfd75c5a, /// 0x298
			 0x795b0d05, /// 0x29c
			 0x4d4502cb, /// 0x2a0
			 0x345597df, /// 0x2a4
			 0x8a89b6fa, /// 0x2a8
			 0x4c875f37, /// 0x2ac
			 0xe3d3f9dd, /// 0x2b0
			 0x072fea4c, /// 0x2b4
			 0x7432f962, /// 0x2b8
			 0x17451212, /// 0x2bc
			 0xa73d838b, /// 0x2c0
			 0xf86be0f3, /// 0x2c4
			 0xc5e1ad16, /// 0x2c8
			 0x203ec81c, /// 0x2cc
			 0x7dabf05a, /// 0x2d0
			 0x8520ff69, /// 0x2d4
			 0x4cd8145c, /// 0x2d8
			 0x8cd06c8b, /// 0x2dc
			 0xd0178305, /// 0x2e0
			 0x1b114aa2, /// 0x2e4
			 0x2dda40d8, /// 0x2e8
			 0x93043726, /// 0x2ec
			 0x39fcce6b, /// 0x2f0
			 0xe05bc4d1, /// 0x2f4
			 0x7f815a21, /// 0x2f8
			 0x7f008a32, /// 0x2fc
			 0x5d8bde62, /// 0x300
			 0x1f0bf3cf, /// 0x304
			 0x71c00679, /// 0x308
			 0xf0d18955, /// 0x30c
			 0x27b72377, /// 0x310
			 0xd3d6d90c, /// 0x314
			 0x416d7c67, /// 0x318
			 0xc782e43e, /// 0x31c
			 0x47ba6e7e, /// 0x320
			 0xda45e908, /// 0x324
			 0x7da3f0b1, /// 0x328
			 0x5d3e6805, /// 0x32c
			 0xa244e22f, /// 0x330
			 0x016880be, /// 0x334
			 0x22078b31, /// 0x338
			 0xfd593134, /// 0x33c
			 0xdbae2cfc, /// 0x340
			 0x9cdb799a, /// 0x344
			 0x4e7f2118, /// 0x348
			 0x0bf0f5b8, /// 0x34c
			 0xc354b9aa, /// 0x350
			 0x068ffc33, /// 0x354
			 0x82eef730, /// 0x358
			 0x955a55ee, /// 0x35c
			 0x5e2e792f, /// 0x360
			 0x4bfd27ad, /// 0x364
			 0x3049095b, /// 0x368
			 0x38705ebf, /// 0x36c
			 0x87976069, /// 0x370
			 0xd211be60, /// 0x374
			 0x3f54d368, /// 0x378
			 0x7bd5b92b, /// 0x37c
			 0x389d7cc9, /// 0x380
			 0x3f49f4de, /// 0x384
			 0x43868205, /// 0x388
			 0x9c9d45c8, /// 0x38c
			 0x1577a2fb, /// 0x390
			 0xba68ece9, /// 0x394
			 0xb3b5d416, /// 0x398
			 0xbd4350ae, /// 0x39c
			 0xea6a9b05, /// 0x3a0
			 0xd2961cfb, /// 0x3a4
			 0x40c442ad, /// 0x3a8
			 0xa0d3f66b, /// 0x3ac
			 0xec102b58, /// 0x3b0
			 0x035e03c3, /// 0x3b4
			 0x16adb571, /// 0x3b8
			 0xe60a8875, /// 0x3bc
			 0xd12afb1e, /// 0x3c0
			 0xbb68ab36, /// 0x3c4
			 0x8af5fee6, /// 0x3c8
			 0xc5827440, /// 0x3cc
			 0xbacbc0f1, /// 0x3d0
			 0xc87d91af, /// 0x3d4
			 0x0b4eb258, /// 0x3d8
			 0x91448045, /// 0x3dc
			 0xe9a43702, /// 0x3e0
			 0x96049aa1, /// 0x3e4
			 0x2216b445, /// 0x3e8
			 0xee552ba3, /// 0x3ec
			 0x67211d68, /// 0x3f0
			 0x4473c831, /// 0x3f4
			 0x6a0caba5, /// 0x3f8
			 0x04e0c1be, /// 0x3fc
			 0x35b4244e, /// 0x400
			 0x3e72ba0f, /// 0x404
			 0x1f2faf5c, /// 0x408
			 0x764de549, /// 0x40c
			 0x0bcc03a2, /// 0x410
			 0x9b467651, /// 0x414
			 0x6ffcd96e, /// 0x418
			 0x31244e17, /// 0x41c
			 0x95ee488e, /// 0x420
			 0x4646fb62, /// 0x424
			 0x7dda736b, /// 0x428
			 0xa34f7854, /// 0x42c
			 0x64d72439, /// 0x430
			 0x76f3e777, /// 0x434
			 0xb93e5073, /// 0x438
			 0xbf6bac14, /// 0x43c
			 0xc4edc5ff, /// 0x440
			 0x7573ba37, /// 0x444
			 0x7be3ede0, /// 0x448
			 0x3c170046, /// 0x44c
			 0x00c98e77, /// 0x450
			 0xa468b51c, /// 0x454
			 0x8bbbed0c, /// 0x458
			 0x6cb21ce6, /// 0x45c
			 0x8b374983, /// 0x460
			 0x908e09e6, /// 0x464
			 0x66602c8f, /// 0x468
			 0xf3852daa, /// 0x46c
			 0xd77dbdf1, /// 0x470
			 0x2afdcbb5, /// 0x474
			 0x014646e8, /// 0x478
			 0x94b66d58, /// 0x47c
			 0xda1aead1, /// 0x480
			 0x2a5c0fd4, /// 0x484
			 0x41d9cf1c, /// 0x488
			 0x853f6631, /// 0x48c
			 0xd7cfbbd9, /// 0x490
			 0xb063da99, /// 0x494
			 0x4e3a38c2, /// 0x498
			 0xb0181ee1, /// 0x49c
			 0x7ee2f9a7, /// 0x4a0
			 0xe85f9d75, /// 0x4a4
			 0xee197ae5, /// 0x4a8
			 0x0a029282, /// 0x4ac
			 0xaae8962d, /// 0x4b0
			 0x13943368, /// 0x4b4
			 0x85f66b1f, /// 0x4b8
			 0x804b5f85, /// 0x4bc
			 0xad209924, /// 0x4c0
			 0x73e47258, /// 0x4c4
			 0x5960789e, /// 0x4c8
			 0xa559af0d, /// 0x4cc
			 0xad2d1309, /// 0x4d0
			 0xce1e4740, /// 0x4d4
			 0x26b1abb0, /// 0x4d8
			 0xf07268ff, /// 0x4dc
			 0x619eb32d, /// 0x4e0
			 0x8db2e28a, /// 0x4e4
			 0x813e0a38, /// 0x4e8
			 0xaf82b2c5, /// 0x4ec
			 0x8faabea5, /// 0x4f0
			 0xcf17e36d, /// 0x4f4
			 0x5f0153c9, /// 0x4f8
			 0xea010122, /// 0x4fc
			 0x6a9da420, /// 0x500
			 0x39333500, /// 0x504
			 0x0932ff5f, /// 0x508
			 0x4ff71339, /// 0x50c
			 0xf28a233c, /// 0x510
			 0x57b9f6ce, /// 0x514
			 0x3a7fd6ea, /// 0x518
			 0x340d7cc3, /// 0x51c
			 0xbcc25394, /// 0x520
			 0x6e575900, /// 0x524
			 0xe54ac988, /// 0x528
			 0x8218e213, /// 0x52c
			 0x857c4b26, /// 0x530
			 0x5a188d44, /// 0x534
			 0x3a435bd9, /// 0x538
			 0x054ba6ea, /// 0x53c
			 0x5f404b86, /// 0x540
			 0xed0c273b, /// 0x544
			 0x29aeebf8, /// 0x548
			 0xcc6b570f, /// 0x54c
			 0x989b1b44, /// 0x550
			 0x8b7e725d, /// 0x554
			 0x30c0e3c0, /// 0x558
			 0x16de2e0c, /// 0x55c
			 0xbded23bf, /// 0x560
			 0xfdaab572, /// 0x564
			 0x3c9ef38f, /// 0x568
			 0x4c30f8b4, /// 0x56c
			 0xa08a3821, /// 0x570
			 0x82dba630, /// 0x574
			 0xfd0af26f, /// 0x578
			 0x84b48c63, /// 0x57c
			 0x97f9dd2a, /// 0x580
			 0x0238b3c5, /// 0x584
			 0x8c558317, /// 0x588
			 0x89eb3d63, /// 0x58c
			 0x8a51cc18, /// 0x590
			 0x8fffdc5a, /// 0x594
			 0x51b41960, /// 0x598
			 0xd9988082, /// 0x59c
			 0x950c1bbd, /// 0x5a0
			 0x08e817a7, /// 0x5a4
			 0x1c8629d9, /// 0x5a8
			 0x45f1bf41, /// 0x5ac
			 0x932b6624, /// 0x5b0
			 0x44f5805d, /// 0x5b4
			 0x37f6956b, /// 0x5b8
			 0x82c52416, /// 0x5bc
			 0xa0ce0f75, /// 0x5c0
			 0xaf6ef78f, /// 0x5c4
			 0x13e42cac, /// 0x5c8
			 0x9bceb1b6, /// 0x5cc
			 0x7fc609d0, /// 0x5d0
			 0x884b0610, /// 0x5d4
			 0x4826a942, /// 0x5d8
			 0xaaef6141, /// 0x5dc
			 0xa4db94f3, /// 0x5e0
			 0xdf341697, /// 0x5e4
			 0x41efa744, /// 0x5e8
			 0xb7d92c47, /// 0x5ec
			 0xcb5999b4, /// 0x5f0
			 0x1165d8ae, /// 0x5f4
			 0x2d2befe8, /// 0x5f8
			 0xc6ef4dfe, /// 0x5fc
			 0x3cb9e38e, /// 0x600
			 0x17eb4e8a, /// 0x604
			 0x9717f4d2, /// 0x608
			 0x2bcfa741, /// 0x60c
			 0x0ec7c88d, /// 0x610
			 0x018104ab, /// 0x614
			 0x008845cb, /// 0x618
			 0xed00baee, /// 0x61c
			 0x4b634eac, /// 0x620
			 0x35713a72, /// 0x624
			 0x75d5834b, /// 0x628
			 0x7987b012, /// 0x62c
			 0xcb9b75f1, /// 0x630
			 0x27ee41d2, /// 0x634
			 0x4fb47d21, /// 0x638
			 0x02db7024, /// 0x63c
			 0xb6971c0c, /// 0x640
			 0x4e5533e3, /// 0x644
			 0x371094b0, /// 0x648
			 0xa168ab95, /// 0x64c
			 0x91e5e9a1, /// 0x650
			 0x9a11371d, /// 0x654
			 0x1b3d5cd6, /// 0x658
			 0xfce79d59, /// 0x65c
			 0x02226c9a, /// 0x660
			 0xef292403, /// 0x664
			 0xe9861f70, /// 0x668
			 0x30b0036c, /// 0x66c
			 0x7e030788, /// 0x670
			 0x1965b92c, /// 0x674
			 0x62460e06, /// 0x678
			 0xd5b256e8, /// 0x67c
			 0x97189821, /// 0x680
			 0xc68d3bfa, /// 0x684
			 0x6a9368a8, /// 0x688
			 0xc6c29324, /// 0x68c
			 0x7715d5d0, /// 0x690
			 0x1b4e6bba, /// 0x694
			 0x18a332d0, /// 0x698
			 0xb4cd2efa, /// 0x69c
			 0xaa26a7af, /// 0x6a0
			 0xbd4be627, /// 0x6a4
			 0x8836f538, /// 0x6a8
			 0x20036b69, /// 0x6ac
			 0xe6252790, /// 0x6b0
			 0x2ba88070, /// 0x6b4
			 0xbce19c37, /// 0x6b8
			 0xe23dbacb, /// 0x6bc
			 0x28691465, /// 0x6c0
			 0x10bc86a9, /// 0x6c4
			 0x3cf8f3cf, /// 0x6c8
			 0x718bc3bf, /// 0x6cc
			 0xc211e2e3, /// 0x6d0
			 0xc6cbd1c3, /// 0x6d4
			 0x40287b51, /// 0x6d8
			 0xb0b0a052, /// 0x6dc
			 0xc4a24eea, /// 0x6e0
			 0xcd7a1c9f, /// 0x6e4
			 0xc10d7eb6, /// 0x6e8
			 0xbb711df4, /// 0x6ec
			 0x2d612b71, /// 0x6f0
			 0x9c55d7c9, /// 0x6f4
			 0x11351097, /// 0x6f8
			 0x099980ee, /// 0x6fc
			 0x95a5a61f, /// 0x700
			 0xc9eee238, /// 0x704
			 0x72481651, /// 0x708
			 0x9e8159b0, /// 0x70c
			 0x3fd09f40, /// 0x710
			 0x676dc68b, /// 0x714
			 0xc0ed3f56, /// 0x718
			 0x2249e30f, /// 0x71c
			 0xca7bbd15, /// 0x720
			 0x633b1889, /// 0x724
			 0x6e0cfa61, /// 0x728
			 0x5e7684dd, /// 0x72c
			 0x4a177178, /// 0x730
			 0xb1c4d3cd, /// 0x734
			 0xb7e9ad34, /// 0x738
			 0x9b02d940, /// 0x73c
			 0xe29fa2b2, /// 0x740
			 0x96304efa, /// 0x744
			 0x27937785, /// 0x748
			 0x3f1b8700, /// 0x74c
			 0xab5730d8, /// 0x750
			 0xfcd18cf0, /// 0x754
			 0x1feceefd, /// 0x758
			 0x260c43b9, /// 0x75c
			 0x2d07da07, /// 0x760
			 0x8149b0ee, /// 0x764
			 0x59602b06, /// 0x768
			 0x8b2cf86f, /// 0x76c
			 0x8d33216a, /// 0x770
			 0x5cce2b74, /// 0x774
			 0xeb4b692d, /// 0x778
			 0xd59de807, /// 0x77c
			 0xc9402883, /// 0x780
			 0xb045058a, /// 0x784
			 0x3f3d3e5c, /// 0x788
			 0x9ac484b3, /// 0x78c
			 0xe8f48233, /// 0x790
			 0x4f2cee4c, /// 0x794
			 0xa6e70ea8, /// 0x798
			 0x1f570385, /// 0x79c
			 0xf8845bee, /// 0x7a0
			 0xae26739e, /// 0x7a4
			 0xed6779f4, /// 0x7a8
			 0xccf6949b, /// 0x7ac
			 0x1f050c6c, /// 0x7b0
			 0xd084df0d, /// 0x7b4
			 0x956c2b6f, /// 0x7b8
			 0xdc7a4d1f, /// 0x7bc
			 0x0042ce61, /// 0x7c0
			 0xbd9a3584, /// 0x7c4
			 0x5eae7789, /// 0x7c8
			 0xa2177d88, /// 0x7cc
			 0xc5b01268, /// 0x7d0
			 0xdc7002fb, /// 0x7d4
			 0x50c0b7f2, /// 0x7d8
			 0x6fc4dd14, /// 0x7dc
			 0x80f899bb, /// 0x7e0
			 0xb6172f08, /// 0x7e4
			 0x1886faa6, /// 0x7e8
			 0x46cb3ed0, /// 0x7ec
			 0x4400b3d2, /// 0x7f0
			 0x0fc03613, /// 0x7f4
			 0x704d4a8b, /// 0x7f8
			 0xa3439c0c, /// 0x7fc
			 0xab3680c8, /// 0x800
			 0xe94d0f1b, /// 0x804
			 0x86e064b7, /// 0x808
			 0x68061f59, /// 0x80c
			 0x857ade09, /// 0x810
			 0xe8e77c05, /// 0x814
			 0x8dd2d5be, /// 0x818
			 0x217176c1, /// 0x81c
			 0x63d40330, /// 0x820
			 0x21b14aa3, /// 0x824
			 0xd2831f4e, /// 0x828
			 0xe6ab2b81, /// 0x82c
			 0x4fd44417, /// 0x830
			 0x195f5c2c, /// 0x834
			 0x006d4290, /// 0x838
			 0xa866bb27, /// 0x83c
			 0x3b066233, /// 0x840
			 0xc87ec549, /// 0x844
			 0x7671a240, /// 0x848
			 0x6d799ca8, /// 0x84c
			 0x8743bd9e, /// 0x850
			 0x0896e623, /// 0x854
			 0x386c9ea9, /// 0x858
			 0x63a7b707, /// 0x85c
			 0xe19f2c97, /// 0x860
			 0x20175acb, /// 0x864
			 0xf7773e06, /// 0x868
			 0xa5a1a26e, /// 0x86c
			 0x63d0e8c5, /// 0x870
			 0xb4db3ec4, /// 0x874
			 0xa3a095b6, /// 0x878
			 0x10316f9e, /// 0x87c
			 0x35ab5703, /// 0x880
			 0x99627077, /// 0x884
			 0x06a3ae1f, /// 0x888
			 0x6bb1f7b5, /// 0x88c
			 0x9118721f, /// 0x890
			 0x7bf89d2a, /// 0x894
			 0x1d0fd697, /// 0x898
			 0x4e12b1ec, /// 0x89c
			 0x91d49374, /// 0x8a0
			 0x5428df56, /// 0x8a4
			 0x7bfea2cd, /// 0x8a8
			 0xba47be7e, /// 0x8ac
			 0x9bf86cda, /// 0x8b0
			 0xc67c6d97, /// 0x8b4
			 0x5f55c889, /// 0x8b8
			 0x2ca66916, /// 0x8bc
			 0x6361029f, /// 0x8c0
			 0xb1d79aa3, /// 0x8c4
			 0x184dfb3f, /// 0x8c8
			 0x47ef78fd, /// 0x8cc
			 0xd1479caa, /// 0x8d0
			 0xdb96864e, /// 0x8d4
			 0x9afafc5e, /// 0x8d8
			 0xb5f3c446, /// 0x8dc
			 0x3e8ed78f, /// 0x8e0
			 0x075f1973, /// 0x8e4
			 0x0c265e00, /// 0x8e8
			 0x6330780e, /// 0x8ec
			 0x841a44a0, /// 0x8f0
			 0xd2b76d3e, /// 0x8f4
			 0x8299fd2a, /// 0x8f8
			 0x753302c7, /// 0x8fc
			 0xe68bea33, /// 0x900
			 0xf72961f9, /// 0x904
			 0x629b91e9, /// 0x908
			 0x8108372f, /// 0x90c
			 0x382e35b7, /// 0x910
			 0x0f60a871, /// 0x914
			 0xccc47cfb, /// 0x918
			 0x183f438e, /// 0x91c
			 0x7184f40a, /// 0x920
			 0xc8868d7a, /// 0x924
			 0xe3a440f6, /// 0x928
			 0xb7a3ef27, /// 0x92c
			 0x159c14b8, /// 0x930
			 0x31c33bf7, /// 0x934
			 0xaaba58e8, /// 0x938
			 0xa6c8e854, /// 0x93c
			 0x116a0277, /// 0x940
			 0x977d13ca, /// 0x944
			 0xbf1acebf, /// 0x948
			 0x296f23e0, /// 0x94c
			 0xaef155f7, /// 0x950
			 0x2fc20865, /// 0x954
			 0x4031dfe2, /// 0x958
			 0x9070932d, /// 0x95c
			 0xf8983f73, /// 0x960
			 0xe74065ac, /// 0x964
			 0x2719adab, /// 0x968
			 0x51dbe377, /// 0x96c
			 0x32adfb0f, /// 0x970
			 0xa127e638, /// 0x974
			 0xd66a89b0, /// 0x978
			 0xfbe430db, /// 0x97c
			 0x544c5fe3, /// 0x980
			 0x89a09fdd, /// 0x984
			 0x821a8afb, /// 0x988
			 0x3b974a08, /// 0x98c
			 0x47fd18c9, /// 0x990
			 0xe444c3cf, /// 0x994
			 0x6d255093, /// 0x998
			 0xd28d644c, /// 0x99c
			 0xc416797d, /// 0x9a0
			 0x46606ff3, /// 0x9a4
			 0xd9455d4a, /// 0x9a8
			 0xd4d7611a, /// 0x9ac
			 0xeeec2056, /// 0x9b0
			 0x38f21887, /// 0x9b4
			 0x10adabaf, /// 0x9b8
			 0x8f6920d5, /// 0x9bc
			 0xc8f83fbb, /// 0x9c0
			 0x8e1c8a25, /// 0x9c4
			 0x4a1fcff0, /// 0x9c8
			 0x37a18ee5, /// 0x9cc
			 0x1930b46c, /// 0x9d0
			 0xfbe1dc03, /// 0x9d4
			 0x77f7f85e, /// 0x9d8
			 0x6b93ef49, /// 0x9dc
			 0x5cbbc2bf, /// 0x9e0
			 0xac4adcb0, /// 0x9e4
			 0x87265f0f, /// 0x9e8
			 0x546bb7cd, /// 0x9ec
			 0x560e028d, /// 0x9f0
			 0xd67a02a2, /// 0x9f4
			 0x55c8ca50, /// 0x9f8
			 0xa79bf53f, /// 0x9fc
			 0xa8e149ea, /// 0xa00
			 0x1755ee5e, /// 0xa04
			 0x0f04ced3, /// 0xa08
			 0x33f14cb4, /// 0xa0c
			 0x5c1f5810, /// 0xa10
			 0x013b38fd, /// 0xa14
			 0x64d9fa82, /// 0xa18
			 0x4b2268b5, /// 0xa1c
			 0xc3205447, /// 0xa20
			 0xfc1e7302, /// 0xa24
			 0x270a953f, /// 0xa28
			 0xb9c207ed, /// 0xa2c
			 0x49653784, /// 0xa30
			 0x4cb7ccbe, /// 0xa34
			 0x4fa06bf9, /// 0xa38
			 0x8e32a593, /// 0xa3c
			 0x4705fec4, /// 0xa40
			 0x1bc4503c, /// 0xa44
			 0xf6c24620, /// 0xa48
			 0x2875ddad, /// 0xa4c
			 0x43da6c7b, /// 0xa50
			 0xba3af491, /// 0xa54
			 0x9daab36d, /// 0xa58
			 0xec1493c9, /// 0xa5c
			 0xd313e8bf, /// 0xa60
			 0x92890714, /// 0xa64
			 0x78406409, /// 0xa68
			 0xe1c43c12, /// 0xa6c
			 0xd5a839fe, /// 0xa70
			 0x1b96ab66, /// 0xa74
			 0x94541c74, /// 0xa78
			 0x1b03471e, /// 0xa7c
			 0x40843d6c, /// 0xa80
			 0x05ecbdc1, /// 0xa84
			 0x1a6ea548, /// 0xa88
			 0x0fdd3663, /// 0xa8c
			 0xc9e07dda, /// 0xa90
			 0xbce85979, /// 0xa94
			 0x17320ca2, /// 0xa98
			 0x157a59f0, /// 0xa9c
			 0xf2c4e2e5, /// 0xaa0
			 0xfcd16ae8, /// 0xaa4
			 0x03bf379e, /// 0xaa8
			 0x68de0f27, /// 0xaac
			 0xa38f87d0, /// 0xab0
			 0x8196cbf6, /// 0xab4
			 0x54c8d718, /// 0xab8
			 0x97bbaf0b, /// 0xabc
			 0x0023a722, /// 0xac0
			 0x560b6e8b, /// 0xac4
			 0x98f4c3fc, /// 0xac8
			 0x4dd4bd40, /// 0xacc
			 0x97584f36, /// 0xad0
			 0xf3d440e7, /// 0xad4
			 0x5cf8e2c4, /// 0xad8
			 0xf6b1c9d0, /// 0xadc
			 0x8ce751d9, /// 0xae0
			 0xeaa2f2a4, /// 0xae4
			 0x006a688a, /// 0xae8
			 0x840b232f, /// 0xaec
			 0xcf58682b, /// 0xaf0
			 0xf0ee4200, /// 0xaf4
			 0x02b129a8, /// 0xaf8
			 0x5ae3f3b5, /// 0xafc
			 0x4e713c73, /// 0xb00
			 0xa8d7cec7, /// 0xb04
			 0xa2a5fb1a, /// 0xb08
			 0x298c5acf, /// 0xb0c
			 0xcff530ed, /// 0xb10
			 0xbb3409c6, /// 0xb14
			 0xd6fac26d, /// 0xb18
			 0x40a74c9c, /// 0xb1c
			 0x69700ef1, /// 0xb20
			 0x7ea0d7ad, /// 0xb24
			 0xd6f04d46, /// 0xb28
			 0xb5369768, /// 0xb2c
			 0x54448c9b, /// 0xb30
			 0x4283ff0b, /// 0xb34
			 0x7571d4e1, /// 0xb38
			 0x74c9961e, /// 0xb3c
			 0x2ccd43b3, /// 0xb40
			 0xb98ed456, /// 0xb44
			 0x43d84189, /// 0xb48
			 0xefe3d806, /// 0xb4c
			 0x4b9f1b0d, /// 0xb50
			 0x9d707b0f, /// 0xb54
			 0x00954cf9, /// 0xb58
			 0x60666cbe, /// 0xb5c
			 0x3000839a, /// 0xb60
			 0x75382710, /// 0xb64
			 0x5b2ee4ba, /// 0xb68
			 0x6368e2c3, /// 0xb6c
			 0x6065064d, /// 0xb70
			 0x3ce09411, /// 0xb74
			 0xa63e349f, /// 0xb78
			 0x54ac44bb, /// 0xb7c
			 0x6fa8e248, /// 0xb80
			 0xe558fefb, /// 0xb84
			 0x725a12b3, /// 0xb88
			 0xf32ac77d, /// 0xb8c
			 0x1d27159b, /// 0xb90
			 0x4cca0d6a, /// 0xb94
			 0x5f4dcae7, /// 0xb98
			 0x7ec6eecd, /// 0xb9c
			 0x65d42a94, /// 0xba0
			 0x5faefb75, /// 0xba4
			 0x814fc224, /// 0xba8
			 0x9d90a86a, /// 0xbac
			 0xe83e43f3, /// 0xbb0
			 0xa4afa989, /// 0xbb4
			 0x22bb4d55, /// 0xbb8
			 0x94bb95a3, /// 0xbbc
			 0xa7a1aace, /// 0xbc0
			 0xc9452579, /// 0xbc4
			 0x05b6a7e9, /// 0xbc8
			 0x782606c1, /// 0xbcc
			 0xfbe4a521, /// 0xbd0
			 0x904bdb94, /// 0xbd4
			 0x50699f8a, /// 0xbd8
			 0x94910cda, /// 0xbdc
			 0x2e81e8b1, /// 0xbe0
			 0x6e81db10, /// 0xbe4
			 0xea5b845d, /// 0xbe8
			 0xc0a01290, /// 0xbec
			 0x2ff6c7a2, /// 0xbf0
			 0x89195b38, /// 0xbf4
			 0xfd47d1d0, /// 0xbf8
			 0xb836df34, /// 0xbfc
			 0xaf6f1e54, /// 0xc00
			 0xb3a3cfaa, /// 0xc04
			 0x6159716e, /// 0xc08
			 0xb2d81254, /// 0xc0c
			 0xca1f44eb, /// 0xc10
			 0x460b5ad1, /// 0xc14
			 0x374cc11b, /// 0xc18
			 0xea3313ab, /// 0xc1c
			 0x2cb2e472, /// 0xc20
			 0xccb90752, /// 0xc24
			 0x9e770977, /// 0xc28
			 0x2217d07c, /// 0xc2c
			 0xb77b5938, /// 0xc30
			 0x688b891a, /// 0xc34
			 0xf721364c, /// 0xc38
			 0x22c9d64c, /// 0xc3c
			 0x72fab896, /// 0xc40
			 0xce69c9e0, /// 0xc44
			 0xd10f129a, /// 0xc48
			 0x4ebf10e3, /// 0xc4c
			 0xcb4a2017, /// 0xc50
			 0x1242a7b2, /// 0xc54
			 0xedae8c72, /// 0xc58
			 0x793f3f90, /// 0xc5c
			 0x0bab5854, /// 0xc60
			 0xdd265808, /// 0xc64
			 0x8993b3f2, /// 0xc68
			 0x963a34b0, /// 0xc6c
			 0x2ae406b8, /// 0xc70
			 0x333e8d95, /// 0xc74
			 0xead46429, /// 0xc78
			 0xdbaa4458, /// 0xc7c
			 0x198963b4, /// 0xc80
			 0x12617cd9, /// 0xc84
			 0x6a923dbb, /// 0xc88
			 0x7fd93a71, /// 0xc8c
			 0xd30c9b4e, /// 0xc90
			 0xe3a679bd, /// 0xc94
			 0x94454d64, /// 0xc98
			 0x75c79d68, /// 0xc9c
			 0x4d0bf7b2, /// 0xca0
			 0x77e6b9ce, /// 0xca4
			 0xe761849d, /// 0xca8
			 0x0c3502b6, /// 0xcac
			 0x91c07a43, /// 0xcb0
			 0xef76108a, /// 0xcb4
			 0x788490a7, /// 0xcb8
			 0x9c8cabd7, /// 0xcbc
			 0x3a5933d1, /// 0xcc0
			 0xa4b0a92e, /// 0xcc4
			 0xc810ef9e, /// 0xcc8
			 0xe9747a37, /// 0xccc
			 0xc1462d7b, /// 0xcd0
			 0xaad3c2f0, /// 0xcd4
			 0xe57dc983, /// 0xcd8
			 0x54818873, /// 0xcdc
			 0x4e90a71b, /// 0xce0
			 0xdbddd670, /// 0xce4
			 0xa307968b, /// 0xce8
			 0x60b1221d, /// 0xcec
			 0x705d9d8d, /// 0xcf0
			 0xfdec6b44, /// 0xcf4
			 0xcacd6d83, /// 0xcf8
			 0x2bc73489, /// 0xcfc
			 0xcf874291, /// 0xd00
			 0x818803de, /// 0xd04
			 0x38268edb, /// 0xd08
			 0x6c33438d, /// 0xd0c
			 0x73ba4206, /// 0xd10
			 0x8c8a4042, /// 0xd14
			 0x53606243, /// 0xd18
			 0x7fcf9486, /// 0xd1c
			 0xf7bc911c, /// 0xd20
			 0xa42e5e0d, /// 0xd24
			 0xc82e031a, /// 0xd28
			 0x716237ea, /// 0xd2c
			 0xbfb31967, /// 0xd30
			 0x63a753e6, /// 0xd34
			 0xfb26f85c, /// 0xd38
			 0x64ca2880, /// 0xd3c
			 0x98c3fa02, /// 0xd40
			 0x1fcb62b8, /// 0xd44
			 0xac4c12fb, /// 0xd48
			 0xaa512eea, /// 0xd4c
			 0xa4b5997c, /// 0xd50
			 0x6c28c544, /// 0xd54
			 0x487a5f85, /// 0xd58
			 0xdef25796, /// 0xd5c
			 0x1e89433c, /// 0xd60
			 0x8bdf71c9, /// 0xd64
			 0x7dfd1de3, /// 0xd68
			 0xc4d8a1c9, /// 0xd6c
			 0xa0b6167c, /// 0xd70
			 0xc318cd7b, /// 0xd74
			 0xf5a3cd17, /// 0xd78
			 0xc177192d, /// 0xd7c
			 0x87cb60be, /// 0xd80
			 0xa2290b6f, /// 0xd84
			 0x239633cd, /// 0xd88
			 0x14803187, /// 0xd8c
			 0x6d1fe026, /// 0xd90
			 0x2521d637, /// 0xd94
			 0x2d8c3842, /// 0xd98
			 0x34cdd9f1, /// 0xd9c
			 0x41f74859, /// 0xda0
			 0x9fccb7a1, /// 0xda4
			 0x8d5eee7d, /// 0xda8
			 0xca763b38, /// 0xdac
			 0x470c7bb0, /// 0xdb0
			 0xed668566, /// 0xdb4
			 0x22e54d67, /// 0xdb8
			 0xcb0ebd73, /// 0xdbc
			 0x10d30fba, /// 0xdc0
			 0x9bcbbfdc, /// 0xdc4
			 0x76d68c4e, /// 0xdc8
			 0x79b9984a, /// 0xdcc
			 0x6c7c1b1d, /// 0xdd0
			 0x38f7604e, /// 0xdd4
			 0x81e23926, /// 0xdd8
			 0x4ae833db, /// 0xddc
			 0x0d8b50b7, /// 0xde0
			 0x53201123, /// 0xde4
			 0x8840e18b, /// 0xde8
			 0x35f62565, /// 0xdec
			 0xacc10745, /// 0xdf0
			 0xd8b9d5d4, /// 0xdf4
			 0xa164a74d, /// 0xdf8
			 0x46b27535, /// 0xdfc
			 0xc260081e, /// 0xe00
			 0xfebdf22a, /// 0xe04
			 0xa68aeece, /// 0xe08
			 0xfe97b289, /// 0xe0c
			 0x85a50c3b, /// 0xe10
			 0x3a57a082, /// 0xe14
			 0xa043af48, /// 0xe18
			 0xfc6481e5, /// 0xe1c
			 0xd7b04ee3, /// 0xe20
			 0xbbbe7a0e, /// 0xe24
			 0x3f788093, /// 0xe28
			 0xf64ee22d, /// 0xe2c
			 0x79036204, /// 0xe30
			 0xd2f772ab, /// 0xe34
			 0x6d600176, /// 0xe38
			 0xce5b82c7, /// 0xe3c
			 0x0c04f75a, /// 0xe40
			 0xb0722305, /// 0xe44
			 0x25c52e19, /// 0xe48
			 0x088a9211, /// 0xe4c
			 0x203e519d, /// 0xe50
			 0xb32cc5ab, /// 0xe54
			 0x3271b918, /// 0xe58
			 0x2c08d0a7, /// 0xe5c
			 0xf840cc67, /// 0xe60
			 0xffb450ec, /// 0xe64
			 0x3e9f6bb6, /// 0xe68
			 0x1bf46352, /// 0xe6c
			 0x6a7d3576, /// 0xe70
			 0x75289437, /// 0xe74
			 0xa635b20e, /// 0xe78
			 0x1bec448a, /// 0xe7c
			 0x785398cf, /// 0xe80
			 0xf23df09a, /// 0xe84
			 0x34c868d5, /// 0xe88
			 0xe73caf3d, /// 0xe8c
			 0x3cc6f2d5, /// 0xe90
			 0xd31cac7f, /// 0xe94
			 0x351d205c, /// 0xe98
			 0x822d9d1c, /// 0xe9c
			 0xcda1bc96, /// 0xea0
			 0x5c5794d1, /// 0xea4
			 0xac0ad612, /// 0xea8
			 0x5e86a485, /// 0xeac
			 0xdb5d4db5, /// 0xeb0
			 0x81832131, /// 0xeb4
			 0x28d497f2, /// 0xeb8
			 0xac7a7eb2, /// 0xebc
			 0x5c1a9f6d, /// 0xec0
			 0x26c5e6f4, /// 0xec4
			 0x59010598, /// 0xec8
			 0x1b5ffd81, /// 0xecc
			 0xed05ce36, /// 0xed0
			 0x0b4236ed, /// 0xed4
			 0x56b8fed1, /// 0xed8
			 0x8e5ce5fb, /// 0xedc
			 0x4642e43e, /// 0xee0
			 0x44e5e877, /// 0xee4
			 0xf370fbd4, /// 0xee8
			 0x6f468b7b, /// 0xeec
			 0x2be22e72, /// 0xef0
			 0x77f4bd79, /// 0xef4
			 0x81baf37c, /// 0xef8
			 0xda03694d, /// 0xefc
			 0x90214c3a, /// 0xf00
			 0x3d25b72b, /// 0xf04
			 0x1de6f0c5, /// 0xf08
			 0xd509ce82, /// 0xf0c
			 0x2d210e6f, /// 0xf10
			 0x733a93fe, /// 0xf14
			 0xdc9ecdc2, /// 0xf18
			 0xfaf591e0, /// 0xf1c
			 0x99dbc440, /// 0xf20
			 0x7f9e7c3f, /// 0xf24
			 0xde742d7f, /// 0xf28
			 0x3784f9c9, /// 0xf2c
			 0x1ffd8141, /// 0xf30
			 0x4846f974, /// 0xf34
			 0x25e09b99, /// 0xf38
			 0x99c9121c, /// 0xf3c
			 0x7efc2862, /// 0xf40
			 0xf7490498, /// 0xf44
			 0x295a715c, /// 0xf48
			 0x252906f5, /// 0xf4c
			 0xc21ff46b, /// 0xf50
			 0xee8be2e9, /// 0xf54
			 0x44ab1fe9, /// 0xf58
			 0xbe530c88, /// 0xf5c
			 0xeba8e4d2, /// 0xf60
			 0x38799380, /// 0xf64
			 0xe423e824, /// 0xf68
			 0x95eb4fdb, /// 0xf6c
			 0xc5c4eac1, /// 0xf70
			 0xbc6aff9b, /// 0xf74
			 0xd04e3886, /// 0xf78
			 0x15d88df9, /// 0xf7c
			 0x25d91cc8, /// 0xf80
			 0x012b8660, /// 0xf84
			 0xb175482f, /// 0xf88
			 0x27642633, /// 0xf8c
			 0xb9a1fe8b, /// 0xf90
			 0x2f425409, /// 0xf94
			 0x458e6dd7, /// 0xf98
			 0x82f895fe, /// 0xf9c
			 0xc25b3201, /// 0xfa0
			 0x28e62138, /// 0xfa4
			 0xe1105dd1, /// 0xfa8
			 0xd39a7223, /// 0xfac
			 0xaa25ba88, /// 0xfb0
			 0xcd4377d0, /// 0xfb4
			 0x770356cf, /// 0xfb8
			 0xe50d0f12, /// 0xfbc
			 0x92ad2c0b, /// 0xfc0
			 0x21120b3c, /// 0xfc4
			 0x14122a9e, /// 0xfc8
			 0x3e9f6441, /// 0xfcc
			 0x1131ae22, /// 0xfd0
			 0xfbe30352, /// 0xfd4
			 0xc43c1691, /// 0xfd8
			 0x2d921f1c, /// 0xfdc
			 0x1c165b22, /// 0xfe0
			 0x60eba49c, /// 0xfe4
			 0xe6d86741, /// 0xfe8
			 0x49a60eac, /// 0xfec
			 0x162afc70, /// 0xff0
			 0x9a6a1d36, /// 0xff4
			 0x4b19d2ff, /// 0xff8
			 0xda720282 /// last
	};
	volatile uint32_t m_14[1024] __attribute__ ((aligned (4096))) = {
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00000
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00004
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00008
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0000c
			 0x55555555,                                                  // 4 random values                             /// 00010
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00014
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00018
			 0x0e000001,                                                  // Trailing 1s:                                /// 0001c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00020
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00024
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00028
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0002c
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00030
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00034
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00038
			 0x0c400000,                                                  // Leading 1s:                                 /// 0003c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00040
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00044
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00048
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0004c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00050
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00054
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00058
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0005c
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00060
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00064
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00068
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 0006c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00070
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00074
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00078
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0007c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00080
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00084
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00088
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0008c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00090
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00094
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00098
			 0x0c700000,                                                  // Leading 1s:                                 /// 0009c
			 0xAAAAAAAA,                                                  // 4 random values                             /// 000a0
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000a4
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000a8
			 0x55555555,                                                  // 4 random values                             /// 000ac
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 000b0
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 000b4
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 000b8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000bc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 000c0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 000c4
			 0x33333333,                                                  // 4 random values                             /// 000c8
			 0xbf028f5c,                                                  // -0.51                                       /// 000cc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 000d0
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 000d4
			 0x80011111,                                                  // -9.7958E-41                                 /// 000d8
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 000dc
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 000e0
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 000e4
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 000e8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000ec
			 0x80000000,                                                  // -zero                                       /// 000f0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f4
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000f8
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 000fc
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00100
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00104
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00108
			 0xff800000,                                                  // -inf                                        /// 0010c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00110
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00114
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00118
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0011c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00120
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00124
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00128
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 0012c
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00130
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00134
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00138
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0013c
			 0x7f800000,                                                  // inf                                         /// 00140
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00144
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00148
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 0014c
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00150
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00154
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00158
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0015c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00160
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00164
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00168
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0016c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00170
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00174
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00178
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 0017c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00180
			 0xffc00001,                                                  // -signaling NaN                              /// 00184
			 0x0e000007,                                                  // Trailing 1s:                                /// 00188
			 0x0d00fff0,                                                  // Set of 1s                                   /// 0018c
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00190
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00194
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00198
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0019c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 001a0
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001a4
			 0x0c600000,                                                  // Leading 1s:                                 /// 001a8
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 001ac
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 001b0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 001b4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 001b8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001bc
			 0x7f800000,                                                  // inf                                         /// 001c0
			 0x0c700000,                                                  // Leading 1s:                                 /// 001c4
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 001c8
			 0xbf028f5c,                                                  // -0.51                                       /// 001cc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 001d0
			 0x0c600000,                                                  // Leading 1s:                                 /// 001d4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001d8
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 001dc
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 001e0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 001e4
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 001e8
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 001ec
			 0x00000000,                                                  // zero                                        /// 001f0
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 001f4
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 001f8
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 001fc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00200
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00204
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00208
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0020c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00210
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00214
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00218
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0021c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00220
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00224
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00228
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 0022c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00230
			 0x0c700000,                                                  // Leading 1s:                                 /// 00234
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00238
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0023c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00240
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00244
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00248
			 0x80011111,                                                  // -9.7958E-41                                 /// 0024c
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00250
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00254
			 0x7f7ffffe, // max norm - 3ulp                               // max norm +ve                                /// 00258
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0025c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00260
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00264
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00268
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0026c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00270
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00274
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00278
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 0027c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00280
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00284
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00288
			 0x3f800001, // 1  + 1ulp                                     // SP +ve numbers                              /// 0028c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00290
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00294
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00298
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 0029c
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 002a0
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 002a4
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 002a8
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002ac
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 002b0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 002b4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 002b8
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 002bc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 002c0
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002c4
			 0x0e007fff,                                                  // Trailing 1s:                                /// 002c8
			 0x7fc00001,                                                  // signaling NaN                               /// 002cc
			 0xffc00000,                                                  // -cquiet NaN                                 /// 002d0
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002d4
			 0xcb000000,                                                  // -8388608.0                                  /// 002d8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 002dc
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 002e0
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 002e4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 002e8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 002ec
			 0x0d000ff0,                                                  // Set of 1s                                   /// 002f0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 002f4
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 002f8
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 002fc
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00300
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00304
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 00308
			 0x80000000,                                                  // -zero                                       /// 0030c
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00310
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00314
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00318
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 0031c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00320
			 0xff800000,                                                  // -inf                                        /// 00324
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00328
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0032c
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00330
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00334
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00338
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0033c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00340
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00344
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00348
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0034c
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00350
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00354
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00358
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 0035c
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00360
			 0x0e000007,                                                  // Trailing 1s:                                /// 00364
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00368
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0036c
			 0x80800000, // min norm                                      // SP - ve numbers                             /// 00370
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00374
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00378
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 0037c
			 0x0c600000,                                                  // Leading 1s:                                 /// 00380
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00384
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00388
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0038c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00390
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00394
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00398
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0039c
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 003a0
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003a4
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003a8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 003ac
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 003b0
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 003b4
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 003b8
			 0x0e000001,                                                  // Trailing 1s:                                /// 003bc
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 003c0
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003c8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003cc
			 0x0e00003f,                                                  // Trailing 1s:                                /// 003d0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 003d4
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 003d8
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 003dc
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 003e0
			 0x0e000003,                                                  // Trailing 1s:                                /// 003e4
			 0x00000000,                                                  // zero                                        /// 003e8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003ec
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 003f0
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 003f4
			 0x0e00000f,                                                  // Trailing 1s:                                /// 003f8
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 003fc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00400
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00404
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00408
			 0x7fc00001,                                                  // signaling NaN                               /// 0040c
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00410
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00414
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00418
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0041c
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00420
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00424
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00428
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 0042c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00430
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00434
			 0xffc00001,                                                  // -signaling NaN                              /// 00438
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0043c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00440
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00444
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00448
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 0044c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00450
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00454
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00458
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0045c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00460
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00464
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00468
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0046c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00470
			 0x0c700000,                                                  // Leading 1s:                                 /// 00474
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00478
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0047c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00480
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00484
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00488
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 0048c
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00490
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00494
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00498
			 0x4b000000,                                                  // 8388608.0                                   /// 0049c
			 0xcb000000,                                                  // -8388608.0                                  /// 004a0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 004a4
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 004a8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 004ac
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 004b0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 004b4
			 0xff7ffffe, // max norm - 1ulp                               // max norm -ve                                /// 004b8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 004bc
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004c0
			 0xffc00001,                                                  // -signaling NaN                              /// 004c4
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 004c8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 004cc
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 004d0
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 004d4
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004d8
			 0xbf028f5c,                                                  // -0.51                                       /// 004dc
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 004e0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 004e4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 004e8
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004ec
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f0
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 004f8
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 004fc
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00500
			 0x0c400000,                                                  // Leading 1s:                                 /// 00504
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00508
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0050c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00510
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00514
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00518
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0051c
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 00520
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00524
			 0x0e03ffff,                                                  // Trailing 1s:                                /// 00528
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 0052c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00530
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00534
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00538
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0053c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00540
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00544
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00548
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0054c
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00550
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00554
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00558
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 0055c
			 0x55555555,                                                  // 4 random values                             /// 00560
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00564
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00568
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 0056c
			 0x7f800000,                                                  // inf                                         /// 00570
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00574
			 0xff800000,                                                  // -inf                                        /// 00578
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0057c
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00580
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00584
			 0x0e000003,                                                  // Trailing 1s:                                /// 00588
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0058c
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00590
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00594
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00598
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0059c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 005a0
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 005a4
			 0x7fc00001,                                                  // signaling NaN                               /// 005a8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005ac
			 0x0e00001f,                                                  // Trailing 1s:                                /// 005b0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005b4
			 0x0c780000,                                                  // Leading 1s:                                 /// 005b8
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 005bc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 005c0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 005c4
			 0x8f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 005c8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005cc
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005d0
			 0x8f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 005d4
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 005d8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 005dc
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 005e0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 005e4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 005e8
			 0x0e007fff,                                                  // Trailing 1s:                                /// 005ec
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 005f0
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 005f4
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 005f8
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 005fc
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00600
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00604
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 00608
			 0x33333333,                                                  // 4 random values                             /// 0060c
			 0xbf028f5c,                                                  // -0.51                                       /// 00610
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00614
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00618
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 0061c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00620
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00624
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00628
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0062c
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00630
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00634
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00638
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 0063c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00640
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00644
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00648
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 0064c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00650
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00654
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00658
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 0065c
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00660
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00664
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00668
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 0066c
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00670
			 0x80011111,                                                  // -9.7958E-41                                 /// 00674
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00678
			 0x0e00001f,                                                  // Trailing 1s:                                /// 0067c
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00680
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00684
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00688
			 0x80011111,                                                  // -9.7958E-41                                 /// 0068c
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00690
			 0x0c7ffe00,                                                  // Leading 1s:                                 /// 00694
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00698
			 0x0c600000,                                                  // Leading 1s:                                 /// 0069c
			 0x7f800000,                                                  // inf                                         /// 006a0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 006a4
			 0x0e00007f,                                                  // Trailing 1s:                                /// 006a8
			 0x7fc00001,                                                  // signaling NaN                               /// 006ac
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 006b0
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006b4
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 006b8
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 006bc
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006c0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 006c4
			 0x00000000,                                                  // zero                                        /// 006c8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 006cc
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006d0
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 006d4
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006d8
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 006dc
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 006e0
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006e4
			 0x00008000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 006e8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 006ec
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 006f0
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 006f4
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 006f8
			 0x0c7fffc0,                                                  // Leading 1s:                                 /// 006fc
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00700
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00704
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00708
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 0070c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00710
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00714
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00718
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0071c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00720
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00724
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00728
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0072c
			 0x00000000,                                                  // zero                                        /// 00730
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00734
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00738
			 0x0d000ff0,                                                  // Set of 1s                                   /// 0073c
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00740
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00744
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00748
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 0074c
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00750
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00754
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00758
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 0075c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00760
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00764
			 0x00000000,                                                  // zero                                        /// 00768
			 0x7fc00000,                                                  // cquiet NaN                                  /// 0076c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00770
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00774
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00778
			 0x80000000, // 0                                             // SP - ve numbers                             /// 0077c
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00780
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00784
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00788
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 0078c
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00790
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00794
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00798
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 0079c
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007a0
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 007a4
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 007a8
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 007ac
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 007b0
			 0x00000000,                                                  // zero                                        /// 007b4
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007b8
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 007bc
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 007c0
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 007c4
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 007c8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 007cc
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007d4
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 007d8
			 0x00004000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 007dc
			 0x0c780000,                                                  // Leading 1s:                                 /// 007e0
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 007e4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 007e8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 007ec
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007f0
			 0xff800000,                                                  // -inf                                        /// 007f4
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 007f8
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 007fc
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00800
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00804
			 0x4b000000,                                                  // 8388608.0                                   /// 00808
			 0x0e00007f,                                                  // Trailing 1s:                                /// 0080c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00810
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00814
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00818
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 0081c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00820
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00824
			 0x0e000003,                                                  // Trailing 1s:                                /// 00828
			 0x0c600000,                                                  // Leading 1s:                                 /// 0082c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00830
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00834
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00838
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 0083c
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00840
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00844
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00848
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 0084c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00850
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00854
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00858
			 0x0c700000,                                                  // Leading 1s:                                 /// 0085c
			 0x0e007fff,                                                  // Trailing 1s:                                /// 00860
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00864
			 0xcb000000,                                                  // -8388608.0                                  /// 00868
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 0086c
			 0xbf028f5c,                                                  // -0.51                                       /// 00870
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00874
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00878
			 0x00011111,                                                  // 9.7958E-41                                  /// 0087c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00880
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00884
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00888
			 0x00800000, // min norm                                      // subnormals +ve                              /// 0088c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00890
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00894
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00898
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 0089c
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 008a0
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 008a4
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 008a8
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 008ac
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008b0
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 008b4
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008b8
			 0x0e003fff,                                                  // Trailing 1s:                                /// 008bc
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 008c0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 008c8
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 008cc
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 008d0
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 008d4
			 0x3f028f5c,                                                  // 0.51                                        /// 008d8
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 008dc
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 008e0
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 008e4
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 008e8
			 0x0e000fff,                                                  // Trailing 1s:                                /// 008ec
			 0xff7fffff, // max norm                                      // SP - ve numbers                             /// 008f0
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 008f4
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 008f8
			 0xff000000, // norm with max exp, min mant                   // SP - ve numbers                             /// 008fc
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00900
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00904
			 0x80000004,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00908
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 0090c
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00910
			 0x00040000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00914
			 0x55555555,                                                  // 4 random values                             /// 00918
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0091c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00920
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00924
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00928
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0092c
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00930
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00934
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00938
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 0093c
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00940
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00944
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00948
			 0xffc00001,                                                  // -signaling NaN                              /// 0094c
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00950
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00954
			 0x80000002,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00958
			 0x0e000003,                                                  // Trailing 1s:                                /// 0095c
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00960
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00964
			 0x7f800000,                                                  // inf                                         /// 00968
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 0096c
			 0x80011111,                                                  // -9.7958E-41                                 /// 00970
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00974
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00978
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 0097c
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00980
			 0x0e0fffff,                                                  // Trailing 1s:                                /// 00984
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00988
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 0098c
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00990
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00994
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00998
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 0099c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 009a0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 009a4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009a8
			 0x55555555,                                                  // 4 random values                             /// 009ac
			 0x807ffffe, // max subnorm - 1ulp                            // SP - ve numbers                             /// 009b0
			 0x3f028f5c,                                                  // 0.51                                        /// 009b4
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 009b8
			 0x00011111,                                                  // 9.7958E-41                                  /// 009bc
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 009c0
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 009c4
			 0x0e00003f,                                                  // Trailing 1s:                                /// 009c8
			 0xcb000000,                                                  // -8388608.0                                  /// 009cc
			 0x0c400000,                                                  // Leading 1s:                                 /// 009d0
			 0x7fc00001,                                                  // signaling NaN                               /// 009d4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 009d8
			 0x0d00fff0,                                                  // Set of 1s                                   /// 009dc
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009e0
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 009e4
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 009e8
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 009ec
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 009f0
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 009f4
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 009f8
			 0x0e00007f,                                                  // Trailing 1s:                                /// 009fc
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00a00
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a04
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a08
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a0c
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00a10
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a14
			 0xff7ffffe, // max norm - 1ulp                               // SP - ve numbers                             /// 00a18
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00a1c
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00a20
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a24
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a28
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00a2c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00a30
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00a34
			 0x0d000ff0,                                                  // Set of 1s                                   /// 00a38
			 0x80ffffff, // norm with min exp, max mant                   // SP - ve numbers                             /// 00a3c
			 0x0c7ffffc,                                                  // Leading 1s:                                 /// 00a40
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a44
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a48
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a4c
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00a50
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a54
			 0x00000004,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a58
			 0x4b8c4b40,                                                  // 18388608.0                                  /// 00a5c
			 0x7f800000, // infinity                                      // SP +ve numbers                              /// 00a60
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a64
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00a68
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00a6c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a70
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00a74
			 0x00800002, // min norm + 2ulp                               // subnormals +ve                              /// 00a78
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a7c
			 0x0e0001ff,                                                  // Trailing 1s:                                /// 00a80
			 0x80000000,                                                  // -zero                                       /// 00a84
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00a88
			 0x00000000,                                                  // zero                                        /// 00a8c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00a90
			 0xffc00001,                                                  // -signaling NaN                              /// 00a94
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00a98
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00a9c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00aa0
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00aa4
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00aa8
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00aac
			 0x0c600000,                                                  // Leading 1s:                                 /// 00ab0
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00ab4
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ab8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00abc
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ac0
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ac4
			 0xbf028f5c,                                                  // -0.51                                       /// 00ac8
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00acc
			 0x0c7f8000,                                                  // Leading 1s:                                 /// 00ad0
			 0x80200000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ad4
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00ad8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00adc
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00ae0
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00ae4
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ae8
			 0x807fffff, // max subnorm                                   // SP - ve numbers                             /// 00aec
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00af0
			 0x0e0007ff,                                                  // Trailing 1s:                                /// 00af4
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00af8
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00afc
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00b00
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b04
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00b08
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00b0c
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00b10
			 0x00000400,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b14
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b18
			 0x80000002, // min subnorm + 1 ulp                           // SP - ve numbers                             /// 00b1c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00b20
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00b24
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00b28
			 0x80000400,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b2c
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00b30
			 0xff800000,                                                  // -inf                                        /// 00b34
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00b38
			 0x0c780000,                                                  // Leading 1s:                                 /// 00b3c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b40
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00b44
			 0x00000000,                                                  // zero                                        /// 00b48
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b4c
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b50
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00b54
			 0x80011111,                                                  // -9.7958E-41                                 /// 00b58
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00b5c
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00b60
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00b64
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00b68
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b6c
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00b70
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00b74
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00b78
			 0x00000002, // min subnorm + 1 ulp                           // SP +ve numbers                              /// 00b7c
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00b80
			 0x0e000003,                                                  // Trailing 1s:                                /// 00b84
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00b88
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00b8c
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00b90
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00b94
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00b98
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00b9c
			 0x0c400000,                                                  // Leading 1s:                                 /// 00ba0
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00ba4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ba8
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00bac
			 0x00800003,                                                  // none of the mantissa set to +3ulp           /// 00bb0
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00bb4
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00bb8
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00bbc
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00bc0
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bc4
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00bc8
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00bcc
			 0x0e001fff,                                                  // Trailing 1s:                                /// 00bd0
			 0x00000000,                                                  // zero                                        /// 00bd4
			 0x80040000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00bd8
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00bdc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be0
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00be4
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00be8
			 0xbf028f5c,                                                  // -0.51                                       /// 00bec
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00bf0
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00bf4
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bf8
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00bfc
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00c00
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00c04
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00c08
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c0c
			 0x80000800,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c10
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00c14
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00c18
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00c1c
			 0x7fc00001,                                                  // signaling NaN                               /// 00c20
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c24
			 0x0f7fffff,                                                  // all bit of mantissa set upto -3ulp          /// 00c28
			 0x7fc00000,                                                  // cquiet NaN                                  /// 00c2c
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c30
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c34
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00c38
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c3c
			 0x00200000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c40
			 0x0e000001,                                                  // Trailing 1s:                                /// 00c44
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00c48
			 0x0c7ff800,                                                  // Leading 1s:                                 /// 00c4c
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00c50
			 0x007fffff, // max subnorm                                   // SP +ve numbers                              /// 00c54
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00c58
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00c5c
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c60
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00c64
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00c68
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00c6c
			 0x80000001,                                                  // -1.4E-45 (-denorm)                          /// 00c70
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00c74
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00c78
			 0x0e00ffff,                                                  // Trailing 1s:                                /// 00c7c
			 0x7fffffff, // QNan                                          // SP +ve numbers                              /// 00c80
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c84
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00c88
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00c8c
			 0x80800000, // min norm                                      // subnormals -ve                              /// 00c90
			 0xffffffff, // QNan                                          // SP - ve numbers                             /// 00c94
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00c98
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00c9c
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00ca0
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ca4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ca8
			 0x0e000007,                                                  // Trailing 1s:                                /// 00cac
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00cb0
			 0x00800001, // min norm + 1ulp                               // SP +ve numbers                              /// 00cb4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cb8
			 0x0e00000f,                                                  // Trailing 1s:                                /// 00cbc
			 0x0e000003,                                                  // Trailing 1s:                                /// 00cc0
			 0xff800000,                                                  // -inf                                        /// 00cc4
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00cc8
			 0x0c7ffff8,                                                  // Leading 1s:                                 /// 00ccc
			 0xCCCCCCCC,                                                  // 4 random values                             /// 00cd0
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00cd4
			 0x00000200,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cd8
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cdc
			 0x00000100,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ce0
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00ce4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00ce8
			 0x4b000000,                                                  // 8388608.0                                   /// 00cec
			 0x00011111,                                                  // 9.7958E-41                                  /// 00cf0
			 0xAAAAAAAA,                                                  // 4 random values                             /// 00cf4
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00cf8
			 0x0c7fff00,                                                  // Leading 1s:                                 /// 00cfc
			 0x00ffffff, // norm with min exp, max mant                   // SP +ve numbers                              /// 00d00
			 0x0f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00d04
			 0x00000000,                                                  // zero                                        /// 00d08
			 0x3f800000, // 1                                             // SP +ve numbers                              /// 00d0c
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00d10
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d14
			 0x0c7f0000,                                                  // Leading 1s:                                 /// 00d18
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d1c
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d20
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00d24
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00d28
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d2c
			 0x80000008,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d30
			 0x00002000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d34
			 0x0e0003ff,                                                  // Trailing 1s:                                /// 00d38
			 0x80000020,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d3c
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00d40
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d44
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00d48
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00d4c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d50
			 0x00080000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d54
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00d58
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d5c
			 0xff7ffffe, // max norm - 2ulp                               // max norm -ve                                /// 00d60
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d64
			 0x80011111,                                                  // -9.7958E-41                                 /// 00d68
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d6c
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00d70
			 0x00800001, // min norm + 1ulp                               // subnormals +ve                              /// 00d74
			 0x80800000,                                                  // none of the mantissa set to +3ulp           /// 00d78
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00d7c
			 0x80000001,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d80
			 0x00000040,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00d84
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00d88
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00d8c
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d90
			 0x0e000003,                                                  // Trailing 1s:                                /// 00d94
			 0x80020000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00d98
			 0x0c7ff000,                                                  // Leading 1s:                                 /// 00d9c
			 0x0c7fff80,                                                  // Leading 1s:                                 /// 00da0
			 0x7f7ffffe, // max norm - 1ulp                               // max norm +ve                                /// 00da4
			 0x00800000, // min norm                                      // subnormals +ve                              /// 00da8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00dac
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00db0
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00db4
			 0xffbfffff, // SNaN                                          // SP - ve numbers                             /// 00db8
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00dbc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00dc0
			 0x80800001, // min norm + 1ulp                               // subnormals -ve                              /// 00dc4
			 0x0c7fc000,                                                  // Leading 1s:                                 /// 00dc8
			 0x0c7fffe0,                                                  // Leading 1s:                                 /// 00dcc
			 0x80002000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd0
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00dd4
			 0x80800002, // min norm + 2ulp                               // subnormals -ve                              /// 00dd8
			 0x0c7ffff0,                                                  // Leading 1s:                                 /// 00ddc
			 0x0e7fffff,                                                  // Trailing 1s:                                /// 00de0
			 0x0e3fffff,                                                  // Trailing 1s:                                /// 00de4
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00de8
			 0x80000001, // min subnorm                                   // SP - ve numbers                             /// 00dec
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df0
			 0x0e000001,                                                  // Trailing 1s:                                /// 00df4
			 0x80001000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00df8
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00dfc
			 0x80011111,                                                  // -9.7958E-41                                 /// 00e00
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e04
			 0x00000002,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e08
			 0x00000001,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e0c
			 0x00800002,                                                  // none of the mantissa set to +3ulp           /// 00e10
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00e14
			 0x00800000, // min norm                                      // SP +ve numbers                              /// 00e18
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00e1c
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e20
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00e24
			 0xff800000,                                                  // -inf                                        /// 00e28
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e2c
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e30
			 0x80004000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e34
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e38
			 0x80100000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e3c
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00e40
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e44
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00e48
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00e4c
			 0x7fbfffff, // SNaN                                          // SP +ve numbers                              /// 00e50
			 0x7fc00000, // DefaultNan                                    // SP +ve numbers                              /// 00e54
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00e58
			 0xbf028f5c,                                                  // -0.51                                       /// 00e5c
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00e60
			 0x0d00fff0,                                                  // Set of 1s                                   /// 00e64
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e68
			 0x00000800,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00e6c
			 0xff800001, // SNaN                                          // SP - ve numbers                             /// 00e70
			 0x007ffffe, // max subnorm - 1ulp                            // SP +ve numbers                              /// 00e74
			 0x0c7fffff,                                                  // Leading 1s:                                 /// 00e78
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00e7c
			 0x80010000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00e80
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00e84
			 0x0c700000,                                                  // Leading 1s:                                 /// 00e88
			 0x00011111,                                                  // 9.7958E-41                                  /// 00e8c
			 0x00000001,                                                  // 1.4E-45 (+denorm)                           /// 00e90
			 0xbf028f5c,                                                  // -0.51                                       /// 00e94
			 0x0e000fff,                                                  // Trailing 1s:                                /// 00e98
			 0x80800003,                                                  // none of the mantissa set to +3ulp           /// 00e9c
			 0x0f7ffffd,                                                  // all bit of mantissa set upto -3ulp          /// 00ea0
			 0x00000080,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ea4
			 0x7f7ffffe, // max norm - 1ulp                               // SP +ve numbers                              /// 00ea8
			 0x00000001, // min subnorm                                   // SP +ve numbers                              /// 00eac
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00eb0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00eb4
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00eb8
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00ebc
			 0x80800001, // min norm + 1ulp                               // SP - ve numbers                             /// 00ec0
			 0x80000010,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ec4
			 0x00100000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ec8
			 0x80800002,                                                  // none of the mantissa set to +3ulp           /// 00ecc
			 0xcb000000,                                                  // -8388608.0                                  /// 00ed0
			 0xff7fffff, // max norm                                      // max norm -ve                                /// 00ed4
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00ed8
			 0x00011111,                                                  // 9.7958E-41                                  /// 00edc
			 0x0e00001f,                                                  // Trailing 1s:                                /// 00ee0
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ee4
			 0x0c7ffc00,                                                  // Leading 1s:                                 /// 00ee8
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00eec
			 0xbf800001, // 1  + 1ulp                                     // SP - ve numbers                             /// 00ef0
			 0x7fc00001,                                                  // signaling NaN                               /// 00ef4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00ef8
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00efc
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00f00
			 0x0e003fff,                                                  // Trailing 1s:                                /// 00f04
			 0x0e0000ff,                                                  // Trailing 1s:                                /// 00f08
			 0x80800003, // min norm + 3ulp                               // subnormals -ve                              /// 00f0c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f10
			 0xbf800000, // 1                                             // SP - ve numbers                             /// 00f14
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00f18
			 0x00001000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f1c
			 0xffc00000,                                                  // -cquiet NaN                                 /// 00f20
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f24
			 0x7f000000, // norm with max exp, min mant                   // SP +ve numbers                              /// 00f28
			 0x0e00003f,                                                  // Trailing 1s:                                /// 00f2c
			 0x0e1fffff,                                                  // Trailing 1s:                                /// 00f30
			 0x00000010,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f34
			 0xff7ffffe, // max norm - 3ulp                               // max norm -ve                                /// 00f38
			 0x0c400000,                                                  // Leading 1s:                                 /// 00f3c
			 0x80000200,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f40
			 0x0c7ffffe,                                                  // Leading 1s:                                 /// 00f44
			 0x0c780000,                                                  // Leading 1s:                                 /// 00f48
			 0x0e000001,                                                  // Trailing 1s:                                /// 00f4c
			 0x00011111,                                                  // 9.7958E-41                                  /// 00f50
			 0x0f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00f54
			 0xcb8c4b40,                                                  // -18388608.0                                 /// 00f58
			 0x80000100,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f5c
			 0xff800000, // infinity                                      // SP - ve numbers                             /// 00f60
			 0x80800001,                                                  // none of the mantissa set to +3ulp           /// 00f64
			 0x00800001,                                                  // none of the mantissa set to +3ulp           /// 00f68
			 0x00010000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f6c
			 0x80000080,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f70
			 0xcb000000,                                                  // -8388608.0                                  /// 00f74
			 0xffc00001, // QNan                                          // SP - ve numbers                             /// 00f78
			 0x7f7ffffe, // max norm - 2ulp                               // max norm +ve                                /// 00f7c
			 0x007fffff,                                                  // 1.1754942E-38                               /// 00f80
			 0x0c7c0000,                                                  // Leading 1s:                                 /// 00f84
			 0x80011111,                                                  // -9.7958E-41                                 /// 00f88
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00f8c
			 0x7f800000,                                                  // inf                                         /// 00f90
			 0x00020000,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00f94
			 0x7fc00001, // QNan                                          // SP +ve numbers                              /// 00f98
			 0x00000000, // 0                                             // SP +ve numbers                              /// 00f9c
			 0x7f7fffff, // max norm                                      // max norm +ve                                /// 00fa0
			 0x00800000,                                                  // none of the mantissa set to +3ulp           /// 00fa4
			 0x7f7fffff, // max norm                                      // SP +ve numbers                              /// 00fa8
			 0x8f7ffffc,                                                  // all bit of mantissa set upto -3ulp          /// 00fac
			 0x0e00007f,                                                  // Trailing 1s:                                /// 00fb0
			 0x00000008,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fb4
			 0x00800003, // min norm + 3ulp                               // subnormals +ve                              /// 00fb8
			 0x80008000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fbc
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00fc0
			 0x80000040,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fc4
			 0x0c780000,                                                  // Leading 1s:                                 /// 00fc8
			 0xffc00000, // DefaultNan                                    // SP - ve numbers                             /// 00fcc
			 0x00400000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fd0
			 0x80000000, // 0                                             // SP - ve numbers                             /// 00fd4
			 0x0c7e0000,                                                  // Leading 1s:                                 /// 00fd8
			 0x0c400000,                                                  // Leading 1s:                                 /// 00fdc
			 0x00000020,                                                  // SP - 1 bit alone set in mantissa +ve        /// 00fe0
			 0x0e07ffff,                                                  // Trailing 1s:                                /// 00fe4
			 0x80080000,                                                  // SP - 1 bit alone set in mantissa -ve        /// 00fe8
			 0x55555555,                                                  // 4 random values                             /// 00fec
			 0x8f7ffffe,                                                  // all bit of mantissa set upto -3ulp          /// 00ff0
			 0x80011111,                                                  // -9.7958E-41                                 /// 00ff4
			 0x0c7fe000,                                                  // Leading 1s:                                 /// 00ff8
			 0x7f7fffff // max norm                                      // SP +ve numbers                              /// last
	};
	volatile uint32_t m_15[1024] __attribute__ ((aligned (4096))) = {
			 0xde50c2ed, /// 0x0
			 0x3e3928f1, /// 0x4
			 0xa1519828, /// 0x8
			 0x35d98c10, /// 0xc
			 0x362c79ff, /// 0x10
			 0xef130934, /// 0x14
			 0xd80b0ed2, /// 0x18
			 0xdbc9dd70, /// 0x1c
			 0xccd149c6, /// 0x20
			 0x934c5c99, /// 0x24
			 0xa0f8e6b0, /// 0x28
			 0xf5cc4749, /// 0x2c
			 0x69d3db29, /// 0x30
			 0xc4567087, /// 0x34
			 0xebf9eb06, /// 0x38
			 0x0367551d, /// 0x3c
			 0xd8ac566b, /// 0x40
			 0x746c8694, /// 0x44
			 0x0ac6d684, /// 0x48
			 0xda4d15de, /// 0x4c
			 0x7040f1eb, /// 0x50
			 0x7fd128d6, /// 0x54
			 0xdc883dc3, /// 0x58
			 0xc6fec2e3, /// 0x5c
			 0xf5ca440b, /// 0x60
			 0x28ec359a, /// 0x64
			 0xa9568a1c, /// 0x68
			 0x9096b4ac, /// 0x6c
			 0xa9e736b1, /// 0x70
			 0x8d75fa6f, /// 0x74
			 0x8d569adb, /// 0x78
			 0xa58e60df, /// 0x7c
			 0x9ca9915e, /// 0x80
			 0x0697a9bb, /// 0x84
			 0x6ca507ff, /// 0x88
			 0xe8505ab3, /// 0x8c
			 0x7605266d, /// 0x90
			 0x74c1fa97, /// 0x94
			 0x53f31335, /// 0x98
			 0x6fb19bb8, /// 0x9c
			 0x31c10e88, /// 0xa0
			 0x84913b97, /// 0xa4
			 0xb2e690d3, /// 0xa8
			 0x124eff27, /// 0xac
			 0xaba2faca, /// 0xb0
			 0x7304c5f5, /// 0xb4
			 0x2322e953, /// 0xb8
			 0xca95634e, /// 0xbc
			 0xf9a7425c, /// 0xc0
			 0xf3d6c217, /// 0xc4
			 0x392f970c, /// 0xc8
			 0x4c05afa0, /// 0xcc
			 0x9cbe487f, /// 0xd0
			 0xb2dda245, /// 0xd4
			 0xbd479459, /// 0xd8
			 0x93ee19c3, /// 0xdc
			 0x3f1dd7d9, /// 0xe0
			 0x30576fda, /// 0xe4
			 0x4ad1459c, /// 0xe8
			 0x907559a7, /// 0xec
			 0x14929206, /// 0xf0
			 0x6df6f0ec, /// 0xf4
			 0xa7a5a5fe, /// 0xf8
			 0x76be2e56, /// 0xfc
			 0x46331737, /// 0x100
			 0x5b570a6b, /// 0x104
			 0x86688510, /// 0x108
			 0x42b10c11, /// 0x10c
			 0x0f01765b, /// 0x110
			 0xbbb839c1, /// 0x114
			 0x9a046ced, /// 0x118
			 0x05526954, /// 0x11c
			 0x16aa2e61, /// 0x120
			 0x5cacd745, /// 0x124
			 0x69ee77f6, /// 0x128
			 0x1c347112, /// 0x12c
			 0xdfeca0f3, /// 0x130
			 0xabf8f566, /// 0x134
			 0xa1792e24, /// 0x138
			 0xa3aa2b84, /// 0x13c
			 0xbcf51552, /// 0x140
			 0x5d28cdaa, /// 0x144
			 0x7be9b083, /// 0x148
			 0x03fc97d5, /// 0x14c
			 0xca223932, /// 0x150
			 0x2e0b1f60, /// 0x154
			 0x5a1d52a7, /// 0x158
			 0x62af9fd7, /// 0x15c
			 0x095eff4a, /// 0x160
			 0x47f3899d, /// 0x164
			 0x85b4a7f0, /// 0x168
			 0x65a9e89b, /// 0x16c
			 0xdc64c91e, /// 0x170
			 0xb7d1f8dc, /// 0x174
			 0x87693cae, /// 0x178
			 0x988b813e, /// 0x17c
			 0xdf535e8e, /// 0x180
			 0xc853405a, /// 0x184
			 0x88994e74, /// 0x188
			 0xb22c86c6, /// 0x18c
			 0xc6a60ad6, /// 0x190
			 0x994a2602, /// 0x194
			 0x9f69e3f4, /// 0x198
			 0xc7edad2c, /// 0x19c
			 0x42bea028, /// 0x1a0
			 0xe5f8da9c, /// 0x1a4
			 0x4f849e84, /// 0x1a8
			 0x6ade3b2e, /// 0x1ac
			 0x72c5a14f, /// 0x1b0
			 0x72da84f8, /// 0x1b4
			 0x092ff3cd, /// 0x1b8
			 0xe0972de0, /// 0x1bc
			 0xdbfdb511, /// 0x1c0
			 0xb191c4d6, /// 0x1c4
			 0xc871a843, /// 0x1c8
			 0xa93ee9f2, /// 0x1cc
			 0x2327cd30, /// 0x1d0
			 0xbfc4a518, /// 0x1d4
			 0x30d1acbb, /// 0x1d8
			 0x12af17d6, /// 0x1dc
			 0xa2202ccf, /// 0x1e0
			 0x31610865, /// 0x1e4
			 0xdf2a9cb0, /// 0x1e8
			 0x0a311dc9, /// 0x1ec
			 0xe823bf54, /// 0x1f0
			 0x639bed29, /// 0x1f4
			 0x64a9e581, /// 0x1f8
			 0x7bda5f4d, /// 0x1fc
			 0x8f347f04, /// 0x200
			 0x9e283b5f, /// 0x204
			 0xc5f3dfa3, /// 0x208
			 0xaf4a2652, /// 0x20c
			 0xf4d90040, /// 0x210
			 0x2b786a30, /// 0x214
			 0x18047aa6, /// 0x218
			 0x722a354a, /// 0x21c
			 0xdb2e4dcd, /// 0x220
			 0x53ee62b8, /// 0x224
			 0x8dcd0b64, /// 0x228
			 0x23c911aa, /// 0x22c
			 0xbb3e52e9, /// 0x230
			 0x76c77aae, /// 0x234
			 0xa8ca4386, /// 0x238
			 0xf9ea9cd6, /// 0x23c
			 0x8ac90136, /// 0x240
			 0x4f1c5f0f, /// 0x244
			 0x5e59b017, /// 0x248
			 0xba3adffb, /// 0x24c
			 0x628af005, /// 0x250
			 0xe8d4744a, /// 0x254
			 0x10fbf7a8, /// 0x258
			 0xffadb0e8, /// 0x25c
			 0x39a80d01, /// 0x260
			 0x32563a4a, /// 0x264
			 0xc3b41e13, /// 0x268
			 0x717d9baa, /// 0x26c
			 0xa41dbedf, /// 0x270
			 0xc5df3f88, /// 0x274
			 0x0a16d802, /// 0x278
			 0x3b3c8df5, /// 0x27c
			 0xd65fb165, /// 0x280
			 0x336b1dae, /// 0x284
			 0x94f1a63e, /// 0x288
			 0x569b6329, /// 0x28c
			 0x48cd46b3, /// 0x290
			 0x1b20da2a, /// 0x294
			 0xff93a695, /// 0x298
			 0x5f5b61c1, /// 0x29c
			 0xce6e427c, /// 0x2a0
			 0xa02a3f62, /// 0x2a4
			 0x698dc247, /// 0x2a8
			 0xfcbe3a61, /// 0x2ac
			 0x36b2cd00, /// 0x2b0
			 0xafd5ed1a, /// 0x2b4
			 0x48bd043f, /// 0x2b8
			 0xd44ec099, /// 0x2bc
			 0x3bf07440, /// 0x2c0
			 0xcaa7104d, /// 0x2c4
			 0xc0c96c86, /// 0x2c8
			 0x02425717, /// 0x2cc
			 0xae20c72e, /// 0x2d0
			 0x88cb88e4, /// 0x2d4
			 0x57f39b39, /// 0x2d8
			 0xf2afd032, /// 0x2dc
			 0x1d450b70, /// 0x2e0
			 0xe170f96a, /// 0x2e4
			 0xdf59b2d8, /// 0x2e8
			 0x1e213689, /// 0x2ec
			 0x87a7a39d, /// 0x2f0
			 0x41e9acbb, /// 0x2f4
			 0x525c7baf, /// 0x2f8
			 0xd5839ffd, /// 0x2fc
			 0x5fa41970, /// 0x300
			 0x031335b3, /// 0x304
			 0x730f655e, /// 0x308
			 0x2dd0faa0, /// 0x30c
			 0xb4dfedc4, /// 0x310
			 0xe35b644c, /// 0x314
			 0xf705376e, /// 0x318
			 0xed3b847d, /// 0x31c
			 0x6ec37305, /// 0x320
			 0xed50d2af, /// 0x324
			 0xf9113652, /// 0x328
			 0x6aa54a40, /// 0x32c
			 0xf5f5fc35, /// 0x330
			 0x465f0418, /// 0x334
			 0x70380afe, /// 0x338
			 0x083d74e0, /// 0x33c
			 0x61721487, /// 0x340
			 0xde4c6a30, /// 0x344
			 0xf32ad72b, /// 0x348
			 0xd1621389, /// 0x34c
			 0x8cad3446, /// 0x350
			 0xbff04c20, /// 0x354
			 0x3da5aa57, /// 0x358
			 0x353106c7, /// 0x35c
			 0x2caa2719, /// 0x360
			 0xc70daf84, /// 0x364
			 0x77b7119f, /// 0x368
			 0xae73826c, /// 0x36c
			 0xdf151647, /// 0x370
			 0xdce4eb90, /// 0x374
			 0xb6c9b948, /// 0x378
			 0x511cc587, /// 0x37c
			 0xed2d8250, /// 0x380
			 0x8a75e052, /// 0x384
			 0x33adcc40, /// 0x388
			 0x5060009c, /// 0x38c
			 0x8597f9c1, /// 0x390
			 0x6b8c81b8, /// 0x394
			 0x35f2634a, /// 0x398
			 0x495e1aef, /// 0x39c
			 0x8e313997, /// 0x3a0
			 0x3afb12d9, /// 0x3a4
			 0x4e320b83, /// 0x3a8
			 0x1ae6bd36, /// 0x3ac
			 0xb2a357e5, /// 0x3b0
			 0x3aaa104c, /// 0x3b4
			 0x085f3d86, /// 0x3b8
			 0x7c39492f, /// 0x3bc
			 0xd91c5c58, /// 0x3c0
			 0x47ded0c1, /// 0x3c4
			 0x9ddfbde5, /// 0x3c8
			 0x53c55969, /// 0x3cc
			 0x692ba752, /// 0x3d0
			 0x5b955a57, /// 0x3d4
			 0x1f34a660, /// 0x3d8
			 0x7f1d8216, /// 0x3dc
			 0x2ed17392, /// 0x3e0
			 0x0ec2abb1, /// 0x3e4
			 0x9560c1f2, /// 0x3e8
			 0xb715d9e9, /// 0x3ec
			 0xe06fefab, /// 0x3f0
			 0x932d3c8a, /// 0x3f4
			 0x49e97698, /// 0x3f8
			 0x0868cf8e, /// 0x3fc
			 0xa923bc0d, /// 0x400
			 0x1b6196ff, /// 0x404
			 0xb94e5273, /// 0x408
			 0x5e45abd1, /// 0x40c
			 0x2eae993d, /// 0x410
			 0xcaafdf03, /// 0x414
			 0x9f08e1b7, /// 0x418
			 0x7d298e6b, /// 0x41c
			 0x316dbaa2, /// 0x420
			 0xa0de371c, /// 0x424
			 0xac4e67d8, /// 0x428
			 0x20134c21, /// 0x42c
			 0x88adcf38, /// 0x430
			 0xd557bbcd, /// 0x434
			 0xd9296c8a, /// 0x438
			 0x93ed6449, /// 0x43c
			 0xe64b8ff1, /// 0x440
			 0xc5db48ac, /// 0x444
			 0x60cb0785, /// 0x448
			 0xd90ffdd4, /// 0x44c
			 0xcdf6ceac, /// 0x450
			 0x5542d8d1, /// 0x454
			 0xbe4d27b2, /// 0x458
			 0x5f3a94e4, /// 0x45c
			 0x11f8560a, /// 0x460
			 0xf454efdb, /// 0x464
			 0xb74297cf, /// 0x468
			 0x8fd38aef, /// 0x46c
			 0xa40ce8df, /// 0x470
			 0xe96e2dd9, /// 0x474
			 0xd90d3c2a, /// 0x478
			 0x4d509fab, /// 0x47c
			 0xa6bb4fde, /// 0x480
			 0x8bb1bf1f, /// 0x484
			 0xa899182f, /// 0x488
			 0x46945c8c, /// 0x48c
			 0x121fdd9d, /// 0x490
			 0x8b95a288, /// 0x494
			 0xbb435201, /// 0x498
			 0x3f892915, /// 0x49c
			 0xe9336d62, /// 0x4a0
			 0x0127e26e, /// 0x4a4
			 0xf0e86de7, /// 0x4a8
			 0xe4f08c4c, /// 0x4ac
			 0xa82d4247, /// 0x4b0
			 0x7b678b3d, /// 0x4b4
			 0xd70ac7f3, /// 0x4b8
			 0x8f88e26d, /// 0x4bc
			 0x058a465c, /// 0x4c0
			 0x45c7fa9d, /// 0x4c4
			 0x555e9565, /// 0x4c8
			 0x933f6cf2, /// 0x4cc
			 0x92b8950a, /// 0x4d0
			 0x7426cfc5, /// 0x4d4
			 0x295833c9, /// 0x4d8
			 0x81c4be6a, /// 0x4dc
			 0x5929ced6, /// 0x4e0
			 0x50288c43, /// 0x4e4
			 0xd5989c1c, /// 0x4e8
			 0xbb4f71cc, /// 0x4ec
			 0x97937aa2, /// 0x4f0
			 0x58ebb98c, /// 0x4f4
			 0xb033a854, /// 0x4f8
			 0x782126aa, /// 0x4fc
			 0xb3455123, /// 0x500
			 0x929fe885, /// 0x504
			 0xcc41a2aa, /// 0x508
			 0xa6b59d51, /// 0x50c
			 0x48fd41a2, /// 0x510
			 0x03ab6b68, /// 0x514
			 0xd5d020e0, /// 0x518
			 0xffe058cb, /// 0x51c
			 0xcd8d443c, /// 0x520
			 0x86392781, /// 0x524
			 0x27eb5f6c, /// 0x528
			 0xfec11d29, /// 0x52c
			 0x311a4459, /// 0x530
			 0x8f20c2c5, /// 0x534
			 0x15fe1cc1, /// 0x538
			 0x2cbda02c, /// 0x53c
			 0xa102917c, /// 0x540
			 0x99494727, /// 0x544
			 0xadad0df3, /// 0x548
			 0x37c6fdfa, /// 0x54c
			 0x4210fb48, /// 0x550
			 0x76b83c60, /// 0x554
			 0xc3d39193, /// 0x558
			 0x1d4fff00, /// 0x55c
			 0x28f4a21c, /// 0x560
			 0xefa1c152, /// 0x564
			 0x3391b78b, /// 0x568
			 0x46b36616, /// 0x56c
			 0x4acf5cd8, /// 0x570
			 0x55c055b8, /// 0x574
			 0x564cd237, /// 0x578
			 0xee60eb68, /// 0x57c
			 0x72813f4b, /// 0x580
			 0x1ea40918, /// 0x584
			 0xb04c4d81, /// 0x588
			 0xab0fcfc1, /// 0x58c
			 0x7143ffb6, /// 0x590
			 0x82c0e335, /// 0x594
			 0x52a773ed, /// 0x598
			 0x5e9b67a8, /// 0x59c
			 0x0099a21c, /// 0x5a0
			 0x5524980b, /// 0x5a4
			 0xe356ec8d, /// 0x5a8
			 0xbeb9af8a, /// 0x5ac
			 0xb89c3c01, /// 0x5b0
			 0x81bfc0ab, /// 0x5b4
			 0x0be773bf, /// 0x5b8
			 0x482a0670, /// 0x5bc
			 0x714ffd7d, /// 0x5c0
			 0x39bd9432, /// 0x5c4
			 0x8e271ba5, /// 0x5c8
			 0xd80e238a, /// 0x5cc
			 0x35024b3c, /// 0x5d0
			 0xf276e654, /// 0x5d4
			 0xfcb6192b, /// 0x5d8
			 0x38e96c1d, /// 0x5dc
			 0xf45f6702, /// 0x5e0
			 0xbdbca669, /// 0x5e4
			 0x6a69a560, /// 0x5e8
			 0x940378fb, /// 0x5ec
			 0x6b21fc7c, /// 0x5f0
			 0xd16a0096, /// 0x5f4
			 0x67b81211, /// 0x5f8
			 0xb6029942, /// 0x5fc
			 0xd9aa87a3, /// 0x600
			 0x869303fa, /// 0x604
			 0xf2d12a3a, /// 0x608
			 0x5f21e54e, /// 0x60c
			 0xbcd23108, /// 0x610
			 0x927d15db, /// 0x614
			 0x09909503, /// 0x618
			 0x5606e47d, /// 0x61c
			 0xa0924fec, /// 0x620
			 0xa4a91b62, /// 0x624
			 0xf14703b6, /// 0x628
			 0x27fd34b4, /// 0x62c
			 0xcac7ac09, /// 0x630
			 0x7d807659, /// 0x634
			 0x6ec7896d, /// 0x638
			 0x21722368, /// 0x63c
			 0x393314ba, /// 0x640
			 0x91496417, /// 0x644
			 0x37ac2597, /// 0x648
			 0x3ebf200e, /// 0x64c
			 0x4fb54c57, /// 0x650
			 0xbf1a3fa8, /// 0x654
			 0x61546949, /// 0x658
			 0x722c49ff, /// 0x65c
			 0xd2de36f8, /// 0x660
			 0x81f1bcca, /// 0x664
			 0x79ae19f0, /// 0x668
			 0xf48c3ede, /// 0x66c
			 0xcc5b1d79, /// 0x670
			 0xebb363e4, /// 0x674
			 0x0b376f5d, /// 0x678
			 0x6a356d80, /// 0x67c
			 0xdc45dc81, /// 0x680
			 0xe72e93ed, /// 0x684
			 0xad9a3e4e, /// 0x688
			 0x8c7947f7, /// 0x68c
			 0x97db5954, /// 0x690
			 0xedd6c62a, /// 0x694
			 0x7c3ad592, /// 0x698
			 0x4509b30a, /// 0x69c
			 0xab0d3097, /// 0x6a0
			 0xcaf835a9, /// 0x6a4
			 0x4173b763, /// 0x6a8
			 0xfeecc698, /// 0x6ac
			 0x53ddaa8e, /// 0x6b0
			 0x896d4f2d, /// 0x6b4
			 0xd55ea72b, /// 0x6b8
			 0xbdc9af7b, /// 0x6bc
			 0xb8e86f3e, /// 0x6c0
			 0xefbe0d4d, /// 0x6c4
			 0xfaa98cf5, /// 0x6c8
			 0x72ed4825, /// 0x6cc
			 0x3d3d5dcd, /// 0x6d0
			 0x0ad53ea3, /// 0x6d4
			 0x4aab8a12, /// 0x6d8
			 0x190ff1c4, /// 0x6dc
			 0xc91ef44d, /// 0x6e0
			 0xcff8706e, /// 0x6e4
			 0xc47b7ec8, /// 0x6e8
			 0x0197007c, /// 0x6ec
			 0x049f7370, /// 0x6f0
			 0xa828acbe, /// 0x6f4
			 0x363052b2, /// 0x6f8
			 0xe9e13c2d, /// 0x6fc
			 0x3ab0a2c3, /// 0x700
			 0x12ff78ed, /// 0x704
			 0xce8950bf, /// 0x708
			 0x410ba0ba, /// 0x70c
			 0x90bc7c9d, /// 0x710
			 0x82509d2b, /// 0x714
			 0x6d9c6572, /// 0x718
			 0x4ff8095d, /// 0x71c
			 0xed7a17fe, /// 0x720
			 0x44c7df95, /// 0x724
			 0xeb5fad15, /// 0x728
			 0x5ada5a4e, /// 0x72c
			 0x9e81f17b, /// 0x730
			 0xcb98c1ea, /// 0x734
			 0x485f1012, /// 0x738
			 0x93fe44ad, /// 0x73c
			 0x3d4565e5, /// 0x740
			 0x75d6df4c, /// 0x744
			 0x754372b4, /// 0x748
			 0xec163c6a, /// 0x74c
			 0x04c4995c, /// 0x750
			 0x8567f204, /// 0x754
			 0x5cb85d0e, /// 0x758
			 0x64427407, /// 0x75c
			 0x7a960a96, /// 0x760
			 0x1f282387, /// 0x764
			 0x7927e1c9, /// 0x768
			 0xda75b844, /// 0x76c
			 0x6d12fb8f, /// 0x770
			 0x7c5bc1eb, /// 0x774
			 0x1332596b, /// 0x778
			 0x74a44203, /// 0x77c
			 0x497af625, /// 0x780
			 0x02382036, /// 0x784
			 0x3fa74112, /// 0x788
			 0xd167817d, /// 0x78c
			 0x73d71624, /// 0x790
			 0x14aca63e, /// 0x794
			 0x6943677f, /// 0x798
			 0x168f25a2, /// 0x79c
			 0xf27f098e, /// 0x7a0
			 0x08ddd427, /// 0x7a4
			 0xdea6f4b8, /// 0x7a8
			 0x06171094, /// 0x7ac
			 0x9da9bde3, /// 0x7b0
			 0x9c4a791e, /// 0x7b4
			 0x7e0c2ef2, /// 0x7b8
			 0x04ce3a79, /// 0x7bc
			 0x41e51087, /// 0x7c0
			 0x1bf2d6a1, /// 0x7c4
			 0x55f3b882, /// 0x7c8
			 0x2445bf2c, /// 0x7cc
			 0x2fc8fb9e, /// 0x7d0
			 0x45cde733, /// 0x7d4
			 0xf00b6789, /// 0x7d8
			 0xeefcdea8, /// 0x7dc
			 0xdf0dee7a, /// 0x7e0
			 0x9497c77e, /// 0x7e4
			 0x6c1ff6f8, /// 0x7e8
			 0xc78ee273, /// 0x7ec
			 0xc79105e6, /// 0x7f0
			 0xe1c75eb6, /// 0x7f4
			 0x88cadd11, /// 0x7f8
			 0x854f98ba, /// 0x7fc
			 0xf4a4f049, /// 0x800
			 0x3f95b893, /// 0x804
			 0x756e8899, /// 0x808
			 0x416c4503, /// 0x80c
			 0x91c8b535, /// 0x810
			 0xace2c4a0, /// 0x814
			 0x99f47bf2, /// 0x818
			 0x4e2bc833, /// 0x81c
			 0x8b3e83ab, /// 0x820
			 0xd9dc9420, /// 0x824
			 0x8ae2b0e7, /// 0x828
			 0x9c924da3, /// 0x82c
			 0x4c0f5eba, /// 0x830
			 0xdd1ea171, /// 0x834
			 0x89328e2f, /// 0x838
			 0x1ca4264f, /// 0x83c
			 0xc07716b8, /// 0x840
			 0x2907ce14, /// 0x844
			 0x506838b0, /// 0x848
			 0x2ae7233d, /// 0x84c
			 0x7a93b2e4, /// 0x850
			 0x5281d7b8, /// 0x854
			 0x23166eb5, /// 0x858
			 0x04021f14, /// 0x85c
			 0x6b14c482, /// 0x860
			 0x73e06291, /// 0x864
			 0x3e57fb3a, /// 0x868
			 0x387a277b, /// 0x86c
			 0x4c5faadf, /// 0x870
			 0xa59012f1, /// 0x874
			 0xf4aafd0c, /// 0x878
			 0x12886c4b, /// 0x87c
			 0xd27c6405, /// 0x880
			 0xdfbf94c8, /// 0x884
			 0xc6856ab7, /// 0x888
			 0xeef12465, /// 0x88c
			 0x3b17208c, /// 0x890
			 0xddf07f9b, /// 0x894
			 0xb5a2ab3d, /// 0x898
			 0x8f90a16f, /// 0x89c
			 0x441463c2, /// 0x8a0
			 0x41456b43, /// 0x8a4
			 0x46e66630, /// 0x8a8
			 0x3922e1f3, /// 0x8ac
			 0x91635bf5, /// 0x8b0
			 0x6fb220ae, /// 0x8b4
			 0x817ff68a, /// 0x8b8
			 0x1f6ef028, /// 0x8bc
			 0x30af6d5c, /// 0x8c0
			 0x2905e94b, /// 0x8c4
			 0x331c0eef, /// 0x8c8
			 0xbe3d0be5, /// 0x8cc
			 0x3aa375fa, /// 0x8d0
			 0xede40f8e, /// 0x8d4
			 0x88e6f61c, /// 0x8d8
			 0x238bf36a, /// 0x8dc
			 0xec3a3ff8, /// 0x8e0
			 0x9a9e1202, /// 0x8e4
			 0x6d6e4969, /// 0x8e8
			 0xd627fdbb, /// 0x8ec
			 0x288249db, /// 0x8f0
			 0xa3393d0c, /// 0x8f4
			 0xc609f10d, /// 0x8f8
			 0xba4d1fcb, /// 0x8fc
			 0x1955954f, /// 0x900
			 0xfc94957c, /// 0x904
			 0xb164b555, /// 0x908
			 0xeb284606, /// 0x90c
			 0x190177f5, /// 0x910
			 0x41c884b4, /// 0x914
			 0xe06183f5, /// 0x918
			 0x44c6c51f, /// 0x91c
			 0x86cae437, /// 0x920
			 0x7cac6ce6, /// 0x924
			 0xecfbc425, /// 0x928
			 0x56d1169f, /// 0x92c
			 0xb6b082d3, /// 0x930
			 0x787925f7, /// 0x934
			 0xa8f44539, /// 0x938
			 0x5391a291, /// 0x93c
			 0x90017e9b, /// 0x940
			 0x89b580d9, /// 0x944
			 0xbd8fc1ef, /// 0x948
			 0x45f4eb84, /// 0x94c
			 0xfb6ce518, /// 0x950
			 0x279ade49, /// 0x954
			 0xb5486ccf, /// 0x958
			 0xd92d3497, /// 0x95c
			 0xbaa0f673, /// 0x960
			 0x8c425686, /// 0x964
			 0x4ad50be6, /// 0x968
			 0xfab48064, /// 0x96c
			 0x5c532fa7, /// 0x970
			 0x81b6f886, /// 0x974
			 0xdc274314, /// 0x978
			 0x756ad7f0, /// 0x97c
			 0x547c0bab, /// 0x980
			 0x8a440650, /// 0x984
			 0x7b4d9c03, /// 0x988
			 0x6d8faf62, /// 0x98c
			 0x2bce0114, /// 0x990
			 0x1ace032c, /// 0x994
			 0x83f53192, /// 0x998
			 0x773957b5, /// 0x99c
			 0xac77e622, /// 0x9a0
			 0x7a30947d, /// 0x9a4
			 0x34c7c607, /// 0x9a8
			 0x17cbd727, /// 0x9ac
			 0x9582e657, /// 0x9b0
			 0x678bd466, /// 0x9b4
			 0xdf5cb0c3, /// 0x9b8
			 0x0fc191d6, /// 0x9bc
			 0x7c7c86c9, /// 0x9c0
			 0xb878b23c, /// 0x9c4
			 0xf678852e, /// 0x9c8
			 0x7b6a34b0, /// 0x9cc
			 0xda5b237e, /// 0x9d0
			 0x3e4a43e1, /// 0x9d4
			 0xb113e6fa, /// 0x9d8
			 0xee89a2ff, /// 0x9dc
			 0xd88f287f, /// 0x9e0
			 0x86b37520, /// 0x9e4
			 0x23e5bd2e, /// 0x9e8
			 0xb9a9b67e, /// 0x9ec
			 0x9995b1a8, /// 0x9f0
			 0x8bdb43c0, /// 0x9f4
			 0xa6a459f3, /// 0x9f8
			 0x6ded2c30, /// 0x9fc
			 0xc1b3d4c3, /// 0xa00
			 0x595365de, /// 0xa04
			 0xcab60654, /// 0xa08
			 0xa42dda31, /// 0xa0c
			 0x92730b99, /// 0xa10
			 0x1b75542a, /// 0xa14
			 0xf434cc11, /// 0xa18
			 0x01ca44f0, /// 0xa1c
			 0x0757970e, /// 0xa20
			 0x2ef32c62, /// 0xa24
			 0x4010846b, /// 0xa28
			 0x57b5faff, /// 0xa2c
			 0x226bdbf1, /// 0xa30
			 0x23621ca7, /// 0xa34
			 0xfd51d9ca, /// 0xa38
			 0xb969924f, /// 0xa3c
			 0x37bfd7ac, /// 0xa40
			 0x6c726023, /// 0xa44
			 0xed7d70d1, /// 0xa48
			 0x902c2653, /// 0xa4c
			 0xaba907c9, /// 0xa50
			 0xec39155e, /// 0xa54
			 0x6fcdf9d3, /// 0xa58
			 0x87beaa1d, /// 0xa5c
			 0xa7b64a34, /// 0xa60
			 0xf732f656, /// 0xa64
			 0x0befe762, /// 0xa68
			 0xa6cc06c6, /// 0xa6c
			 0xb3b2db7d, /// 0xa70
			 0x6cd8a06b, /// 0xa74
			 0x27c7f3cb, /// 0xa78
			 0xa0c3eea4, /// 0xa7c
			 0xd78ff0ac, /// 0xa80
			 0x8203a6a6, /// 0xa84
			 0x3f3075e6, /// 0xa88
			 0x0150de96, /// 0xa8c
			 0x25eb9625, /// 0xa90
			 0x32182998, /// 0xa94
			 0xae7eb0d8, /// 0xa98
			 0x4e3527bd, /// 0xa9c
			 0x0d19d660, /// 0xaa0
			 0x4e3115a8, /// 0xaa4
			 0x968cd911, /// 0xaa8
			 0x96ced5c2, /// 0xaac
			 0x12d663d1, /// 0xab0
			 0x6378ebf8, /// 0xab4
			 0x19e077cc, /// 0xab8
			 0x5e29d11f, /// 0xabc
			 0xbb58ea91, /// 0xac0
			 0x88baf7b4, /// 0xac4
			 0xf93750f7, /// 0xac8
			 0xd7eb6fc0, /// 0xacc
			 0x888b98be, /// 0xad0
			 0x74fbb9df, /// 0xad4
			 0x690b7ab2, /// 0xad8
			 0xb45a1430, /// 0xadc
			 0x9a29351e, /// 0xae0
			 0x810dc63c, /// 0xae4
			 0x22456edd, /// 0xae8
			 0xeb6526c3, /// 0xaec
			 0xe6a51e3b, /// 0xaf0
			 0x5140f74d, /// 0xaf4
			 0x06d64eeb, /// 0xaf8
			 0xa0328349, /// 0xafc
			 0xcd38afb5, /// 0xb00
			 0xc9bc996f, /// 0xb04
			 0x635c8402, /// 0xb08
			 0x398210bf, /// 0xb0c
			 0x75e42ba6, /// 0xb10
			 0x1a36f76d, /// 0xb14
			 0x9690bb69, /// 0xb18
			 0x23d15f7f, /// 0xb1c
			 0x31cde0d3, /// 0xb20
			 0x81d4a6af, /// 0xb24
			 0xd5a4fe2c, /// 0xb28
			 0xf5416277, /// 0xb2c
			 0x154ac388, /// 0xb30
			 0xa1e17be8, /// 0xb34
			 0x6d881c81, /// 0xb38
			 0x8f52cd50, /// 0xb3c
			 0xb9c25f37, /// 0xb40
			 0xd36ef864, /// 0xb44
			 0x86268f9e, /// 0xb48
			 0xf853597c, /// 0xb4c
			 0x357fac0e, /// 0xb50
			 0x4f82f274, /// 0xb54
			 0x00e14d90, /// 0xb58
			 0xca7bc0d2, /// 0xb5c
			 0x755a84f0, /// 0xb60
			 0x0aa37fcd, /// 0xb64
			 0x9fbb675f, /// 0xb68
			 0x34c8604f, /// 0xb6c
			 0x9f43612e, /// 0xb70
			 0xd47d3791, /// 0xb74
			 0xf0f90ab7, /// 0xb78
			 0x7f447b48, /// 0xb7c
			 0x83b8409d, /// 0xb80
			 0x355dd412, /// 0xb84
			 0xa55e823d, /// 0xb88
			 0x30d36bba, /// 0xb8c
			 0x2db3bd17, /// 0xb90
			 0xdad5d359, /// 0xb94
			 0x5b71187e, /// 0xb98
			 0x892e8edf, /// 0xb9c
			 0x16cdebb8, /// 0xba0
			 0x9c463677, /// 0xba4
			 0x8f5c7be1, /// 0xba8
			 0xe9b4d768, /// 0xbac
			 0x2f26ed83, /// 0xbb0
			 0x99c5d355, /// 0xbb4
			 0xda4cfc57, /// 0xbb8
			 0xf35a5704, /// 0xbbc
			 0x6c6f17ab, /// 0xbc0
			 0xe8676bf2, /// 0xbc4
			 0x5825a05c, /// 0xbc8
			 0x20f41de3, /// 0xbcc
			 0x8a89eedb, /// 0xbd0
			 0x440ce5be, /// 0xbd4
			 0x5984fc7a, /// 0xbd8
			 0x0fc5f425, /// 0xbdc
			 0xacea92c7, /// 0xbe0
			 0x7ac8dc92, /// 0xbe4
			 0x861cdf29, /// 0xbe8
			 0x15f37765, /// 0xbec
			 0xc0ee21f4, /// 0xbf0
			 0x013863ee, /// 0xbf4
			 0xb3b24910, /// 0xbf8
			 0xaa174c5c, /// 0xbfc
			 0xbb328740, /// 0xc00
			 0xaf087598, /// 0xc04
			 0x6a3b8200, /// 0xc08
			 0x3f550a1b, /// 0xc0c
			 0x5d9d69b2, /// 0xc10
			 0x49b83aa4, /// 0xc14
			 0x54fd64c6, /// 0xc18
			 0x2a64052a, /// 0xc1c
			 0xccc1ccb4, /// 0xc20
			 0x834c4add, /// 0xc24
			 0x9b35ad00, /// 0xc28
			 0x25838f55, /// 0xc2c
			 0x65c6bd06, /// 0xc30
			 0x484aacdb, /// 0xc34
			 0xea004749, /// 0xc38
			 0x8d84db4d, /// 0xc3c
			 0x7e0a73bf, /// 0xc40
			 0x6ef60ff9, /// 0xc44
			 0xe7c427fc, /// 0xc48
			 0xaa9aeec0, /// 0xc4c
			 0x51e68adf, /// 0xc50
			 0xdeedb960, /// 0xc54
			 0xbb0454c3, /// 0xc58
			 0x85438e37, /// 0xc5c
			 0xdede7727, /// 0xc60
			 0x814dbe20, /// 0xc64
			 0xb91d1cec, /// 0xc68
			 0xffecc859, /// 0xc6c
			 0x9599f6e1, /// 0xc70
			 0x7be43da6, /// 0xc74
			 0x2f71b3e0, /// 0xc78
			 0xd98fd222, /// 0xc7c
			 0x6cdfc517, /// 0xc80
			 0xae896ea8, /// 0xc84
			 0x62444d35, /// 0xc88
			 0x213d646b, /// 0xc8c
			 0x0a817fcb, /// 0xc90
			 0xe082593f, /// 0xc94
			 0xc3154de6, /// 0xc98
			 0x701bb2ce, /// 0xc9c
			 0xcb9bb4c1, /// 0xca0
			 0xe66bbc44, /// 0xca4
			 0xf68462c0, /// 0xca8
			 0xfe6e306e, /// 0xcac
			 0x820fad4b, /// 0xcb0
			 0x7c2c2af7, /// 0xcb4
			 0xb1ee510d, /// 0xcb8
			 0xc63fb3b8, /// 0xcbc
			 0x258dc47f, /// 0xcc0
			 0x85ccfcf9, /// 0xcc4
			 0x241a2ace, /// 0xcc8
			 0xef3b56e8, /// 0xccc
			 0xf301b4dc, /// 0xcd0
			 0xe09d66d7, /// 0xcd4
			 0xe6a9a979, /// 0xcd8
			 0xbe175456, /// 0xcdc
			 0xc15e38b6, /// 0xce0
			 0x8a50bd0c, /// 0xce4
			 0xd2065201, /// 0xce8
			 0xb1299ec4, /// 0xcec
			 0x801f0cd1, /// 0xcf0
			 0xe3a2a21c, /// 0xcf4
			 0x42b8d619, /// 0xcf8
			 0xe3923638, /// 0xcfc
			 0xd718425a, /// 0xd00
			 0x36e6f975, /// 0xd04
			 0xa748c620, /// 0xd08
			 0x08d86967, /// 0xd0c
			 0x38d9e246, /// 0xd10
			 0x63162133, /// 0xd14
			 0x8ac144bc, /// 0xd18
			 0x44c63d54, /// 0xd1c
			 0x3fceb408, /// 0xd20
			 0x7e0c8c7d, /// 0xd24
			 0xb7825852, /// 0xd28
			 0x2b4d38e0, /// 0xd2c
			 0x357b08af, /// 0xd30
			 0x910517c6, /// 0xd34
			 0x4414ae51, /// 0xd38
			 0x2bf1bf9b, /// 0xd3c
			 0xba83d798, /// 0xd40
			 0x8c87da3d, /// 0xd44
			 0xc0771858, /// 0xd48
			 0xddc86ca9, /// 0xd4c
			 0xa0fb6669, /// 0xd50
			 0x24dc48ff, /// 0xd54
			 0xa818ae06, /// 0xd58
			 0x7edf99ef, /// 0xd5c
			 0xa1594018, /// 0xd60
			 0x19a7cbbd, /// 0xd64
			 0xb68cd197, /// 0xd68
			 0x9bfe271d, /// 0xd6c
			 0x0cbb995d, /// 0xd70
			 0xa9c89c5e, /// 0xd74
			 0x8a4dceab, /// 0xd78
			 0x455e6b97, /// 0xd7c
			 0x2846a40e, /// 0xd80
			 0xae3926e7, /// 0xd84
			 0xaf3e2431, /// 0xd88
			 0xcaa20e34, /// 0xd8c
			 0xf7ecfaf3, /// 0xd90
			 0xdddb7a07, /// 0xd94
			 0x2ede7641, /// 0xd98
			 0x4ea87602, /// 0xd9c
			 0xeed27297, /// 0xda0
			 0x2d8e599a, /// 0xda4
			 0xd4d5626a, /// 0xda8
			 0x007fb28d, /// 0xdac
			 0x6eb69ef0, /// 0xdb0
			 0x51965442, /// 0xdb4
			 0x52b415ea, /// 0xdb8
			 0xa77bc1c1, /// 0xdbc
			 0xca522742, /// 0xdc0
			 0x938c357b, /// 0xdc4
			 0xe1642950, /// 0xdc8
			 0x1d925756, /// 0xdcc
			 0x8e0ba233, /// 0xdd0
			 0xb372f7f0, /// 0xdd4
			 0x7462a381, /// 0xdd8
			 0x996d830b, /// 0xddc
			 0xc53a6e79, /// 0xde0
			 0xb7827550, /// 0xde4
			 0x5e9e4615, /// 0xde8
			 0x0f20abe1, /// 0xdec
			 0xeac39710, /// 0xdf0
			 0x34471fae, /// 0xdf4
			 0x7bb3eb3c, /// 0xdf8
			 0xe6f6a0f6, /// 0xdfc
			 0xa9d61e61, /// 0xe00
			 0x78ca01de, /// 0xe04
			 0xde7aca76, /// 0xe08
			 0x89cae0d6, /// 0xe0c
			 0x6ba270a6, /// 0xe10
			 0xf046394a, /// 0xe14
			 0xe159d9d1, /// 0xe18
			 0xb5d61e38, /// 0xe1c
			 0xc18c5f31, /// 0xe20
			 0x77158f5d, /// 0xe24
			 0x5e2fdfcd, /// 0xe28
			 0x25190e96, /// 0xe2c
			 0xc57f98c6, /// 0xe30
			 0xadaa6a1e, /// 0xe34
			 0x42e476f5, /// 0xe38
			 0xc1274bb3, /// 0xe3c
			 0xafc8982f, /// 0xe40
			 0xb4a54d32, /// 0xe44
			 0x865a629f, /// 0xe48
			 0x8be2da3f, /// 0xe4c
			 0x3c88cc1a, /// 0xe50
			 0x0c46a2e1, /// 0xe54
			 0x378e6c71, /// 0xe58
			 0xfc57cbbf, /// 0xe5c
			 0x96419271, /// 0xe60
			 0x3145bb2a, /// 0xe64
			 0x846a2da7, /// 0xe68
			 0x757df9c2, /// 0xe6c
			 0x036b2c9c, /// 0xe70
			 0x09f9a73d, /// 0xe74
			 0x42756bd3, /// 0xe78
			 0x6b598213, /// 0xe7c
			 0xca21d913, /// 0xe80
			 0x85a6d17b, /// 0xe84
			 0xbd8e7f3a, /// 0xe88
			 0xbdaae502, /// 0xe8c
			 0x7edef7b3, /// 0xe90
			 0x0bcb1fe5, /// 0xe94
			 0x568d9032, /// 0xe98
			 0x0af94730, /// 0xe9c
			 0x71faa70b, /// 0xea0
			 0x2eee87a0, /// 0xea4
			 0x6c1f7493, /// 0xea8
			 0x5e869240, /// 0xeac
			 0xe793d7bb, /// 0xeb0
			 0x8d496ab3, /// 0xeb4
			 0x6331cf8e, /// 0xeb8
			 0xd8f761ca, /// 0xebc
			 0x77ae2532, /// 0xec0
			 0xdfff7deb, /// 0xec4
			 0xfd938484, /// 0xec8
			 0x0d5281e5, /// 0xecc
			 0x93d36008, /// 0xed0
			 0xb5535cda, /// 0xed4
			 0xbb64c7e3, /// 0xed8
			 0x39723667, /// 0xedc
			 0xccbe1fc4, /// 0xee0
			 0x1f96730e, /// 0xee4
			 0x4f64cf9b, /// 0xee8
			 0xd4cab010, /// 0xeec
			 0xa2c39800, /// 0xef0
			 0x86e0de32, /// 0xef4
			 0xad8bd9e9, /// 0xef8
			 0x0316294c, /// 0xefc
			 0x2ad95d90, /// 0xf00
			 0x8e556a6f, /// 0xf04
			 0xa370f3a6, /// 0xf08
			 0x980dd5aa, /// 0xf0c
			 0xb1671251, /// 0xf10
			 0x9ab06133, /// 0xf14
			 0x3bdfc716, /// 0xf18
			 0x28aa2d52, /// 0xf1c
			 0xa98b7fbd, /// 0xf20
			 0x11efb5f5, /// 0xf24
			 0x511d3a9c, /// 0xf28
			 0xf054d8ab, /// 0xf2c
			 0xd4e74bf7, /// 0xf30
			 0x7cc6cb41, /// 0xf34
			 0x8caad1e0, /// 0xf38
			 0xc2d6b57b, /// 0xf3c
			 0x3752fecc, /// 0xf40
			 0x290054c6, /// 0xf44
			 0x1af39bc4, /// 0xf48
			 0xd1ebdd73, /// 0xf4c
			 0xae71da64, /// 0xf50
			 0x00b7907f, /// 0xf54
			 0xfe3cf6ac, /// 0xf58
			 0x1f7003d1, /// 0xf5c
			 0x213b1a88, /// 0xf60
			 0x711f3289, /// 0xf64
			 0xd42fd9af, /// 0xf68
			 0xdbf419f0, /// 0xf6c
			 0x70a936fe, /// 0xf70
			 0x7e35bf42, /// 0xf74
			 0x5d0fd15d, /// 0xf78
			 0x3640a9cc, /// 0xf7c
			 0x6e28b7d7, /// 0xf80
			 0x90baa39b, /// 0xf84
			 0x9e53d301, /// 0xf88
			 0xfd242f38, /// 0xf8c
			 0x6407d10b, /// 0xf90
			 0x4ca5a1f5, /// 0xf94
			 0xc5eb55b7, /// 0xf98
			 0xb3b7a1f1, /// 0xf9c
			 0x54822390, /// 0xfa0
			 0xb107a66f, /// 0xfa4
			 0xeb3118d2, /// 0xfa8
			 0xff81b740, /// 0xfac
			 0x41c578e0, /// 0xfb0
			 0xa33b9337, /// 0xfb4
			 0x0e4b87a2, /// 0xfb8
			 0x4123fb33, /// 0xfbc
			 0xeb96c7b7, /// 0xfc0
			 0x1bd9a67a, /// 0xfc4
			 0x05ed3f53, /// 0xfc8
			 0xc7a306ea, /// 0xfcc
			 0xe3df1513, /// 0xfd0
			 0xff14e145, /// 0xfd4
			 0x586895ce, /// 0xfd8
			 0xf2a0995f, /// 0xfdc
			 0x483b56a3, /// 0xfe0
			 0x08b7f612, /// 0xfe4
			 0x72c82e2f, /// 0xfe8
			 0xe4c71a46, /// 0xfec
			 0xdb5514d5, /// 0xff0
			 0x51b683e3, /// 0xff4
			 0x90812d6f, /// 0xff8
			 0x9ba2e1e5 /// last
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
			 0x00000354,
			 0x00000760,
			 0x00000620,
			 0x000003c8,
			 0x00000368,
			 0x0000025c,
			 0x00000664,
			 0x000002c0,

			 /// vpu register f2
			 0x41a00000,
			 0x41f80000,
			 0x40400000,
			 0x41100000,
			 0x41c00000,
			 0x41300000,
			 0x41b80000,
			 0x42000000,

			 /// vpu register f3
			 0x3f800000,
			 0x41d80000,
			 0x41e00000,
			 0x41a80000,
			 0x41000000,
			 0x41a00000,
			 0x41c80000,
			 0x41600000,

			 /// vpu register f4
			 0x3f800000,
			 0x41900000,
			 0x41600000,
			 0x41a80000,
			 0x41b80000,
			 0x41f00000,
			 0x41a00000,
			 0x40000000,

			 /// vpu register f5
			 0x40800000,
			 0x41300000,
			 0x41d80000,
			 0x41000000,
			 0x41700000,
			 0x41d00000,
			 0x42000000,
			 0x41200000,

			 /// vpu register f6
			 0x41e80000,
			 0x41900000,
			 0x41f00000,
			 0x41a80000,
			 0x40000000,
			 0x40c00000,
			 0x41f00000,
			 0x41700000,

			 /// vpu register f7
			 0x41800000,
			 0x41200000,
			 0x41e00000,
			 0x41800000,
			 0x41f00000,
			 0x41a80000,
			 0x41b80000,
			 0x40800000,

			 /// vpu register f8
			 0x40800000,
			 0x41200000,
			 0x41600000,
			 0x41c80000,
			 0x40c00000,
			 0x41100000,
			 0x41e80000,
			 0x40400000,

			 /// vpu register f9
			 0x40c00000,
			 0x41b80000,
			 0x40800000,
			 0x41980000,
			 0x41200000,
			 0x40c00000,
			 0x41c00000,
			 0x41900000,

			 /// vpu register f10
			 0x41c00000,
			 0x41e00000,
			 0x40400000,
			 0x41400000,
			 0x42000000,
			 0x41600000,
			 0x41d00000,
			 0x41b00000,

			 /// vpu register f11
			 0x40400000,
			 0x40000000,
			 0x41300000,
			 0x41a80000,
			 0x41e00000,
			 0x41600000,
			 0x40400000,
			 0x40a00000,

			 /// vpu register f12
			 0x41700000,
			 0x41600000,
			 0x41000000,
			 0x41700000,
			 0x41500000,
			 0x41a80000,
			 0x40e00000,
			 0x41a00000,

			 /// vpu register f13
			 0x40800000,
			 0x41600000,
			 0x40800000,
			 0x41000000,
			 0x41880000,
			 0x40000000,
			 0x41a80000,
			 0x40e00000,

			 /// vpu register f14
			 0x40400000,
			 0x41c00000,
			 0x42000000,
			 0x41d00000,
			 0x41200000,
			 0x41d00000,
			 0x40400000,
			 0x40e00000,

			 /// vpu register f15
			 0x40e00000,
			 0x41e00000,
			 0x41c80000,
			 0x41000000,
			 0x41300000,
			 0x40800000,
			 0x41800000,
			 0x41e80000,

			 /// vpu register f16
			 0x41200000,
			 0x40000000,
			 0x41000000,
			 0x41d00000,
			 0x41700000,
			 0x40a00000,
			 0x40800000,
			 0x41200000,

			 /// vpu register f17
			 0x41e00000,
			 0x41200000,
			 0x41200000,
			 0x41900000,
			 0x41880000,
			 0x40c00000,
			 0x40000000,
			 0x41400000,

			 /// vpu register f18
			 0x3f800000,
			 0x40800000,
			 0x41d80000,
			 0x40800000,
			 0x41a00000,
			 0x41f00000,
			 0x41c80000,
			 0x41a00000,

			 /// vpu register f19
			 0x41700000,
			 0x41c80000,
			 0x41d80000,
			 0x41c80000,
			 0x41800000,
			 0x41500000,
			 0x41f80000,
			 0x41300000,

			 /// vpu register f20
			 0x41400000,
			 0x41a80000,
			 0x41100000,
			 0x40800000,
			 0x41000000,
			 0x40400000,
			 0x41d80000,
			 0x41d80000,

			 /// vpu register f21
			 0x40000000,
			 0x40400000,
			 0x41600000,
			 0x41d00000,
			 0x40a00000,
			 0x41980000,
			 0x41400000,
			 0x41300000,

			 /// vpu register f22
			 0x3f800000,
			 0x41b00000,
			 0x40c00000,
			 0x41000000,
			 0x41600000,
			 0x41400000,
			 0x41e00000,
			 0x41d00000,

			 /// vpu register f23
			 0x3f800000,
			 0x41d80000,
			 0x41980000,
			 0x3f800000,
			 0x41a00000,
			 0x41400000,
			 0x40000000,
			 0x41800000,

			 /// vpu register f24
			 0x41a00000,
			 0x41a80000,
			 0x41100000,
			 0x41e80000,
			 0x41500000,
			 0x41400000,
			 0x41200000,
			 0x41f80000,

			 /// vpu register f25
			 0x40a00000,
			 0x41a00000,
			 0x41300000,
			 0x41980000,
			 0x41b00000,
			 0x41500000,
			 0x41f00000,
			 0x41b00000,

			 /// vpu register f26
			 0x41800000,
			 0x41d80000,
			 0x41100000,
			 0x41300000,
			 0x40a00000,
			 0x41700000,
			 0x41a00000,
			 0x40a00000,

			 /// vpu register f27
			 0x3f800000,
			 0x40400000,
			 0x41000000,
			 0x41100000,
			 0x40000000,
			 0x41300000,
			 0x41b80000,
			 0x41500000,

			 /// vpu register f28
			 0x41a00000,
			 0x40800000,
			 0x41300000,
			 0x41d00000,
			 0x41d80000,
			 0x3f800000,
			 0x41f00000,
			 0x41c80000,

			 /// vpu register f29
			 0x41800000,
			 0x41e80000,
			 0x41200000,
			 0x41f80000,
			 0x40000000,
			 0x40000000,
			 0x40000000,
			 0x41e80000,

			 /// vpu register f30
			 0x40a00000,
			 0x41e00000,
			 0x41f80000,
			 0x40c00000,
			 0x41600000,
			 0x3f800000,
			 0x41000000,
			 0x41e00000,

			 /// vpu register f31
			 0x41100000,
			 0x41000000,
			 0x41000000,
			 0x40a00000,
			 0x40e00000,
			 0x40c00000,
			 0x41d80000,
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
		"flt.pi f29, f7, f21\n"                               ///  1,     0
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f26, f26, f3\n"                               ///  1,     1
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f10, f25, f3\n"                               ///  1,     2
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f6, f21, f8\n"                                ///  1,     3
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f24, f22, f29\n"                              ///  1,     4
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f20, f9, f5\n"                                ///  1,     5
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f6, f0, f25\n"                                ///  1,     6
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f16, f26, f14\n"                              ///  1,     7
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f16, f18, f2\n"                               ///  1,     8
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f11, f5, f16\n"                               ///  1,     9
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f25, f4, f8\n"                                ///  1,    10
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f24, f25, f17\n"                              ///  1,    11
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f12, f3, f21\n"                               ///  1,    12
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f21, f5, f9\n"                                ///  1,    13
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f15, f12, f14\n"                              ///  1,    14
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f29, f18, f14\n"                              ///  1,    15
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f19, f24, f14\n"                              ///  1,    16
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f25, f22, f14\n"                              ///  1,    17
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f19, f23, f3\n"                               ///  1,    18
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f30, f30, f8\n"                               ///  1,    19
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f12, f6, f10\n"                               ///  1,    20
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f18, f18, f26\n"                              ///  1,    21
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f28, f27, f20\n"                              ///  1,    22
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f27, f30, f13\n"                              ///  1,    23
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f6, f18, f26\n"                               ///  1,    24
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f26, f18, f18\n"                              ///  1,    25
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f2, f1, f29\n"                                ///  1,    26
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f10, f21, f16\n"                              ///  1,    27
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f16, f12, f15\n"                              ///  1,    28
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f30, f21, f4\n"                               ///  1,    29
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f24, f28, f12\n"                              ///  1,    30
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f17, f24, f22\n"                              ///  1,    31
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f26, f8, f2\n"                                ///  1,    32
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f30, f2, f6\n"                                ///  1,    33
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f4, f14, f23\n"                               ///  1,    34
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f14, f19, f24\n"                              ///  1,    35
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f4, f10, f5\n"                                ///  1,    36
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f24, f11, f15\n"                              ///  1,    37
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f7, f5, f21\n"                                ///  1,    38
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f20, f24, f1\n"                               ///  1,    39
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f21, f5, f26\n"                               ///  1,    40
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f23, f10, f7\n"                               ///  1,    41
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f4, f29, f11\n"                               ///  1,    42
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f19, f2, f27\n"                               ///  1,    43
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f18, f28, f13\n"                              ///  1,    44
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f15, f28, f8\n"                               ///  1,    45
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f21, f15, f10\n"                              ///  1,    46
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f29, f5, f9\n"                                ///  1,    47
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f28, f1, f9\n"                                ///  1,    48
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f5, f2, f14\n"                                ///  1,    49
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f16, f3, f14\n"                               ///  1,    50
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f6, f13, f10\n"                               ///  1,    51
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f14, f2, f19\n"                               ///  1,    52
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f30, f22, f28\n"                              ///  1,    53
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f28, f8, f26\n"                               ///  1,    54
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f6, f7, f4\n"                                 ///  1,    55
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f28, f28, f22\n"                              ///  1,    56
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f24, f29, f8\n"                               ///  1,    57
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f4, f6, f24\n"                                ///  1,    58
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f10, f16, f8\n"                               ///  1,    59
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f10, f28, f30\n"                              ///  1,    60
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f20, f15, f21\n"                              ///  1,    61
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f14, f14, f1\n"                               ///  1,    62
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f4, f11, f12\n"                               ///  1,    63
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f20, f19, f28\n"                              ///  1,    64
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f11, f3, f12\n"                               ///  1,    65
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f19, f3, f19\n"                               ///  1,    66
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f4, f2, f29\n"                                ///  1,    67
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f17, f4, f3\n"                                ///  1,    68
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f2, f4, f18\n"                                ///  1,    69
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f20, f15, f15\n"                              ///  1,    70
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f6, f0, f1\n"                                 ///  1,    71
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f28, f23, f23\n"                              ///  1,    72
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f30, f6, f3\n"                                ///  1,    73
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f13, f15, f3\n"                               ///  1,    74
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f14, f10, f4\n"                               ///  1,    75
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f25, f3, f6\n"                                ///  1,    76
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f24, f7, f1\n"                                ///  1,    77
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f11, f20, f24\n"                              ///  1,    78
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f17, f13, f13\n"                              ///  1,    79
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f3, f4, f7\n"                                 ///  1,    80
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f24, f20, f27\n"                              ///  1,    81
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f8, f4, f18\n"                                ///  1,    82
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f17, f0, f28\n"                               ///  1,    83
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f5, f23, f26\n"                               ///  1,    84
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f4, f22, f14\n"                               ///  1,    85
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f21, f16, f20\n"                              ///  1,    86
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f4, f2, f15\n"                                ///  1,    87
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f14, f5, f24\n"                               ///  1,    88
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f23, f4, f26\n"                               ///  1,    89
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f28, f4, f3\n"                                ///  1,    90
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f9, f10, f22\n"                               ///  1,    91
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f30, f3, f22\n"                               ///  1,    92
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f25, f2, f25\n"                               ///  1,    93
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f18, f4, f1\n"                                ///  1,    94
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f17, f19, f11\n"                              ///  1,    95
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f19, f30, f2\n"                               ///  1,    96
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f30, f18, f19\n"                              ///  1,    97
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"nop\n"
		"flt.pi f24, f15, f10\n"                              ///  1,    98
		VSNIP_RSV
	);
	__asm__ __volatile__ (
		"LBL_C_TEST_PASS:\n"
		VSNIP_RSV
	);
	C_TEST_PASS;
	return 0;
}
